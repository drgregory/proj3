/* Name : Gregory Roberts
   Login : cs61c-il
   Name : Owen Lu
   Login : cs61c-ck
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <x86intrin.h>

void square_sgemm( int n, float *A, float *B, float *C ) {
    int i, j , k, l;
    float temp, temp1, temp2, temp3, temp4;
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

    float *At = malloc(n*n*sizeof(float));

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
	for (; j<n; j++) {
	    At[i + j*n] = A[j+i*n];
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
		if (k < n) {
		    cij = 0.0;
		    cij1 = 0.0;
		    cij2 = 0.0;
		    cij3 = 0.0;
		    cij4 = 0.0;
		    cij5 = 0.0;
		    cij6 = 0.0;
		    cij7 = 0.0;
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
		    partialSum = _mm_set_ps(cij3, cij2, cij1, cij);
		    partialSum1 = _mm_set_ps(cij7, cij6, cij5, cij4);
		    c1 = _mm_add_ps(c1, partialSum);
		    c2 = _mm_add_ps(c2, partialSum1);
		    }
		_mm_storeu_ps(C + i + j*n, c1);
		_mm_storeu_ps(C + i + (j+1)*n, c2);
		}
	//cleanup j
	for (; j < n; j++) {
	    c1 = _mm_loadu_ps(C+i+j*n);
	    partialSum = _mm_setzero_ps();
	    partialSum1 = _mm_setzero_ps();
	    partialSum2 = _mm_setzero_ps();
	    partialSum3 = _mm_setzero_ps();
	    for (k = 0; k < n/4*4; k+=4) {
		x = _mm_loadu_ps(At + k + i*n);
		y = _mm_loadu_ps(B + k + j*n);
		a = _mm_mul_ps(x, y);
		partialSum = _mm_add_ps(partialSum, a);

		x = _mm_loadu_ps(At + k + (i+1)*n);
		a = _mm_mul_ps(x, y);
		partialSum1 = _mm_add_ps(partialSum1, a);
		
		x = _mm_loadu_ps(At + k + (i+2)*n);
		a = _mm_mul_ps(x, y);
		partialSum2 = _mm_add_ps(partialSum2, a);
		
		x = _mm_loadu_ps(At + k + (i+3)*n);
		a = _mm_mul_ps(x, y);
		partialSum3 = _mm_add_ps(partialSum3, a);
	    }
	    partialSum = _mm_hadd_ps(partialSum, partialSum1);
	    partialSum2 = _mm_hadd_ps(partialSum2, partialSum3);
	    c1 = _mm_add_ps(_mm_hadd_ps(partialSum, partialSum2), c1);
	    temp1 = 0.0;
	    temp2 = 0.0;
	    temp3 = 0.0;
	    temp4 = 0.0;
	    for (; k < n; k++) {
		temp1 += At[k+i*n] * B[k+j*n];
		temp2 += At[k+(i+1)*n] * B[k+j*n];
		temp3 += At[k+(i+2)*n] * B[k+j*n];
		temp4 += At[k+(i+3)*n] * B[k+j*n];
	    }
	    //float collect[4] = {cij, cij1, cij2, cij3};
	    d = _mm_set_ps(temp4, temp3, temp2, temp1);
	    c1 = _mm_add_ps(c1, d);
	    _mm_storeu_ps(C+i+j*n, c1);	    
	    }
    }

    // For each row i of A 
    for (;i < n; ++i) {
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
	  d = _mm_mul_ps(x, y);
	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+1) *n);
	  a = _mm_mul_ps(x, y);
	  partialSum = _mm_add_ps(partialSum, d);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+2)*n);
	  d = _mm_mul_ps(x, y);
	  partialSum1 = _mm_add_ps(partialSum1, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+3)*n);
	  a = _mm_mul_ps(x, y);
	  partialSum2 = _mm_add_ps(partialSum2, d);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+4)*n);
	  d = _mm_mul_ps(x, y);
	  partialSum3 = _mm_add_ps(partialSum3, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+5) *n);
	  a = _mm_mul_ps(x, y);
	  partialSum4 = _mm_add_ps(partialSum4, d);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+6)*n);
	  d = _mm_mul_ps(x, y);
	  partialSum5 = _mm_add_ps(partialSum5, a);

	  //accumulate dot prduct
	  y = _mm_loadu_ps(B + k + (j+7)*n);
	  a = _mm_mul_ps(x, y);
	  partialSum6 = _mm_add_ps(partialSum6, d);

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
    }
		//cleanup j
    for (; j < n; j++) {
	cij = C[i+j*n];
	partialSum = _mm_setzero_ps();
	for (k = 0; k < n/4*4; k+=4) {
	    x = _mm_loadu_ps(At + k + i*n);
	    y = _mm_loadu_ps(B + k + j*n);
	    d = _mm_mul_ps(x, y);
	    //accumulate dot prduct
	    partialSum = _mm_add_ps(partialSum, d);
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
    }
  }
    free(At);
}

