FLAGS = -Wall -fno-reorder-blocks -Wno-gnu-label-as-value -Wno-unused-label -mtune=native -march=native
OPTS = -O1 -fcaller-saves -fcode-hoisting -fdevirtualize  -fdevirtualize-speculatively -fexpensive-optimizations -ffinite-loops -fgcse  -fgcse-lm -fhoist-adjacent-loads -finline-functions -finline-small-functions -findirect-inlining -fipa-bit-cp  -fipa-cp  -fipa-icf -fipa-ra  -fipa-sra  -fipa-vrp -fisolate-erroneous-paths-dereference -flra-remat -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -freorder-functions -frerun-cse-after-loop -fschedule-insns  -fschedule-insns2 -fsched-interblock  -fsched-spec -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-builtin-call-dce -ftree-loop-vectorize -ftree-pre -ftree-slp-vectorize -ftree-switch-conversion  -ftree-tail-merge -ftree-vrp -fgcse-after-reload -fipa-cp-clone -floop-interchange -floop-unroll-and-jam -fpeel-loops -fpredictive-commoning -fsplit-loops -fsplit-paths -ftree-loop-distribution -ftree-partial-pre -funswitch-loops -fvect-cost-model=dynamic -fversion-loops-for-strides
CC = g++.exe

.PHONY: all
all: switch jmptable reg4 reg8

switch:
	${CC} ${FLAGS} vm_switch.cc -o vm_switch

jmptable:
	${CC} ${FLAGS} vm_jmptable.cc -o vm_jmptable

reg4:
	${CC} ${FLAGS} -O2 vm_reg4.cc -o vm_reg4

reg4less:
	${CC} ${FLAGS} ${OPTS} -falign-labels=32 vm_regtableless4.cc -Wno-uninitialized -o vm_reg4less

reg8:
	${CC} ${FLAGS} vm_reg8.cc -o vm_reg8
	
benchmark:
	${CC} ${FLAGS} vm_benchmark.cc -o vm_benchmark -lbenchmark -lpthread