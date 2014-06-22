CSOURCES:=$(shell find . -name "*.c")
COBJECTS:=$(patsubst %.c, %.o, $(CSOURCES))
CPPSOURCES:=$(shell find . -name "*.cpp")
CPPOBJECTS:=$(patsubst %.cpp, %.o, $(CPPSOURCES))


all: build run

build: clean $(COBJECTS) $(CPPOBJECTS) link

WFLAGSON=-Wall -Wextra -Werror=return-type -Wshadow -Wundef -Wframe-larger-than=16384 -Wdeprecated \
-Wredundant-decls
WFLAGSOFF=-Wno-sequence-point -Wno-unused-parameter -Wno-sign-compare -Wno-pointer-arith
FFLAGS=-fno-omit-frame-pointer
CFLAGS=-O3 -std=c11 $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS)
CPPFLAGS=-O0 -std=c++11 $(WFLAGSON) $(WFLAGSOFF) $(FFLAGS)

clean:
	@echo Cleaning workspace
	@find . -name '*.o' -delete
	@rm -f tcpTest

link:
	@echo Linking
	@g++ -o tcpTest $(COBJECTS) $(CPPOBJECTS)

.c.o:
	@echo Compiling $<
	@gcc $(CFLAGS) -o $@ -c $<

.cpp.o:
	@echo Compiling $<
	@g++ $(CPPFLAGS) -o $@ -c $<

run:
	./tcpTest