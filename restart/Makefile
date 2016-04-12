cc = clang++
objs = main.o Scene.o Object.o Vec3.o Vec2.o
flags = -std=c++14 -L/usr/lib64 -lX11 -lGL -lGLU -lglut -lm

all: $(objs)
	$(cc) -g -o render $(objs) $(flags)

main.o : main.cpp Vec3.hpp
Scene.o : Scene.cpp Object.hpp
	$(cc) -c Scene.cpp $(flags)
Object.o : Object.hpp
	$(cc) -c Object.cpp $(flags)
Vec3.o:  Vec3.cpp Vec3.hpp
	$(cc) -c Vec3.cpp $(flags)
Vec2.o: Vec2.cpp Vec2.hpp
	$(cc) -c Vec2.cpp $(flags)

clean:
	rm render $(objs)
