
import sys
import struct

# Keep these synchronized with nebin.h

OPCODES = {
    "NOP": 0,
    "MOV": 1,
    "JMP": 2,

    "ALLOC": 3,
    "FREE": 4,
    "MOVI": 5,
    "MOVP": 6,

    "OSFLAG": 7,

    "INITBUF": 8,

    "PUTPIXEL": 9,
    "RECTANGLE": 10,
    "CIRCLE": 11,

    "READFILE": 12,
    "SETARRAYINDEX": 13,

    "ADD": 14,

    "LOADSTR": 15,

    "CMPE":16,
    "CMPG":17,
    "CMPGE":18,
    "JE":19,
    "JNE":20,

    "SUB":21,
    "MUL":22,
    "DIV":23,

    "RECTREG":24,
    "CIRCREG":25,
}

def u32(value):
    """Encode a 32-bit unsigned integer as big-endian."""
    if not 0 <= value <= 0xFFFFFFFF:
        raise ValueError(f"Value out of 32-bit range: {value}")

    return struct.pack(">I", value)


def number(value):
    """Parse decimal or hexadecimal numbers."""
    return int(value, 0)


def instruction_size(line):
    """Return the number of bytes an instruction produces."""
    parts = line.split()
    instruction = parts[0].upper()

    if instruction not in OPCODES:
        raise ValueError(f"Unknown instruction: {instruction}")

    # 1 byte opcode + 4 bytes per argument
    return 1 + (len(parts) - 1) * 4


def parse_datstring(line):
    """
    Parse:
        DATSTRING NAME "Some string"

    Returns:
        name, string
    """
    parts = line.split(None, 2)

    if len(parts) != 3:
        raise ValueError(
            'DATSTRING requires a name and string, e.g. '
            'DATSTRING TITLE "Neb Test Program"'
        )

    name = parts[1].upper()
    string = parts[2]

    if len(string) < 2 or string[0] != '"' or string[-1] != '"':
        raise ValueError(
            "DATSTRING string must be surrounded by quotes"
        )

    return name, string[1:-1]


def parse_jmppoint(line):
    """
    Parse:
        JMPPOINT NAME

    Returns:
        name
    """
    parts = line.split()

    if len(parts) != 2:
        raise ValueError(
            "JMPPOINT requires exactly one name"
        )

    return parts[1].upper()


def strip_comment(line):
    """Remove comments while preserving the rest of the line."""
    return line.split(";", 1)[0].strip()


def assemble_instruction(line, labels):
    parts = line.split()

    instruction = parts[0].upper()
    args = parts[1:]

    if instruction not in OPCODES:
        raise ValueError(f"Unknown instruction: {instruction}")

    opcode = OPCODES[instruction]

    output = bytearray()
    output.append(opcode)

    for arg in args:
        # Resolve DATSTRING labels and JMPPOINT labels.
        name = arg.upper()

        if name in labels:
            value = labels[name]

            if value is None:
                raise ValueError(
                    f"Unresolved label: {arg}"
                )
        else:
            try:
                value = number(arg)
            except ValueError:
                raise ValueError(
                    f"Unknown value or label: {arg}"
                )

        output += u32(value)

    return output


def assemble(source):
    lines = source.splitlines()

    labels = {}
    pc = 0

    # ---------------------------------------------------------
    # PASS 1
    #
    # Calculate instruction addresses and register jump points.
    # JMPPOINT does not generate any bytes.
    # ---------------------------------------------------------

    for line_number, raw_line in enumerate(lines, 1):
        line = strip_comment(raw_line)

        if not line:
            continue

        parts = line.split(None, 1)
        instruction = parts[0].upper()

        try:
            if instruction == "JMPPOINT":
                name = parse_jmppoint(line)

                if name in labels:
                    raise ValueError(
                        f"Duplicate label '{name}'"
                    )

                # The jump target is the current code address.
                labels[name] = pc

            elif instruction == "DATSTRING":
                name, _ = parse_datstring(line)

                if name in labels:
                    raise ValueError(
                        f"Duplicate label '{name}'"
                    )

                # String addresses will be assigned after code sizing.
                labels[name] = None

            else:
                pc += instruction_size(line)

        except ValueError as e:
            raise ValueError(f"Line {line_number}: {e}")

    # ---------------------------------------------------------
    # PASS 2
    #
    # Place strings immediately after the instruction code.
    # ---------------------------------------------------------

    code_size = pc
    data_address = code_size
    data = bytearray()

    for line_number, raw_line in enumerate(lines, 1):
        line = strip_comment(raw_line)

        if not line:
            continue

        if line.split(None, 1)[0].upper() != "DATSTRING":
            continue

        try:
            name, string = parse_datstring(line)

            encoded = string.encode("utf-8") + b"\0"

            labels[name] = data_address

            data.extend(encoded)
            data_address += len(encoded)

        except ValueError as e:
            raise ValueError(f"Line {line_number}: {e}")

    # ---------------------------------------------------------
    # PASS 3
    #
    # Assemble instructions using the resolved addresses.
    # ---------------------------------------------------------

    output = bytearray()

    for line_number, raw_line in enumerate(lines, 1):
        line = strip_comment(raw_line)

        if not line:
            continue

        instruction = line.split(None, 1)[0].upper()

        # Directives generate no instruction bytes.
        if instruction in ("DATSTRING", "JMPPOINT"):
            continue

        try:
            output.extend(
                assemble_instruction(line, labels)
            )

        except ValueError as e:
            raise ValueError(f"Line {line_number}: {e}")

    # Append the null-terminated UTF-8 strings.
    output.extend(data)

    return output


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 nma.py input.nma output.neb")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    try:
        with open(input_file, "r", encoding="utf-8") as f:
            source = f.read()

        binary = assemble(source)

        with open(output_file, "wb") as f:
            f.write(binary)

    except (ValueError, OSError) as e:
        print(f"Assembler error: {e}")
        sys.exit(1)

    print(f"Assembled {len(binary)} bytes → {output_file}")


if __name__ == "__main__":
    main()
