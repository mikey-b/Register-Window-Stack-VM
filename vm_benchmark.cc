#include <benchmark/benchmark.h>
#include "vm.cc"

static void BM_switch(benchmark::State& state) {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 17;
	constexpr int expecting = 1597;

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
  
  int res;
  
  for (auto _ : state) {
    // This code gets timed
    res = v1.run_switch();
	v1.reset();
  }
  assert(res == expecting);
}

static void BM_jmptbl(benchmark::State& state) {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 17;
	constexpr int expecting = 1597;

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
  
  int res;
  
  for (auto _ : state) {
    // This code gets timed
    res = v1.run_table();
	v1.reset();
  }
  assert(res == expecting);
}

static void BM_reg4(benchmark::State& state) {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 17;
	constexpr int expecting = 1597;

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
  
  int res;
  
  for (auto _ : state) {
    // This code gets timed
    res = v1.run_specialsedtable4();
	v1.reset();
  }
  assert(res == expecting);
}

static void BM_reg4less(benchmark::State& state) {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 17;
	constexpr int expecting = 1597;

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
  
  int res;
  
  for (auto _ : state) {
    // This code gets timed
    res = v1.run_specialsedtableless4();
	v1.reset();
  }
  assert(res == expecting);
}

static void BM_reg8(benchmark::State& state) {
	constexpr int FIB_FN = 5; // FIB entry is 5th instruction in fib[]
	constexpr int fib_of = 17;
	constexpr int expecting = 1597;

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
  
  int res;
  
  for (auto _ : state) {
    // This code gets timed
    res = v1.run_specialsedtable8();
	v1.reset();
  }
  assert(res == expecting);
}

// Register the function as a benchmark
BENCHMARK(BM_switch);
BENCHMARK(BM_jmptbl);
BENCHMARK(BM_reg4);
BENCHMARK(BM_reg4less);
BENCHMARK(BM_reg8);
// Run the benchmark
BENCHMARK_MAIN();