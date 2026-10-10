build:
	mkdir -p NebulaOS\ Meteor.app/Contents/MacOS/
	mkdir -p NebulaOS\ Meteor.app/Contents/Resources/

	python nma.py test.nma assets/programs/test.neb
	python nma.py starcatcher.nma assets/programs/starcatcher.neb

	cp Info.plist NebulaOS\ Meteor.app/Contents/
	cp -r assets/* NebulaOS\ Meteor.app/Contents/Resources

	gcc src/*.c -lraylib -Iinclude -o NebulaOS\ Meteor.app/Contents/MacOS/nebmeteor \
		-framework IOKit -framework Cocoa -framework CoreGraphics -framework CoreAudio | tee compile.log

run: build
	./NebulaOS\ Meteor.app/Contents/MacOS/nebmeteor | tee run.log
	/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister \
		-f ./NebulaOS\ Meteor.app
