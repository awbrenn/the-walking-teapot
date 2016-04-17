cc = clang
objs = main.o utilities.o objParser.o
flags = -g -L/usr/lib64 -lX11 -lGL -lGLU -lglut -lm

all: $(objs)
	$(cc) -o render $(objs) $(flags)

main.o : main.c utilities.h objParser.h
	$(cc) -c main.c $(flags)
utilities.o: utilities.c utilities.h
	$(cc) -c utilities.c $(flags)
objParser.o: objParser.h objParser.c
	$(cc) -c objParser.c $(flags)
clean:
	rm render $(objs)
