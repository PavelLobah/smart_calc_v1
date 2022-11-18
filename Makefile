DATE = $(shell date -R)
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address -lm
VFLAGS = -Wall -Werror -Wextra -lm
UNAME = $(shell uname)
FILE = smart_calc
STACK = ./stack.c
GUI = ./gui.c
SOURCE = $(STACK) $(GUI)
HTML_CREATE=lcov -t a.out -o rep.info -c -d . --no-external; genhtml -o report rep.info
OS=$(shell uname)
ifeq ($(OS), Linux)
	CTEST = -lcheck -lpthread -lrt -lm -lsubunit --coverage
	# COVERAGE = xdg-open report/index.html
	CHECK_FLAGS=-lcheck -pthread -lrt -lm -lsubunit
else
    CHECK_FLAGS=-lcheck
	CTEST = -lcheck --coverage
	COVERAGE = open report/index.html
endif
FILE = s21_smart_calc

all: clean build

rebuild: clean all

build: clean	
ifeq ($(UNAME), Linux)
	$(CC) -export-dynamic `pkg-config --cflags gtk+-3.0` -o $(FILE).o $(FILE).c $(SOURCE) `pkg-config --libs gtk+-3.0` -lm
else
	$(CC) -o $(FILE).o  $(FILE).c $(SOURCE) `pkg-config --cflags --libs gtk+-3.0`
endif

rebuild: clean all

run:
	./$(FILE).o

dvi:
	open ./text/manual.pdf

dist:
	mkdir ./dist
	cp -r ./install/* dist/
	tar cvzf smartcalc_dist.tgz dist/
	rm -rf dist/

#	распаковка 
#	tar -xvf smartcalc_dist.tgz

install:
	mkdir ./install
	cp ./calculator.glade ./install
	cp ./text/manual.pdf ./install
	install ./$(FILE).o ./install/
	

uninstall:
	rm -rf ./install

test: clean
	# checkmk ./tests/test.check > ./tests/$(FILE)_test.c
	$(CC) $(FILE).c ./tests/$(FILE)_test.c stack.c $(CTEST)
	./a.out

gcov_report: test
	./a.out
	gcovr --html --html-details -o reports.html 
	open reports.html

calc: 
ifeq ($(UNAME), Linux)
	$(CC) -export-dynamic `pkg-config --cflags gtk+-3.0` -o main.o smart_calc.c stack.c gui.c `pkg-config --libs gtk+-3.0` -lm
	./main.o
else
	$(CC) -o $(FILE).o  smart_calc.c stack.c gui.c `pkg-config --cflags --libs gtk+-3.0`
	./$(FILE).o
endif

clang:
	cp ../materials/linters/.clang-format ./
	clang-format -i ./s21_smart_calc.*
	clang-format -n ./s21_smart_calc.*
	clang-format -i ./tests/*
	clang-format -n ./tests/*
	clang-format -i ./stack.*
	clang-format -n ./stack.*
	clang-format -i ./gui.*
	clang-format -i ./gui.*
	rm -rf .clang-format
	
valg: clean
	$(CC) -g $(VFLAGS) $(FILE).c stack.c ./tests/$(FILE)_test.c $(CTEST) `pkg-config --libs gtk+-3.0` -L.
	valgrind --track-origins=yes -q ./a.out

clean:
	rm -f *.html *.o *.a *.gcno *.gcda *.info *.out *.css *.tgz
	rm -rf ./report
	rm -f $(FILE)
	rm -rf ./dist
	rm -rf *.dSYM
	clear

# cpplint:
# 	python3 ../materials/linters/cpplint.py --extensions=c $(FILE).c $(FILE)_test.c
# 	python3 ../materials/linters/cpplint.py --extensions=c $(FILE).h

git:
	git add .
	git commit -m "$(DATE)"
	git push origin develop

leaks: 
	$(CC) -g ./tests/$(FILE)_test.c $(FILE).c stack.c $(CTEST) -L. -lcheck --coverage
#	leaks --atExit -- ./a.out
	CK_FORK=no leaks --atExit -- ./a.out
