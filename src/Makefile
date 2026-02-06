CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
CHECK_FLAGS = $(shell pkg-config --cflags --libs check)
SOURCES = $(wildcard core/*.c)
HEADERS = headers/s21_decimal.h headers/s21_helpers.h
OBJECTS = $(SOURCES:.c=.o)
LIB = s21_decimal.a

TEST_SRC = tests/test_s21_decimal_all.c tests/test_arithmetic.c tests/test_comparison.c tests/test_converters.c tests/test_other.c tests/test_helpers.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXE = test.out
REPORT_DIR = report
REPORT = coverage_report.details.html


OS = $(shell uname -s)

ifeq ($(OS), Darwin)
	CHECK_FLAGS = $(shell pkg-config --cflags --libs check)
	OPEN_CMD = open
else
	CHECK_FLAGS = -lcheck -lpthread -lm -lrt -lsubunit
	OPEN_CMD = xdg-open
endif


all: $(LIB) gcov_report

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

$(LIB): $(OBJECTS) $(HEADERS)
	ar rcs $(LIB) $(OBJECTS)

test: $(TEST_OBJ)	$(LIB) 
	$(CC) $(CFLAGS) $(CHECK_FLAGS) -o $(TEST_EXE) $(TEST_OBJ) $(LIB)
	@./$(TEST_EXE)

gcov_report: $(SOURCES) $(TEST_SRC)
	$(CC) $(CFLAGS) $(CHECK_FLAGS) --coverage -o $(TEST_EXE) $(SOURCES) $(TEST_SRC)
	./$(TEST_EXE)
	@mkdir $(REPORT_DIR)
	@gcovr --html-details $(REPORT_DIR)/$(REPORT)
	@rm -rf *.gcov *.gcno *.gcda

clean:	
	rm -rf $(OBJECTS) $(LIB) $(TEST_OBJ) $(TEST_EXE) *.gcov *.gcno *.gcda
	rm -rf $(REPORT_DIR)

rebuild: clean all

.PHONY: clean all rebuild test gcov_report
