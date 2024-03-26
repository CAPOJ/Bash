SHELL := /bin/bash
CHECK = 2<&1 valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all
FILE = test.txt
PATTERNS = greppat.reg
all:
	make s21_grep tests
s21_grep:
	gcc -Wall -Werror -Wextra grep.c -o s21_grep
cpp:
	cppcheck --enable=all --Suppress=missingIncludeSystem main.c
rm:
	rm s21_grep
clang:
	clang-format -i *.c
tests:
	
	-diff <(./s21_grep It $(FILE)) <(grep It $(FILE))
	-diff <(./s21_grep -v It $(FILE)) <(grep -v It $(FILE))
	-diff <(./s21_grep -i It $(FILE)) <(grep -i It $(FILE))
	-diff <(./s21_grep -iv It $(FILE)) <(grep -iv It $(FILE))
	-diff <(./s21_grep -in It $(FILE)) <(grep -in It $(FILE))
	-diff <(./s21_grep -ie "^it" $(FILE)) <(grep -ie "^it" $(FILE))
	-diff <(./s21_grep -ince "^it" $(FILE)) <(grep -ince "^it" $(FILE))
	-diff <(./s21_grep -lh ve $(FILE)) <(grep -lh ve $(FILE))
	-diff <(./s21_grep -hc e $(FILE)) <(grep -hc e $(FILE))
	-diff <(./s21_grep -oc it $(FILE)) <(grep -oc it $(FILE))
	-diff <(./s21_grep -s it file.txt) <(grep -s it file.txt)

	-diff <(./s21_grep -e It -e it $(FILE)) <(grep -e It -e it $(FILE))
	-diff <(./s21_grep -s it $(FILE)) <(grep -s it $(FILE))
	-diff <(./s21_grep -f $(PATTERNS) $(FILE)) <(grep -f $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oi it $(FILE)) <(grep -oi it $(FILE))
	-diff <(./s21_grep -ohf $(PATTERNS) $(FILE)) <(grep -ohf $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oinc it $(FILE)) <(grep -oinc it $(FILE))
	-diff <(./s21_grep -oihlnc -e it -e ve -f ./$(PATTERNS) $(FILE)) <(grep -oihlnc -e it -e ve -f ./$(PATTERNS) $(FILE))

	$(CHECK) ./s21_grep it *.txt | grep -E ERROR
	$(CHECK) ./s21_grep -e It -e it $(FILE) | grep -E ERROR
	$(CHECK) ./s21_grep -f $(PATTERNS) $(FILE) | grep -E ERROR