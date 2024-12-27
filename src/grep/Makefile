CCNFLAGS=gcc -Werror -Wextra -Wall
FILESFORCOMPILE=s21_grep.c
FILESFORSTYLE=s21_grep.c s21_grep.h

all: s21_grep style test clean

s21_grep: $(FILESFORCOMPILE)
	$(CCNFLAGS) -o s21_grep $(FILESFORCOMPILE)

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n $(FILESFORSTYLE)

valgrind: s21_grep
	valgrind --tool=memcheck --leak-check=yes  ./s21_grep

clean:
	rm -rf s21_grep .clang-format

test: s21_grep
	bash run_tests.sh

rebuild: clean all
