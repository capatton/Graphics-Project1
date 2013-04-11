TARGET   = glutharness

CC       = g++
CFLAGS   = -c -g -DDEBUG -DLINUX
LDFLAGS  = -lGL -lGLU -lglut -lGLEW
SRC      = $(wildcard ../main.cpp ../*.cpp)
OBJ      = $(SRC:.cpp=.o)

all: $(TARGET)
	
$(TARGET): $(OBJ) 
	$(CC) $< -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ../*.o
	rm -f $(TARGET)