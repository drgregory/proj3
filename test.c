#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <x86intrin.h>

#define NUM_REGISTERS 4;

int main(void) {
    float test[4] = { 1.0, 2.0, 3.0, 4.0 };
    f(test);
    return 0;
}

void f(float *test) {
    int mask = 0;
    float result;
    __m128 a, zero;
    a = _mm_loadu_ps(test);
    zero = _mm_setzero_ps();
    a = _mm_hadd_ps(a, zero);
    a = _mm_hadd_ps(a, zero);
    //_mm_storeu_ps(test, a);
    /*for (int i = 0; i < 4; i += 1) {
	printf("%f\n", test[i]);
	}*/
    _MM_EXTRACT_FLOAT(result, a, mask);
    printf("%f\n", result);
}
