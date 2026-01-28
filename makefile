CC      = cc
CFLAGS  = -Wall -fsanitize=address -Wextra -Werror -std=c11
DEBUG   = -g -O0
RELEASE = -O2 -DNDEBUG
BUILD_DIR = ./build
PROGRAM_NAME = app
SRC = ./src/*.c ./src/**/*.c ./lib/object_pool/object_pool.c

OBJECT_POOL_SRC = ./lib/object_pool/object_pool_test.c ./lib/object_pool/object_pool.c
GUN_SRC = ./test/player_gun_test.c ./src/utils/math.c

debug: $(SRC)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $(BUILD_DIR)/$(PROGRAM_NAME)

release: $(SRC)
	$(CC) $(CFLAGS) $(RELEASE) $^ -o $(BUILD_DIR)/$(PROGRAM_NAME)

run: debug
	./build/$(PROGRAM_NAME)


test_object_pool: $(OBJECT_POOL_SRC)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $(BUILD_DIR)/test_object_pool && ./build/test_object_pool

test_gun: $(GUN_SRC)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $(BUILD_DIR)/test_gun && ./build/test_gun

test: test_object_pool

clean:
	rm -rf ../build/*

loc:
	find . \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 wc -l | sort

