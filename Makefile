default:imgui
	g++ -Wall src/main.cpp \
		-g \
		fluidsim.o \
		imgui*.o \
		particle.o \
		-std=c++20 \
		-Ilib/fluidsim/ \
		-Ilib/color/src/color/ \
		-Ilib/imgui/ \
		-lSDL2 -lSDL2_image \
		-Wall `sdl2-config --cflags --libs` \
		-o out 
clean:
	rm -f ./out ./*.o
run: default
	./out
particle.o:
	g++ -Wall -c src/particle.cpp -o particle.o
fluidsim.o:
	g++ -Wall -c -Ilib/fluidsim/ lib/fluidsim/fluidsim.cpp -o fluidsim.o
imgui:
	g++ -c -Wall \
	lib/imgui/*.cpp \
	-I/usr/include/SDL2 -D_REENTRANT
