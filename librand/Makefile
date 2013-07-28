OBJECTS += librand.o

all: $(OBJECTS)
	@# right now, don't do anything. In the future this will be a shared library call?

%.o: %.c
	gcc -fPIC -g -Wall -c $< -o $@