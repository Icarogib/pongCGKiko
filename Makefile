CC = g++

all:
	$(CC) pong.cpp -lglut -lGLU -lGL -I. -o pongz

clean:
	rm pongz
