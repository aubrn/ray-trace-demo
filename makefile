
all: sphereRT

# framework information for older version of MACOS
#INCLUDES = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# frameworks for newer MACOS, where include files are moved 
INCLUDES = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

TARGET = -mmacosx-version-min=10.8

sphereRT: sphereRT.c
	clang sphereRT.c -o sphereRT $(INCLUDES) $(TARGET)


