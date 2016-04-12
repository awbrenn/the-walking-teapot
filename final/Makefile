cc = clang
objs = main.o utilities.o objParser.o
flags = -L/usr/lib64 -lX11 -lGL -lGLU -lglut -lm

all: $(objs)
	$(cc) -g -o render $(objs) $(flags)

main.o : utilities.h objParser.h
objParser.o: utilities.h

clean:
	rm render $(objs)
