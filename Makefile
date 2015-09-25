CC=i586-mingw32msvc-gcc
TARGET=teamviewer-getid.exe

CFLAGS+=-Wall

all: $(TARGET)
	
$(TARGET): getid.o
	$(CC) $(LDFLAGS) getid.o -o "$(TARGET)"

clean:
	rm *.o "$(TARGET)"
