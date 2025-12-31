CC      = cc
CFLAGS  = -Wall -Wextra -Werror -std=c11
DEBUG   = -g -O0
RELEASE = -O2 -DNDEBUG
PROGRAM_NAME = "app"

debug: ./src/main.c
	$(CC) $(CFLAGS) $(DEBUG) $< -o ./build/$(PROGRAM_NAME)

release: ./src/main.c
	$(CC) $(CFLAGS) $(RELEASE) $< -o ./build/$(PROGRAM_NAME)

run: debug
	./build/$(PROGRAM_NAME)

clean:
	rm -rf ./app_debug*/ ./build/$(PROGRAM_NAME)
