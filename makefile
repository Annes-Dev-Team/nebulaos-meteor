build:
	mkdir -p NebulaOS\ Meteor.app/Contents/MacOS/
	mkdir -p NebulaOS\ Meteor.app/Contents/Resources/
	cp Info.plist NebulaOS\ Meteor.app/Contents/
	cp assets/* NebulaOS\ Meteor.app/Contents/Resources

	gcc src/*.c -lraylib -Iinclude -o NebulaOS\ Meteor.app/Contents/MacOS/nebmeteor \
		-framework IOKit -framework Cocoa -framework CoreGraphics -framework CoreAudio
	

run: build
	./NebulaOS\ Meteor.app/Contents/MacOS/nebmeteor
