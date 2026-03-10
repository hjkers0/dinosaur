PATH_RAYLIB_WEB=/home/hjkers/.local/share/raylib-web/src 

all: web
game: main.c nn.c obstacle.c
	gcc -Wall -Wextra main.c nn.c obstacle.c -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./game

web:
	emcc -o game.html main.c nn.c obstacle.c \
		-Os -Wall \
		-L$(PATH_RAYLIB_WEB) \
		-I$(PATH_RAYLIB_WEB) \
		-lraylib \
		-s USE_GLFW=3 \
		-s ASYNCIFY \
		-DPLATFORM_WEB \
		-lGL -lm

