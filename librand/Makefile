OBJECTS += librand.o
LIBNAME = librand.so
CFLAGS += -g -Wall

all: $(OBJECTS)
	@echo "  [LINK ] Linking into shared library $(LIBNAME)"
	@gcc -shared -o $(LIBNAME) $(OBJECTS) -lc

%.o: %.c
	@echo "  [COMPL] Compiling source file $<"
	@gcc -fPIC $(CFLAGS) -c $< -o $@

clean:
	@echo "  [CLEAN] Cleaning compilation data"
	@rm *.o *.so