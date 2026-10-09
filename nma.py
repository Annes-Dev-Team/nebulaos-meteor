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

    "LOADSTR": 15
}


def u32(value):
    """Encode a 32-bit unsigned integer as big-endian."""
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

    name = parts[1]

    string = parts[2]

    if len(string) < 2 or string[0] != '"' or string[-1] != '"':
        raise ValueError(
            'DATSTRING string must be surrounded by quotes'
        )

    # Remove quotes.
    string = string[1:-1]

    return name, string


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
        # Is this a DATSTRING label?
        if arg in labels:
            value = labels[arg]
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

    # ---------------------------------------------------------
    # PASS 1
    #
    # Calculate where instructions and data will live.
    # ---------------------------------------------------------

    labels = {}

    pc = 0

    for line_number, raw_line in enumerate(lines, 1):

        line = strip_comment(raw_line)

        if not line:
            continue

        parts = line.split(None, 1)

        instruction = parts[0].upper()

        if instruction == "DATSTRING":
            try:
                name, string = parse_datstring(line)
            except ValueError as e:
                raise ValueError(f"Line {line_number}: {e}")

            if name in labels:
                raise ValueError(
                    f"Line {line_number}: Duplicate label '{name}'"
                )

            # The string itself will be appended after the code.
            #
            # We don't know its final address yet, so store the
            # string for now.
            labels[name] = None

        else:
            try:
                pc += instruction_size(line)
            except ValueError as e:
                raise ValueError(f"Line {line_number}: {e}")

    # ---------------------------------------------------------
    # Calculate data addresses.
    #
    # Strings are placed immediately after the instruction code.
    # ---------------------------------------------------------

    code_size = pc

    data_address = code_size
    data = bytearray()

    for line_number, raw_line in enumerate(lines, 1):

        line = strip_comment(raw_line)

        if not line:
            continue

        parts = line.split(None, 1)

        if parts[0].upper() != "DATSTRING":
            continue

        try:
            name, string = parse_datstring(line)
        except ValueError as e:
            raise ValueError(f"Line {line_number}: {e}")

        # Store the address of this string.
        labels[name] = data_address

        # UTF-8 string + NULL terminator.
        encoded = string.encode("utf-8") + b"\0"

        data += encoded
        data_address += len(encoded)

    # ---------------------------------------------------------
    # PASS 2
    #
    # Actually assemble the instructions.
    # ---------------------------------------------------------

    output = bytearray()

    for line_number, raw_line in enumerate(lines, 1):

        line = strip_comment(raw_line)

        if not line:
            continue

        if line.split(None, 1)[0].upper() == "DATSTRING":
            continue

        try:
            output += assemble_instruction(line, labels)
        except ValueError as e:
            raise ValueError(f"Line {line_number}: {e}")

    # ---------------------------------------------------------
    # Append data section.
    # ---------------------------------------------------------

    output += data

    return output


def main():
    if len(sys.argv) != 3:
        print("Usage: python3 nma.py input.nma output.neb")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    with open(input_file, "r", encoding="utf-8") as f:
        source = f.read()

    try:
        binary = assemble(source)
    except ValueError as e:
        print(f"Assembler error: {e}")
        sys.exit(1)

    with open(output_file, "wb") as f:
        f.write(binary)

    print(f"Assembled {len(binary)} bytes → {output_file}")


if __name__ == "__main__":
    main()
