#include "vm.cc"

int main() {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 38;
	constexpr int expecting = 39088169;

	char fib[] = {
		PUSH, fib_of, CALL1, FIB_FN, HALT,

		// DROP, PUSH, 0
		// FIB(n)
		DUP, PUSH, 0, IFNEQ, 1,  RET1, // if n == 0, return 0 --- as the return result is the same as n, we can remove "DROP, PUSH, 0" and do if n == 0, return n.
		DUP, PUSH, 1, IFNEQ, 1, RET1, // if n == 1, return 1

		DUP, PUSH, 1, SUB, // (n-1)
		CALL1, FIB_FN, // fib(n-1)

		ROT, PUSH, 2, SUB, // (n-2)
		CALL1, FIB_FN, // fib(n-2)
		ADD,
		RET1
	};	
	
    vm v1(fib);
	auto res = v1.run_specialsedtableless4();
	printf("Done! %d == %d\n", res, expecting);
    assert(res == expecting);
    return 0;
}