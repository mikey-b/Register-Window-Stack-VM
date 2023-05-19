FLAGS = -std=c++20 -Wall -Wpedantic -Werror -Wno-gnu-label-as-value -Ofast -mtune=native -march=native
CC = clang++.exe

.PHONY: all
all: switch jmptable reg4 reg8

switch:
	${CC} ${FLAGS} vm_switch.cc -o vm_switch

jmptable:
	${CC} ${FLAGS} vm_jmptable.cc -o vm_jmptable

reg4:
	${CC} ${FLAGS} vm_reg4.cc -o vm_reg4

reg8:
	${CC} ${FLAGS} vm_reg8.cc -o vm_reg8
	
benchmark:
	${CC} ${FLAGS} vm_benchmark.cc -o vm_benchmark -lbenchmark -lpthread