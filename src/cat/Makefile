SHELL := /bin/bash
FILE = test.txt
CHECK=2>&1 valgrind --tool=memcheck --leak-check=yes
all:
	make s21_cat tests
s21_cat:
	gcc -Wall -Werror -Wextra cat.c -o s21_cat
cpp:
	cppcheck --enable=all --Suppress=missingIncludeSystem cat.c
rm:
	rm s21_cat
clang:
	clang-format -i *.c
tests:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -E $(FILE)) <(cat -E $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat -T $(FILE)) <(cat -T $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat --number $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat --show-ends $(FILE)) <(cat -E $(FILE))

	$(CHECK) ./s21_cat $(FILE) | grep ERROR
	$(CHECK) ./s21_cat -b $(FILE) | grep ERROR
	$(CHECK) ./s21_cat -n $(FILE) | grep ERROR
	$(CHECK) ./s21_cat -e $(FILE) | grep ERROR
	$(CHECK) ./s21_cat -t $(FILE) | grep ERROR
	$(CHECK) ./s21_cat -s $(FILE) | grep ERROR

