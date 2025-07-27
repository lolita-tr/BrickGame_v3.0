CC = gcc
CXX = g++
CFLAGS = -std=c++17 -pthread -Wall -Werror -Wextra -lstdc++
FLAGS = -Wall -Werror -Wextra

COMMON_DIR = brick_game/common
TET_DIR = brick_game/tetris
SNAKE_DIR = brick_game/snake
BUILD_DIR = build

OS := $(shell uname -s)

ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
	TEST_LIBS = -lgtest -lgtest_main -lsubunit -lrt -lm
	TEST_LIBS_TET = -lcheck -lsubunit -lrt -lm -pthread -lncurses
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
	TEST_LIBS = -L/usr/local/lib -lgtest -lgtest_main
	TEST_LIBS_TET = -lcheck
endif

TEST_FILES_SNAKE = tests/test_snake.cpp $(SNAKE_DIR)/snake.cpp
TEST_FILES_TETRIS = tests/test_tetris.c $(TET_DIR)/field.c $(TET_DIR)/figure.c $(TET_DIR)/fsm.c $(TET_DIR)/utility.c

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

all: clean install

install: $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a
	$(CXX) $(FLAGS) -o $(BUILD_DIR)/Console gui/cli/main_console.cpp \
	$(SNAKE_DIR)/snake_view.cpp gui/cli/tetris_frontend.c \
	$(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/snake_lib.a -lncurses

#   TODO:
#	cd $(BUILD_DIR) && /usr/local/Qt-6.6.2/bin/qmake ../gui/desktop/brick_game && make не собирается qt надо подумать как сделать

$(BUILD_DIR)/tetris_lib.a: $(BUILD_DIR)/field.o $(BUILD_DIR)/figure.o \
	$(BUILD_DIR)/fsm.o $(BUILD_DIR)/utility.o $(BUILD_DIR)/game_common.o
	ar rcs $(BUILD_DIR)/tetris_lib.a $(BUILD_DIR)/field.o $(BUILD_DIR)/figure.o \
	$(BUILD_DIR)/fsm.o $(BUILD_DIR)/utility.o $(BUILD_DIR)/game_common.o
	ranlib $(BUILD_DIR)/tetris_lib.a

$(BUILD_DIR)/field.o: $(TET_DIR)/field.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(TET_DIR)/field.c -o $(BUILD_DIR)/field.o

$(BUILD_DIR)/figure.o: $(TET_DIR)/figure.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(TET_DIR)/figure.c -o $(BUILD_DIR)/figure.o

$(BUILD_DIR)/fsm.o: $(TET_DIR)/fsm.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(TET_DIR)/fsm.c -o $(BUILD_DIR)/fsm.o

$(BUILD_DIR)/utility.o: $(TET_DIR)/utility.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(TET_DIR)/utility.c -o $(BUILD_DIR)/utility.o

$(BUILD_DIR)/game_common.o: $(COMMON_DIR)/game_common.c | $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(COMMON_DIR)/game_common.c -o $(BUILD_DIR)/game_common.o

$(BUILD_DIR)/snake.o: | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $(SNAKE_DIR)/snake.cpp -o $(BUILD_DIR)/snake.o

$(BUILD_DIR)/Controller.o: | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $(SNAKE_DIR)/snake_controller.cpp -o $(BUILD_DIR)/Controller.o


$(BUILD_DIR)/snake_lib.a: $(BUILD_DIR)/snake.o $(BUILD_DIR)/Controller.o $(BUILD_DIR)/game_common.o
	ar rcs $(BUILD_DIR)/snake_lib.a $(BUILD_DIR)/*.o $(BUILD_DIR)/game_common.o
	rm -rf $(BUILD_DIR)/*.o
	ranlib $(BUILD_DIR)/snake_lib.a

uninstall:
	rm -rf $(BUILD_DIR)

dvi:
	doxygen
	open doxygen/html/index.html

dist:
	mkdir -p archive
	cp -r brick_game gui inc Makefile archive
	tar -cf brick_game2.tar -C archive .
	rm -rf archive

test:
	$(CXX) $(CFLAGS) $(TEST_FILES_SNAKE) $(TEST_LIBS) -o snake_test
	$(CC) $(FLAGS) $(TEST_FILES_TETRIS) $(TEST_LIBS_TET) -o tetris_test

	./snake_test
	./tetris_test

gcov_report:
	$(CXX) $(CFLAGS) --coverage $(TEST_FILES_SNAKE) $(TEST_LIBS) -o snake_test
	$(CC) $(FLAGS) --coverage $(TEST_FILES_TETRIS) $(TEST_LIBS_TET) -o tetris_test

	chmod +x ./snake_test ./tetris_test
	./snake_test
	./tetris_test

	lcov --capture --branch-coverage --directory . --output-file ./coverage.info --no-external --ignore-errors inconsistent,inconsistent
	lcov --list ./coverage.info
	genhtml ./coverage.info --output-directory ./coverage_report

	$(OPEN_CMD) ./coverage_report/index.html

clean:
	rm -rf coverage_info coverage_report *.dSYM gtest_test *.gcno *.gcda *.out coverage.info test leaks_log.txt doxygen .clang-format \
	tetris_test snake_test brick_game2.tar

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log_snake.txt ./snake_test
	@echo --- Valgrind summary --- && cat leaks_log_snake.txt | grep 'total heap usage' && cat leaks_log_snake.txt | grep 'ERROR SUMMARY'

	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=leaks_log_tetris.txt ./tetris_test
	@echo --- Valgrind summary --- && cat leaks_log_tetris.txt | grep 'total heap usage' && cat leaks_log_tetris.txt | grep 'ERROR SUMMARY'

desktop:
	./build/brick_game.app/Contents/MacOS/brick_game

console:
	./build/Console

style_correct:
	cp ../materials/linters/.clang-format .
	clang-format -style=Google -i */*.c */*.cpp
	clang-format -style=Google -i */*/*.c */*/*.cpp
	clang-format -style=Google -i */*.h
	clang-format -style=Google -i */*/*.h
	rm -rf .clang-format

style_check:
	cp ../materials/linters/.clang-format .
	clang-format -style=Google -n */*.c */*.cpp
	clang-format -style=Google -n */*/*.c */*/*.cpp
	clang-format -style=Google -n */*.h
	rm -rf .clang-format


