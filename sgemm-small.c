#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <x86intrin.h>

#define NUM_REGISTERS 4;

void square_sgemm( int n, float *A, float *B, float *C ) {
	int i, j , k, l;
	//int count = 0; //for debug
	float At[n*n] __attribute__ ((aligned(16)));
	float temp;
	__m128 x;
	__m128 y;
	__m128 a;
	__m128 b;
	__m128 c;
	__m128 d;
	__m128 partialSum;
	__m128 partialSum1;
	__m128 partialSum2;
	__m128 partialSum3;
	__m128 partialSum4;
	__m128 partialSum5;
	__m128 partialSum6;
	__m128 partialSum7;
	float cij=0.0, cij1=0.0, cij2=0.0, cij3=0.0, cij4=0.0, cij5=0.0, cij6=0.0, cij7=0.0;
	__m128 c1;
	__m128 c2;
  //transpose A
  for (i = 0; i < n; i ++) {
		for (j = 0; j < n/4*4; j += 4) {
			
			x = _mm_loadu_ps(A + j + i*n);
			_MM_EXTRACT_FLOAT(temp, x, 0);
			At[i+j*n] = temp;
			_MM_EXTRACT_FLOAT(temp, x, 1);
			At[i+(j+1)*n] = temp;
			_MM_EXTRACT_FLOAT(temp, x, 2);
			At[i+(j+2)*n] = temp;
			_MM_EXTRACT_FLOAT(temp, x, 3);
			At[i+(j+3)*n] = temp;
			
		}
		for (; j<n; j ++) {
		    At[i+j*n] = A[j+i*n];
		}
  }

  // For each row i of A 
  for (i = 0; i < n/4*4; i+=4) {
    // For each column j of B 
    for (j = 0; j < n/2*2; j+=2)
    {
	//load C inital values
  c1 = _mm_loadu_ps(C + i + j*n);
	c2 = _mm_loadu_ps(C + i + (j+1)*n);

	//this will hold 4 floats which sum to the dot product
	partialSum = _mm_setzero_ps();
	partialSum1 = _mm_setzero_ps();
	partialSum2 = _mm_setzero_ps();
	partialSum3 = _mm_setzero_ps();
	partialSum4 = _mm_setzero_ps();
	partialSum5 = _mm_setzero_ps();
	partialSum6 = _mm_setzero_ps();
	partialSum7 = _mm_setzero_ps();
	       
      for(k = 0; k < n/4*4; k += 4) {
	  a = _mm_loadu_ps(At + k + i*n);
	  x = _mm_loadu_ps(B + k + j*n);
          y = _mm_mul_ps(a,x);
          partialSum = _mm_add_ps(partialSum, y);
          b = _mm_loadu_ps(At + k + (i+1)*n);
          y = _mm_mul_ps(b,x);
          partialSum1 = _mm_add_ps(partialSum1, y);
          c = _mm_loadu_ps(At + k + (i+2)*n);
          y = _mm_mul_ps(c,x);
          partialSum2 = _mm_add_ps(partialSum2,y);
          d = _mm_loadu_ps(At + k + (i+3)*n);
          y = _mm_mul_ps(d,x);
          partialSum3 = _mm_add_ps(partialSum3,y);
          x = _mm_loadu_ps(B + k + (j+1)*n);
          y = _mm_mul_ps(a, x);
          partialSum4 = _mm_add_ps(partialSum4,y);
          y = _mm_mul_ps(b, x);
          partialSum5 = _mm_add_ps(partialSum5, y);
          y = _mm_mul_ps(c, x);
          partialSum6 = _mm_add_ps(partialSum6, y);
          y = _mm_mul_ps(d, x);
          partialSum7 = _mm_add_ps(partialSum7, y);

      }

      partialSum = _mm_hadd_ps(partialSum, partialSum1);
      partialSum1 = _mm_hadd_ps(partialSum2, partialSum3);
      partialSum2 = _mm_hadd_ps(partialSum, partialSum1); // [p0,p1,p2,p3] where p0 = sum of partialSum0

      partialSum4 = _mm_hadd_ps(partialSum4, partialSum5);
      partialSum5 = _mm_hadd_ps(partialSum6, partialSum7);
      partialSum6 = _mm_hadd_ps(partialSum4, partialSum5); // [p4,p5,p6,p7]

      c1 = _mm_add_ps(c1, partialSum2);
      c2 = _mm_add_ps(c2, partialSum6);
      //cleanup k
			if (k != n) {
				for (; k < n; k ++) {
						cij += At[k+i*n] * B[k+j*n];
						cij1 += At[k+(i+1)*n] * B[k+j*n];
						cij2 += At[k+(i+2)*n] * B[k+j*n];
						cij3 += At[k+(i+3)*n] * B[k+j*n];
						cij4 += At[k+i*n] * B[k+(j+1)*n];
						cij5 += At[k+(i+1)*n] * B[k+(j+1)*n];
						cij6 += At[k+(i+2)*n] * B[k+(j+1)*n];
						cij7 += At[k+(i+3)*n] * B[k+(j+1)*n];
				}
				float ca[] = {cij, cij1, cij2, cij3, cij4, cij5, cij6, cij7}; //LOOK FOR ALTERNATIVE
				partialSum = _mm_loadu_ps(ca);
				partialSum1 = _mm_loadu_ps(ca + 3);
				c1 = _mm_add_ps(c1, partialSum);
				c2 = _mm_add_ps(c2, partialSum1);
			}
      _mm_storeu_ps(C + i + j*n, c1);
      _mm_storeu_ps(C + i + (j+1)*n, c2);
      //count += 4; //for debug
    }
    //cleanup j NOT FULLY IMPLEMENTED
    for (; j < n; j++) {
        cij = C[i+j*n];
        partialSum = _mm_setzero_ps();
        for (k = 0; k < n/4*4; k+=4) {
            x = _mm_loadu_ps(At + k + i*n);
            y = _mm_loadu_ps(B + k + j*n);
            a = _mm_mul_ps(x, y);
            //accumulate dot prduct
            partialSum = _mm_add_ps(partialSum, a);
        }
        partialSum = _mm_hadd_ps(partialSum,partialSum);
        _MM_EXTRACT_FLOAT(temp, partialSum, 0);
        cij += temp;
        _MM_EXTRACT_FLOAT(temp, partialSum, 1);
        cij += temp;
        for (; k < n; k++) {
            cij += At[k+i*n] * B[k+j*n];
        }
        C[i+j*n] = cij;
        //count += 1; //for debug
    }
  } // NEED TO CLEANUP i
}

