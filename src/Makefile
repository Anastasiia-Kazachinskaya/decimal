CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

DECIMAL_SOURCES = $(wildcard core/decimal/*.c core/big_decimal/*.c)
HEADERS = s21_decimal.h headers/s21_big_decimal.h headers/s21_utils.h
OBJECTS = $(DECIMAL_SOURCES:.c=.o)

LIB = s21_decimal.a

TEST_SRC = tests/test_s21_decimal_all.c tests/test_arithmetic.c tests/test_comparison.c \
           tests/test_converters.c tests/test_other.c tests/test_utils.c tests/test_complex.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXE = test.out
REPORT_DIR = report
REPORT = coverage_report.details.html

OS = $(shell uname -s)

ifeq ($(OS), Darwin)
    CHECK_CFLAGS = $(shell pkg-config --cflags check 2>/dev/null || echo "")
    CHECK_LIBS = $(shell pkg-config --libs check 2>/dev/null || echo "-lcheck -lpthread -lm")
    OPEN_CMD = open
else
    CHECK_CFLAGS = $(shell pkg-config --cflags check 2>/dev/null || echo "")
    CHECK_LIBS = $(shell pkg-config --libs check 2>/dev/null || echo "-lcheck -lpthread -lm -lrt -lsubunit")
    OPEN_CMD = xdg-open
endif

ifneq ($(CHECK_CFLAGS),)
    CFLAGS += $(CHECK_CFLAGS)
endif

all: $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJECTS) $(HEADERS)
	ar rcs $(LIB) $(OBJECTS)

test: $(TEST_OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $(TEST_EXE) $(TEST_OBJ) $(LIB) $(CHECK_LIBS)
	@./$(TEST_EXE)

gcov_report: $(DECIMAL_SOURCES) $(TEST_SRC)
	$(CC) $(CFLAGS) --coverage -o $(TEST_EXE) $(DECIMAL_SOURCES) $(TEST_SRC) $(CHECK_LIBS)
	./$(TEST_EXE)
	mkdir -p $(REPORT_DIR)
	gcovr --html-details $(REPORT_DIR)/$(REPORT) \
	rm -rf *.gcov *.gcno *.gcda

clean:
	rm -rf $(OBJECTS) $(LIB) $(TEST_OBJ) $(TEST_EXE) *.gcov *.gcno *.gcda
	rm -rf $(REPORT_DIR)

rebuild: clean all

clang_check:
	clang-format -n $(DECIMAL_SOURCES) $(HEADERS)

style:
	clang-format -i $(DECIMAL_SOURCES) $(HEADERS)

docker_test:
	@echo "Cleaning src/ before docker tests..."
	$(MAKE) clean
	@echo "Running docker tests from project root..."
	cd .. && docker run --rm -v .:/project checkimages/decimal

.PHONY: clean all rebuild test gcov_report clang_check style docker_test