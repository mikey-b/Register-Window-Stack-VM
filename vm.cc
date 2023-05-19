// https://godbolt.org/z/696q46WEh

#include <cassert>

enum ins: char {
    PUSH,
    ADD,
    SUB,
    ROT,
    DUP,
    DROP,

    CALL1,
    IFNEQ,
    RET1,
    HALT,
};

struct vm {
    int datastack[1024];
    int callstack[1024];
    int *sp;
    char *pc;
    int *fp;

    char *code;
	
	void reset() {
		sp = &datastack[0];
		pc = code;
		fp = &callstack[0];
	}

    vm(char *pcode): sp(&datastack[0]), pc(pcode), fp(&callstack[0]), code(pcode) { }

    int run_specialsedtable4() {

        int r0, r1, r2, r3;
                    
// printf("[ "); for(auto i = &datastack[0]; i < sp; ++i) { printf("%d,", *i); }; printf("] - ");
#define NEXT(N) goto *jmp##N[(unsigned char)*pc++]

// FLUSH (stack depth)_(keeping in reg)
/*
#define FLUSH0_0 {};

#define FLUSH1_0 *sp++ = r0;
#define FLUSH1_1 {};

#define FLUSH2_0 *sp++ = r0; *sp++ = r1;
#define FLUSH2_1 *sp++ = r0; r0 = r1;

#define FLUSH3_0 *sp++ = r0; *sp++ = r1; *sp++ = r2;
#define FLUSH3_1 *sp++ = r0; r0 = r2; *sp++ = r1;

#define FLUSH7_1 *sp++ = r0; r0 = r6; *sp++ = r1; *sp++ = r2; *sp++ = r3; *sp++ = r4; *sp++ = r5;
#define FLUSH7_3 *sp++ = r0; r0 = r4; *sp++ = r1; r1 = r5; *sp++ = r2; r2 = r6; *sp++ = r3;
*/
#define FLUSH4_0 *sp++ = r0; *sp++ = r1; *sp++ = r2; *sp++ = r3;
#define FLUSH4_1 *sp++ = r0; r0 = r3; *sp++ = r1; *sp++ = r2;
#define FLUSH4_2 *sp++ = r0; r0 = r3; *sp++ = r1; r1 = r2;



		/*   PUSH,     ADD,     SUB,     ROT,     DUP,     DROP,     CALL1,     IFNEQ,     RET1,     HALT, */
        static const void* jmp0[] = {&&PUSH0, &&ADD0, &&SUB0, &&ROT0, &&DUP0, &&DROP0, &&CALL10, &&IFNEQ0, &&RET10, &&HALT0};
        static const void* jmp1[] = {&&PUSH1, &&ADD1, &&SUB1, &&ROT1, &&DUP1, &&DROP1, &&CALL11, &&IFNEQ1, &&RET11, &&HALT1};
        static const void* jmp2[] = {&&PUSH2, &&ADD2, &&SUB2, &&ROT2, &&DUP2, &&DROP2, &&CALL12, &&IFNEQ2, &&RET12, &&HALT2};
        static const void* jmp3[] = {&&PUSH3, &&ADD3, &&SUB3, &&ROT3, &&DUP3, &&DROP3, &&CALL13, &&IFNEQ3, &&RET13, &&HALT3};
		static const void* jmp4[] = {&&PUSH4, &&ADD4, &&SUB4, &&ROT4, &&DUP4, &&DROP4, &&CALL14, &&IFNEQ4, &&RET14, &&HALT4};

        NEXT(0);

        SUB0: { auto r = *(sp - 2) - *(sp - 1); sp -= 2; r0 = r; NEXT(1); }
        SUB1: { auto r = *(sp - 1) - r0;        sp -= 1; r0 = r; NEXT(1); }
        SUB2: { r0 = r0 - r1; NEXT(1); }
        SUB3: { r1 = r1 - r2; NEXT(2); }
		SUB4: { r2 = r2 - r3; NEXT(3); }

        ADD0: { auto r = *(sp - 2) + *(sp - 1); sp -= 2; r0 = r; NEXT(1); }
        ADD1: { auto r = *(sp - 1) + r0;        sp -= 1; r0 = r; NEXT(1); }
        ADD2: { r0 = r0 + r1; NEXT(1); }
        ADD3: { r1 = r1 + r2; NEXT(2); }		
		ADD4: { r2 = r2 + r3; NEXT(3); }

        DROP0: { sp -= 1; NEXT(0); }
        DROP1: {          NEXT(0); }
        DROP2: {          NEXT(1); }
        DROP3: {          NEXT(2); }		
		DROP4: {          NEXT(3); }

        PUSH0: { r0 = *pc++; NEXT(1); }
        PUSH1: { r1 = *pc++; NEXT(2); }
        PUSH2: { r2 = *pc++; NEXT(3); }
		PUSH3: { r3 = *pc++; NEXT(4); }
        PUSH4: { FLUSH4_0; r0 = *pc++; NEXT(1); }
		
        DUP0:  { r0 = *(sp - 1); NEXT(1); }
        DUP1:  { r1 = r0;        NEXT(2); }
        DUP2:  { r2 = r1;        NEXT(3); }
		DUP3:  { r3 = r2;        NEXT(4); }
        DUP4:  { FLUSH4_1; *sp++ = r0; NEXT(1); } // Special Case: lets just flush 3, keeping the top in a reg, then push a copy onto the stack.
		
        ROT0: { auto t = *(sp - 2); *(sp - 2) = *(sp - 1); *(sp - 1) = t; NEXT(0); }
        ROT1: { auto t = *(sp - 1); *(sp - 1) = r0; r0 = t;               NEXT(1); }
        ROT2: { auto t = r0; r0 = r1; r1 = t; NEXT(2); }
        ROT3: { auto t = r1; r1 = r2; r2 = t; NEXT(3); }
        ROT4: { auto t = r2; r2 = r3; r3 = t; NEXT(4); }

        IFNEQ0: { auto jmpt = *pc++; auto r = *(sp - 1) != *(sp - 2);  sp -= 2; if (r) pc += jmpt; NEXT(0); }
        IFNEQ1: { auto jmpt = *pc++; auto r = r0 != *(sp - 1);         sp -= 1; if (r) pc += jmpt; NEXT(0); }
        IFNEQ2: { auto jmpt = *pc++; auto r = r1 != r0;					        if (r) pc += jmpt; NEXT(0); }
        IFNEQ3: { auto jmpt = *pc++; auto r = r2 != r1;					        if (r) pc += jmpt; NEXT(1); }
        IFNEQ4: { auto jmpt = *pc++; auto r = r3 != r2;				            if (r) pc += jmpt; NEXT(2); }

        CALL10: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(0); }
        CALL11: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(1); }
        CALL12: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(2); }
        CALL13: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(3); }
        CALL14: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(4); }

        RET10:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(0); }
        RET11:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(1); }
        RET12:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(2); }
        RET13:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(3); }
        RET14:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(4); }
		
        HALT0: { /* printf("TOP = %d\n", *(sp - 1)); */	return *(sp - 1); }
        HALT1: { /* printf("TOP = %d\n", r0); */         return r0;	      }
        HALT2: { /* printf("TOP = %d\n", r1); */         return r1;        }
        HALT3: { /* printf("TOP = %d\n", r2); */         return r2;        }
        HALT4: { /* printf("TOP = %d\n", r3); */         return r3;        }	
		
        return 0;
    }


    int run_specialsedtable8() {

        int r0, r1, r2, r3, r4, r5, r6, r7;
                    
// printf("[ "); for(auto i = &datastack[0]; i < sp; ++i) { printf("%d,", *i); }; printf("] - ");
#define NEXT(N) goto *jmp##N[(unsigned char)*pc++]

// FLUSH (stack depth)_(keeping in reg)
/*
#define FLUSH0_0 {};

#define FLUSH1_0 *sp++ = r0;
#define FLUSH1_1 {};

#define FLUSH2_0 *sp++ = r0; *sp++ = r1;
#define FLUSH2_1 *sp++ = r0; r0 = r1;

#define FLUSH3_0 *sp++ = r0; *sp++ = r1; *sp++ = r2;
#define FLUSH3_1 *sp++ = r0; r0 = r2; *sp++ = r1;

#define FLUSH4_0 *sp++ = r0; *sp++ = r1; *sp++ = r2; *sp++ = r3;
#define FLUSH4_1 *sp++ = r0; r0 = r3; *sp++ = r1; *sp++ = r2;
#define FLUSH4_2 *sp++ = r0; r0 = r3; *sp++ = r1; r1 = r2;

#define FLUSH7_1 *sp++ = r0; r0 = r6; *sp++ = r1; *sp++ = r2; *sp++ = r3; *sp++ = r4; *sp++ = r5;
#define FLUSH7_3 *sp++ = r0; r0 = r4; *sp++ = r1; r1 = r5; *sp++ = r2; r2 = r6; *sp++ = r3;
*/
#define FLUSH8_3 *sp++ = r0; *sp++ = r1; *sp++ = r2; *sp++ = r3; *sp++ = r4; r0 = r5; r1 = r6; r2 = r7;

		/*   PUSH,     ADD,     SUB,     ROT,     DUP,     DROP,     CALL1,     IFNEQ,     RET1,     HALT, */
        static const void* jmp0[] = {&&PUSH0, &&ADD0, &&SUB0, &&ROT0, &&DUP0, &&DROP0, &&CALL10, &&IFNEQ0, &&RET10, &&HALT0};
        static const void* jmp1[] = {&&PUSH1, &&ADD1, &&SUB1, &&ROT1, &&DUP1, &&DROP1, &&CALL11, &&IFNEQ1, &&RET11, &&HALT1};
        static const void* jmp2[] = {&&PUSH2, &&ADD2, &&SUB2, &&ROT2, &&DUP2, &&DROP2, &&CALL12, &&IFNEQ2, &&RET12, &&HALT2};
        static const void* jmp3[] = {&&PUSH3, &&ADD3, &&SUB3, &&ROT3, &&DUP3, &&DROP3, &&CALL13, &&IFNEQ3, &&RET13, &&HALT3};
        static const void* jmp4[] = {&&PUSH4, &&ADD4, &&SUB4, &&ROT4, &&DUP4, &&DROP4, &&CALL14, &&IFNEQ4, &&RET14, &&HALT4};
        static const void* jmp5[] = {&&PUSH5, &&ADD5, &&SUB5, &&ROT5, &&DUP5, &&DROP5, &&CALL15, &&IFNEQ5, &&RET15, &&HALT5};
        static const void* jmp6[] = {&&PUSH6, &&ADD6, &&SUB6, &&ROT6, &&DUP6, &&DROP6, &&CALL16, &&IFNEQ6, &&RET16, &&HALT6};
        static const void* jmp7[] = {&&PUSH7, &&ADD7, &&SUB7, &&ROT7, &&DUP7, &&DROP7, &&CALL17, &&IFNEQ7, &&RET17, &&HALT7};
		static const void* jmp8[] = {&&PUSH8, &&ADD8, &&SUB8, &&ROT8, &&DUP8, &&DROP8, &&CALL18, &&IFNEQ8, &&RET18, &&HALT8};

        NEXT(0);

        SUB0: { auto r = *(sp - 2) - *(sp - 1); sp -= 2; r0 = r; NEXT(1); }
        SUB1: { auto r = *(sp - 1) - r0;        sp -= 1; r0 = r; NEXT(1); }
        SUB2: { r0 = r0 - r1; NEXT(1); }
        SUB3: { r1 = r1 - r2; NEXT(2); }
        SUB4: { r2 = r2 - r3; NEXT(3); }
        SUB5: { r3 = r3 - r4; NEXT(4); }
        SUB6: { r4 = r4 - r5; NEXT(5); }
        SUB7: { r5 = r5 - r6; NEXT(6); }
		SUB8: { r6 = r6 - r7; NEXT(7); }

        ADD0: { auto r = *(sp - 2) + *(sp - 1); sp -= 2; r0 = r; NEXT(1); }
        ADD1: { auto r = *(sp - 1) + r0;        sp -= 1; r0 = r; NEXT(1); }
        ADD2: { r0 = r0 + r1; NEXT(1); }
        ADD3: { r1 = r1 + r2; NEXT(2); }
        ADD4: { r2 = r2 + r3; NEXT(3); }
        ADD5: { r3 = r3 + r4; NEXT(4); }
        ADD6: { r4 = r4 + r5; NEXT(5); }
        ADD7: { r5 = r5 + r6; NEXT(6); }		
		ADD8: { r6 = r6 + r7; NEXT(7); }

        DROP0: { sp -= 1; NEXT(0); }
        DROP1: {          NEXT(0); }
        DROP2: {          NEXT(1); }
        DROP3: {          NEXT(2); }
        DROP4: {          NEXT(3); }
        DROP5: {          NEXT(4); }
        DROP6: {          NEXT(5); }
        DROP7: {          NEXT(6); }
		DROP8: {          NEXT(7); }

        PUSH0: { r0 = *pc++; NEXT(1); }
        PUSH1: { r1 = *pc++; NEXT(2); }
        PUSH2: { r2 = *pc++; NEXT(3); }
        PUSH3: { r3 = *pc++; NEXT(4); }
        PUSH4: { r4 = *pc++; NEXT(5); }
        PUSH5: { r5 = *pc++; NEXT(6); }
        PUSH6: { r6 = *pc++; NEXT(7); }
		PUSH7: { r7 = *pc++; NEXT(8); }
        PUSH8: { FLUSH8_3; r3 = *pc++; NEXT(4); }
		
        DUP0:  { r0 = *(sp - 1); NEXT(1); }
        DUP1:  { r1 = r0;        NEXT(2); }
        DUP2:  { r2 = r1;        NEXT(3); }
        DUP3:  { r3 = r2;        NEXT(4); }
        DUP4:  { r4 = r3;        NEXT(5); }
        DUP5:  { r5 = r4;        NEXT(6); }
        DUP6:  { r6 = r5;        NEXT(7); }
		DUP7:  { r7 = r6;        NEXT(8); }
        DUP8:  { FLUSH8_3; *sp++ = r2; NEXT(4); } // Special Case: lets just flush 3, keeping the top in a reg, then push a copy onto the stack.
		
        ROT0: { auto t = *(sp - 2); *(sp - 2) = *(sp - 1); *(sp - 1) = t; NEXT(0); }
        ROT1: { auto t = *(sp - 1); *(sp - 1) = r0; r0 = t;               NEXT(1); }
        ROT2: { auto t = r0; r0 = r1; r1 = t; NEXT(2); }
        ROT3: { auto t = r1; r1 = r2; r2 = t; NEXT(3); }
        ROT4: { auto t = r2; r2 = r3; r3 = t; NEXT(4); }
		ROT5: { auto t = r3; r3 = r4; r4 = t; NEXT(5); }
        ROT6: { auto t = r4; r4 = r5; r5 = t; NEXT(6); }
        ROT7: { auto t = r5; r5 = r6; r6 = t; NEXT(7); }
		ROT8: { auto t = r6; r6 = r7; r7 = t; NEXT(8); }

        IFNEQ0: { auto jmpt = *pc++; auto r = *(sp - 1) != *(sp - 2);  sp -= 2; if (r) pc += jmpt; NEXT(0); }
        IFNEQ1: { auto jmpt = *pc++; auto r = r0 != *(sp - 1);         sp -= 1; if (r) pc += jmpt; NEXT(0); }
        IFNEQ2: { auto jmpt = *pc++; auto r = r1 != r0;					        if (r) pc += jmpt; NEXT(0); }
        IFNEQ3: { auto jmpt = *pc++; auto r = r2 != r1;					        if (r) pc += jmpt; NEXT(1); }
        IFNEQ4: { auto jmpt = *pc++; auto r = r3 != r2;				            if (r) pc += jmpt; NEXT(2); }
        IFNEQ5: { auto jmpt = *pc++; auto r = r4 != r3;					        if (r) pc += jmpt; NEXT(3); }
        IFNEQ6: { auto jmpt = *pc++; auto r = r5 != r4;					        if (r) pc += jmpt; NEXT(4); }
        IFNEQ7: { auto jmpt = *pc++; auto r = r6 != r5;				            if (r) pc += jmpt; NEXT(5); }
		IFNEQ8: { auto jmpt = *pc++; auto r = r7 != r6;				            if (r) pc += jmpt; NEXT(6); }

        HALT0: { /* printf("TOP = %d\n", *(sp - 1)); */	return *(sp - 1); }
        HALT1: { /* printf("TOP = %d\n", r0); */         return r0;	      }
        HALT2: { /* printf("TOP = %d\n", r1); */         return r1;        }
        HALT3: { /* printf("TOP = %d\n", r2); */         return r2;        }
        HALT4: { /* printf("TOP = %d\n", r3); */         return r3;        }
        HALT5: { /* printf("TOP = %d\n", r4); */         return r4;        }
        HALT6: { /* printf("TOP = %d\n", r5); */         return r5;        }
        HALT7: { /* printf("TOP = %d\n", r6); */         return r6;        }
		HALT8: { /* printf("TOP = %d\n", r7); */         return r7;        }

        CALL10: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(0); }
        CALL11: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(1); }
        CALL12: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(2); }
        CALL13: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(3); }
        CALL14: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(4); }
        CALL15: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(5); }
        CALL16: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(6); }
        CALL17: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(7); }
		CALL18: { *fp++ = ((pc+1) - code); pc = code + *pc; NEXT(8); }

        RET10:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(0); }
        RET11:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(1); }
        RET12:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(2); }
        RET13:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(3); }
        RET14:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(4); }
        RET15:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(5); }
        RET16:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(6); }
        RET17:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(7); }
		RET18:  { auto r = *(fp - 1); fp -= 1; pc = code + r; NEXT(8); }
		
        return 0;
    }
	
    int run_table() {
#define NEXT_ goto *jmp_[(unsigned char)*pc++];

        static const void* jmp_[] = {&&PUSH, &&ADD, &&SUB, &&ROT, &&DUP, &&DROP, &&CALL1, &&IFNEQ, &&RET1, &&HALT};

        NEXT_;

        SUB: {
			auto r = *(sp - 2) - *(sp - 1);
			sp -= 2;
			*sp++ = r;
            NEXT_;
        }


        ADD: {
			auto r = *(sp - 1) + *(sp - 2);
			sp -= 2;
			*sp++ = r;
            NEXT_;
        }

        DROP: {
            sp -= 1;
            NEXT_;
        }

        PUSH: {
            //printf("pushing %d\n", *pc);
            *sp++ = *pc++;
            NEXT_;
        }
		
        DUP: {
			*sp = *(sp-1);
			sp++;
            NEXT_;
        }
		
        ROT: {
            auto t = *(sp - 2);
            *(sp - 2) = *(sp - 1);
            *(sp - 1) = t;
            NEXT_;            
        }

        IFNEQ: {
            auto jmpt = *pc++;
            auto r = *(sp - 1) != *(sp - 2);
            sp -= 2;
            if (r) pc += jmpt;  
            NEXT_;
        }

        HALT: {
            //printf("TOP = %d\n", *(sp - 1));
            return *(sp - 1);
        }

        CALL1:
        {
            *fp++ = ((pc+1) - code);
            pc = code + *pc;
            NEXT_;
        }

        RET1:
        {
            auto r = *(fp - 1);
            fp -= 1;
            pc = code + r;
            NEXT_;
        }

    }
	

    int run_switch() {
        for(;;) {
            /*
                    printf("[ ");
                    for(auto i = &datastack[0]; i < sp; ++i) {
                        printf("%d,", *i);
                    }
                    printf("] - ");
            */
            switch(*pc++) {
                case SUB: {
                    //printf("sub %d - %d\n", *(sp - 2), *(sp - 1));
                    auto r = *(sp - 2) - *(sp - 1);
                    sp -= 2;
                    *sp++ = r;
                    break;
                }
                case ADD: {
                    //printf("add %d + %d\n", *(sp - 1), *(sp - 2));
                    auto r = *(sp - 1) + *(sp - 2);
                    sp -= 2;
                    *sp++ = r;
                    break;
                }

                case DROP: {
                    //printf("drop\n");
                    sp -= 1;
                    break;
                }

                case PUSH: {
                    *sp++ = *pc++;
                    break;
                }
                case CALL1: {
                    //printf("calling %d\n", *pc);
                    *fp++ = ((pc+1) - code);
                    pc = code + *pc;
                    break;
                }
                case RET1: {
                    //printf("ret\n");
                    auto r = *(fp - 1);
                    fp -= 1;
                    pc = code + r;
                    break;
                }

                case DUP: {
                    //printf("DUP\n");
                    *sp = *(sp-1);
					sp++;
                    break;
                }

                case ROT: {
                    //printf("ROT\n");
                    auto t = *(sp - 2);
                    *(sp - 2) = *(sp - 1);
                    *(sp - 1) = t;
                    break;
                }

                case IFNEQ: {
                    //printf("IFNEQ\n");
                    auto jmp = *pc++;
                    auto r = *(sp - 1) != *(sp - 2);
                    sp -= 2;
                    if (r) pc += jmp;
                    break;
                }

                case HALT: {
					//printf("TOP = %d\n", *(sp - 1));
                    return *(sp - 1);
                }
            }
        }
    }
};