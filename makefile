CC      = cc
CFLAGS  = -Wall -Wextra -Werror -std=c11
DEBUG   = -g -O0
RELEASE = -O2 -DNDEBUG
BUILD_DIR = ./build
PROGRAM_NAME = app
SRC = ./src/*.c

debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $(BUILD_DIR)/$(PROGRAM_NAME)

release: $(SRC)
	$(CC) $(CFLAGS) $(RELEASE) $^ -o $(BUILD_DIR)/$(PROGRAM_NAME)

run: debug
	./build/$(PROGRAM_NAME)

clean:
	rm -rf ./app_debug*/ ./build/$(PROGRAM_NAME)
