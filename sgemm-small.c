#include <stdio.h>
#include <stdlib.h>
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
	__m128 z;
	__m128 a;
	__m128 zero = _mm_setzero_ps();
	__m128 partialSum;
	__m128 partialSum1;
	__m128 partialSum2;
	__m128 partialSum3;
	__m128 partialSum4;
	__m128 partialSum5;
	__m128 partialSum6;
	__m128 partialSum7;
	/*
	float pSum[4];
	float pSum1[4];
	float pSum2[4];
	float pSum3[4];
	*/
	float cij=0.0, cij1=0.0, cij2=0.0, cij3=0.0, cij4=0.0, cij5=0.0, cij6=0.0, cij7=0.0;
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
			
			/*
			At[i+j*n] = A[j + i*n];
			At[i+(j+1)*n] = A[j + 1 + i*n];
			At[i+(j+2)*n] = A[j + 2 + i*n];
			At[i+(j+3)*n] = A[j + 3 + i*n];
			*/
		}
		for (; j<n; j ++) {
		    At[i+j*n] = A[j+i*n];
		}
  }

  // For each row i of A 
  for (i = 0; i < n; ++i) {
    // For each column j of B 
    for (j = 0; j < n/8*8; j+=8)
    {
      // Compute C(i,j)
	
	cij = C[i+j*n];
	cij1 = C[i+(j+1)*n];
	cij2 = C[i+(j+2)*n];
	cij3 = C[i+(j+3)*n];
	cij4 = C[i+(j+4)*n];
	cij5 = C[i+(j+5)*n];
	cij6 = C[i+(j+6)*n];
	cij7 = C[i+(j+7)*n];
	

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
	  x = _mm_loadu_ps(At + k + i*n);
	  y = _mm_loadu_ps(B + k + j*n);
	  z = _mm_mul_ps(x, y);
	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+1) *n);
	  a = _mm_mul_ps(x, y);
	  partialSum = _mm_add_ps(partialSum, z);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+2)*n);
	  z = _mm_mul_ps(x, y);
	  partialSum1 = _mm_add_ps(partialSum1, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+3)*n);
	  a = _mm_mul_ps(x, y);
	  partialSum2 = _mm_add_ps(partialSum2, z);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+4)*n);
	  z = _mm_mul_ps(x, y);
	  partialSum3 = _mm_add_ps(partialSum3, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+5) *n);
	  a = _mm_mul_ps(x, y);
	  partialSum4 = _mm_add_ps(partialSum4, z);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+6)*n);
	  z = _mm_mul_ps(x, y);
	  partialSum5 = _mm_add_ps(partialSum5, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+7)*n);
	  a = _mm_mul_ps(x, y);
	  partialSum6 = _mm_add_ps(partialSum6, z);

	  //accumulate dot prduct
	  partialSum7 = _mm_add_ps(partialSum7, a);
	  
      }

			partialSum = _mm_hadd_ps(partialSum, partialSum1);
			partialSum1 = _mm_hadd_ps(partialSum2, partialSum3);
			partialSum2 = _mm_hadd_ps(partialSum, partialSum1); // [p0,p1,p2,p3] where p1 = accumulation of partialSum1
			
			partialSum4 = _mm_hadd_ps(partialSum4, partialSum5);
			partialSum5 = _mm_hadd_ps(partialSum6, partialSum7);
			partialSum6 = _mm_hadd_ps(partialSum4, partialSum5); // [p4,p5,p6,p7]

      _MM_EXTRACT_FLOAT(temp, partialSum2, 0);
      cij += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum2, 1);
      cij1 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum2, 2);
      cij2 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum2, 3);
      cij3 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum6, 0);
      cij4 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum6, 1);
      cij5 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum6, 2);
      cij6 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum6, 3);
      cij7 += temp;
      
			//cleanup k
			for (; k < n; k ++) {
			    cij += At[k+i*n] * B[k+j*n];
			    cij1 += At[k+i*n] * B[k+(j+1)*n];
			    cij2 += At[k+i*n] * B[k+(j+2)*n];
			    cij3 += At[k+i*n] * B[k+(j+3)*n];
			    cij4 += At[k+i*n] * B[k+(j+4)*n];
			    cij5 += At[k+i*n] * B[k+(j+5)*n];
			    cij6 += At[k+i*n] * B[k+(j+6)*n];
			    cij7 += At[k+i*n] * B[k+(j+7)*n];
			}
			C[i+j*n] = cij;
			C[i+(j+1)*n] = cij1;
			C[i+(j+2)*n] = cij2;
			C[i+(j+3)*n] = cij3;
			C[i+(j+4)*n] = cij4;
			C[i+(j+5)*n] = cij5;
			C[i+(j+6)*n] = cij6;
			C[i+(j+7)*n] = cij7;
			//count += 4; //for debug
    }
		//cleanup j
    for (; j < n; j++) {
	cij = C[i+j*n];
	partialSum = _mm_setzero_ps();
	for (k = 0; k < n/4*4; k+=4) {
	    x = _mm_loadu_ps(At + k + i*n);
	    y = _mm_loadu_ps(B + k + j*n);
	    z = _mm_mul_ps(x, y);
	    //accumulate dot prduct
	    partialSum = _mm_add_ps(partialSum, z);
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
  }
}