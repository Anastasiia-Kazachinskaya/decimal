CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

SOURCES = $(wildcard core/*.c)
HEADERS = headers/s21_decimal.h headers/s21_helpers.h
OBJECTS = $(SOURCES:.c=.o)
LIB = s21_decimal.a

TEST_SRC = tests/test_arithmetic.c tests/test_comparison.c tests/test_converters.c tests/test_other.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXE = test.out
REPORT_DIR = report
REPORT = coverage_report.details.html

all: $(LIB) gcov_report

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

$(LIB): $(OBJECTS) $(HEADERS)
	ar rcs $(LIB) $(OBJECTS)

test: $(TEST_OBJ)	$(LIB) 
	$(CC) $(CFLAGS) -o $(TEST_EXE) $(TEST_OBJ) -lcheck -L./ -l:$(LIB)
	@./$(TEST_EXE)

gcov_report: $(SOURCES) $(TEST_SRC)
	$(CC) $(CFLAGS) --coverage -o $(TEST_EXE) $(SOURCES) $(TEST_SRC) -lcheck
	./$(TEST_EXE)
	@mkdir $(REPORT_DIR)
	@gcovr --html-details $(REPORT_DIR)/$(REPORT)
	@rm -rf *.gcov *.gcno *.gcda

clean:	
	rm -rf $(OBJECTS) $(LIB) $(TEST_OBJ) $(TEST_EXE) *.gcov *.gcno *.gcda
	rm -rf $(REPORT_DIR)

rebuild: clean all

clang_check:
	clang-format -n core/*.c *.h 

style:
	clang-format -i core/*.c *.h 


.PHONY: clean all rebuild test gcov_report
