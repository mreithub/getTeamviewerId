CC=i586-mingw32msvc-gcc
TARGET=teamviewer-getid.exe

all: $(TARGET)
	
$(TARGET): getid.o
	$(CC) getid.o -o "$(TARGET)"

clean:
	rm *.o "$(TARGET)"
