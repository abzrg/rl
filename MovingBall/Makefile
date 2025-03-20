CFLAGS      = -Wall -Wextra -g -std=c17 -pedantic
# CFLAGS     += `pkg-config --cflags --libs glfw3`
CFLAGS     += `pkg-config --cflags --libs raylib`

all: build/main

build:
	mkdir -pv $@

build/main: main.c | build
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) main
