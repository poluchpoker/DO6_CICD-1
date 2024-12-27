CCNFLAGS=gcc -Wall -Wextra -Werror
FILESFORCOMPILE=s21_cat.c
FILESFORSTYLE=s21_cat.c s21_cat.h

all: s21_cat style test leaks clean

s21_cat: $(FILESFORCOMPILE)
	$(CCNFLAGS) -o s21_cat $(FILESFORCOMPILE)

style:
	cp ../../materials/linters/.clang-format ./ 
	clang-format -n $(FILESFORSTYLE)

leaks: s21_cat
	leaks -atExit -- ./s21_cat

clean:
	rm -rf s21_cat .clang-format

test: s21_cat
	bash run_tests.sh

rebuild: clean all
