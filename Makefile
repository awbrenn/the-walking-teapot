cc = clang
objs = main.o utilities.o objParser.o
flags = -g -L/usr/lib64 -lX11 -lGL -lGLU -lglut -lm

all: $(objs)
	$(cc) -o teapot $(objs) $(flags)

main.o : utilities.h objParser.h
objParser.o: utilities.h

clean:
	rm teapot $(objs)
