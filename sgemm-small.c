#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <x86intrin.h>

#define NUM_REGISTERS 4;


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <x86intrin.h>

#define NUM_REGISTERS 4;

void square_sgemm( int n, float *A, float *B, float *C ) {
    int i, j , k, l;
    int count = 0; //for debug
    float At[n*n] __attribute__ ((aligned(16)));
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

    /*for (i = 0; i < n; i ++) {
	for (j = 0; j < n; j ++) {
	    At[i + j*n] = A[j + i *n];
	}
	}*/

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
	}
    /*
	for (i = 0; i < n; i += 1) {
	    for (j = 0; j < n; j += 1) {
		printf(" %g ", At[i + j*n]);
	    }
	    printf(" \n ");
	}
	printf(" \n ");
	for (i = 0; i < n; i += 1) {
	    for (j = 0; j < n; j += 1) {
		printf(" %g ", A[i + j*n]);
	    }
	    printf(" \n ");
	}
	printf(" \n ");*/


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
			printf("cleanup K");
			/*printf(" %d ", k + j*n);
			printf(" %d ", k + (j+1)*n);
			printf(" \n ");*/
			cij += At[k+i*n] * B[k+j*n];
			cij1 += At[k+(i+1)*n] * B[k+j*n];
			cij2 += At[k+(i+2)*n] * B[k+j*n];
			cij3 += At[k+(i+3)*n] * B[k+j*n];
			cij4 += At[k+i*n] * B[k+(j+1)*n];
			cij5 += At[k+(i+1)*n] * B[k+(j+1)*n];
			cij6 += At[k+(i+2)*n] * B[k+(j+1)*n];
			cij7 += At[k+(i+3)*n] * B[k+(j+1)*n];
		    }
		    // printf(" \n ");
		    //float ca[] = {cij, cij1, cij2, cij3, cij4, cij5, cij6, cij7}; //LOOK FOR ALTERNATIVE
		    //partialSum = _mm_loadu_ps(ca);
		    //partialSum1 = _mm_loadu_ps(ca + 4);
		    partialSum = _mm_set_ps(cij3, cij2, cij1, cij);
		    partialSum1 = _mm_set_ps(cij7, cij6, cij5, cij4);
		    c1 = _mm_add_ps(c1, partialSum);
		    c2 = _mm_add_ps(c2, partialSum1);
		}
		//printf(" %d ", i + j*n);
		//printf(" %d ", i + (j+1)*n);
		//printf(" \n ");
		_mm_storeu_ps(C + i + j*n, c1);
		_mm_storeu_ps(C + i + (j+1)*n, c2);
		count += 8; //for debug
		}
	//cleanup j
	for (; j < n; j++) {
	    printf("cleanup J");
	    //cij = C[i+j*n];
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
		//y = _mm_loadu_ps(B + k + j*n);
		a = _mm_mul_ps(x, y);
		partialSum1 = _mm_add_ps(partialSum1, a);
		
		x = _mm_loadu_ps(At + k + (i+2)*n);
		//y = _mm_loadu_ps(B + k + j*n);
		a = _mm_mul_ps(x, y);
		partialSum2 = _mm_add_ps(partialSum2, a);
		
		x = _mm_loadu_ps(At + k + (i+3)*n);
		//y = _mm_loadu_ps(B + k + j*n);
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
	    count += 4; //for debug
	}
	//NEED TO CLEANUP i
    }
      /* For each row i of A */
    for (; i < n; ++i) {
	printf("cleanup");
    /* For each column j of B */
    for (int j = 0; j < n; ++j) 
    {
      /* Compute C(i,j) */
      float cij = C[i+j*n];
      for( int k = 0; k < n; k++ )
	cij += A[i+k*n] * B[k+j*n];
      C[i+j*n] = cij;
    }
    }
}
    /*
    cij = 0.0;
    cij1 = 0.0;
    cij2 = 0.0;
    cij3 = 0.0;
    cij4 = 0.0;
    cij5 = 0.0;
    cij6 = 0.0;
    cij7 = 0.0;
    __m128 zero = _mm_setzero_ps();
    __m128 z;
    float pSum[4];
    float pSum1[4];
    float pSum2[4];
    float pSum3[4];

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
      partialSum = _mm_hadd_ps(partialSum, zero);
      partialSum1 = _mm_hadd_ps(partialSum1, zero);
      partialSum2 = _mm_hadd_ps(partialSum2, zero);
      partialSum3 = _mm_hadd_ps(partialSum3, zero);
      partialSum4 = _mm_hadd_ps(partialSum4, zero);
      partialSum5 = _mm_hadd_ps(partialSum5, zero);
      partialSum6 = _mm_hadd_ps(partialSum6, zero);
      partialSum7 = _mm_hadd_ps(partialSum7, zero);

      partialSum = _mm_hadd_ps(partialSum, zero);
      partialSum1 = _mm_hadd_ps(partialSum1, zero);
      partialSum2 = _mm_hadd_ps(partialSum2, zero);
      partialSum3 = _mm_hadd_ps(partialSum3, zero);
      partialSum4 = _mm_hadd_ps(partialSum4, zero);
      partialSum5 = _mm_hadd_ps(partialSum5, zero);
      partialSum6 = _mm_hadd_ps(partialSum6, zero);
      partialSum7 = _mm_hadd_ps(partialSum7, zero);

      _mm_storeu_ps(pSum, partialSum);
      _mm_storeu_ps(pSum1, partialSum1);
      _mm_storeu_ps(pSum2, partialSum2);
      _mm_storeu_ps(pSum3, partialSum3);

	  cij += pSum[0];
	  cij1 += pSum1[0];
	  cij2 += pSum2[0];
	  cij3 += pSum3[0];

	  _mm_storeu_ps(pSum, partialSum4);
      _mm_storeu_ps(pSum1, partialSum5);
      _mm_storeu_ps(pSum2, partialSum6);
      _mm_storeu_ps(pSum3, partialSum7);

      cij4 += pSum[0];
      cij5 + pSum1[0];
      cij6 += pSum2[0];
      cij7 += pSum3[0];

      /*_mm_storeu_ps(pSum, partialSum);
			_mm_storeu_ps(pSum1, partialSum1);
			_mm_storeu_ps(pSum2, partialSum2);
			_mm_storeu_ps(pSum3, partialSum3);
			for (l = 0; l < 4; l++) {
				cij += pSum[l];
				cij1 += pSum1[l];
				cij2 += pSum2[l];
				cij3 += pSum3[l];
			}
			_mm_storeu_ps(pSum, partialSum4);
			_mm_storeu_ps(pSum1, partialSum5);
			_mm_storeu_ps(pSum2, partialSum6);
			_mm_storeu_ps(pSum3, partialSum7);
			for (l = 0; l < 4; l++) {
				cij4 += pSum[l];
				cij5 += pSum1[l];
				cij6 += pSum2[l];
				cij7 += pSum3[l];
				}*/
			//cleanup k
/*			for (; k < n; k ++) {
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
	_mm_storeu_ps(pSum, partialSum);
	for (l = 0; l < 4; l++) {
	    cij += pSum[l];
	}
	for (; k < n; k++) {
	    //printf(" %f $$$ %f \n", At[k+i*n], B[k + j*n]);
	    //printf(" .. %d .. %d .. \n", k+i*n, k + j*n);
	    cij += At[k+i*n] * B[k+j*n];
	}
	C[i+j*n] = cij;
	//count += 1; //for debug
    }
  }
  //if (127*127 == n*n) {
		    //printf("  %d!:%d  ",count, n*n);
  //		}
  //printf("\n");
  //printf("\n");

}
    /*
    // For each row i of A 
    for (; i < n; ++i) {
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
		count += 8; //for debug
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
	    count += 1; //for debug
	}
    }
}




/*
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
  float pSum[4];
  float pSum1[4];
  float pSum2[4];
  float pSum3[4];
  float cij=0.0, cij1=0.0, cij2=0.0, cij3=0.0, cij4=0.0, cij5=0.0, cij6=0.0, cij7=0.0;
  //transpose A
  //I was unable to simd this without doing extra store/loads
  for (i = 0; i < n; i ++) {
  for (j = 0; j < n/4*4; j += 4) {
  At[i+j*n] = A[j+i*n];
  At[i+(j+1)*n] = A[j+i*n + 1];
  At[i+(j+2)*n] = A[j+i*n + 2];
  At[i+(j+3)*n] = A[j+i*n + 3];
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
      
  partialSum = _mm_hadd_ps(partialSum, zero);
  partialSum1 = _mm_hadd_ps(partialSum1, zero);
  partialSum2 = _mm_hadd_ps(partialSum2, zero);
  partialSum3 = _mm_hadd_ps(partialSum3, zero);
  partialSum4 = _mm_hadd_ps(partialSum4, zero);
  partialSum5 = _mm_hadd_ps(partialSum5, zero);
  partialSum6 = _mm_hadd_ps(partialSum6, zero);
  partialSum7 = _mm_hadd_ps(partialSum7, zero);

  partialSum = _mm_hadd_ps(partialSum, zero);
  partialSum1 = _mm_hadd_ps(partialSum1, zero);
  partialSum2 = _mm_hadd_ps(partialSum2, zero);
  partialSum3 = _mm_hadd_ps(partialSum3, zero);
  partialSum4 = _mm_hadd_ps(partialSum4, zero);
  partialSum5 = _mm_hadd_ps(partialSum5, zero);
  partialSum6 = _mm_hadd_ps(partialSum6, zero);
  partialSum7 = _mm_hadd_ps(partialSum7, zero);

  /*_mm_storeu_ps(pSum, partialSum);
  _mm_storeu_ps(pSum1, partialSum1);
  _mm_storeu_ps(pSum2, partialSum2);
  _mm_storeu_ps(pSum3, partialSum3);*/

/*    _MM_EXTRACT_FLOAT(temp, partialSum, 0);
      cij += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum1, 0);
      cij1 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum2, 0);
      cij2 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum3, 0);
      cij3 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum4, 0);
      cij4 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum5, 0);
      cij5 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum6, 0);
      cij6 += temp;
      _MM_EXTRACT_FLOAT(temp, partialSum7, 0);
      cij7 += temp;
      
   
      /*_mm_storeu_ps(pSum, partialSum4);
      _mm_storeu_ps(pSum1, partialSum5);
      _mm_storeu_ps(pSum2, partialSum6);
      _mm_storeu_ps(pSum3, partialSum7);*/
	  
/*cij4 += pSum[0];
  cij5 += pSum1[0];
  cij6 += pSum2[0];
  cij7 += pSum3[0];*/

/*_mm_storeu_ps(pSum, partialSum);
  _mm_storeu_ps(pSum1, partialSum1);
  _mm_storeu_ps(pSum2, partialSum2);
  _mm_storeu_ps(pSum3, partialSum3);*/
//for (l = 0; l < 4; l++) {
//cij += pSum[l];
//cij1 += pSum1[l];
//	cij2 += pSum2[l];
//	cij3 += pSum3[l];
//}
/*_mm_storeu_ps(pSum, partialSum4);
  _mm_storeu_ps(pSum1, partialSum5);
  _mm_storeu_ps(pSum2, partialSum6);
*/
//_mm_storeu_ps(pSum3, partialSum7);
//for (l = 0; l < 4; l++) {
//cij4 += pSum[l];
//cij5 += pSum1[l];
//cij6 += pSum2[l];
//			cij7 += pSum3[l];
//	}
//cleanup k
/*	for (; k < n; k ++) {
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
	/*    for (; j < n; j++) {
	cij = C[i+j*n];
	partialSum = _mm_setzero_ps();
	for (k = 0; k < n/4*4; k+=4) {
	x = _mm_loadu_ps(At + k + i*n);
	y = _mm_loadu_ps(B + k + j*n);
	z = _mm_mul_ps(x, y);
	//accumulate dot prduct
	partialSum = _mm_add_ps(partialSum, z);
	}
	_mm_storeu_ps(pSum, partialSum);
	for (l = 0; l < 4; l++) {
	cij += pSum[l];
	}
	for (; k < n; k++) {
	//printf(" %f $$$ %f \n", At[k+i*n], B[k + j*n]);
	//printf(" .. %d .. %d .. \n", k+i*n, k + j*n);
	cij += At[k+i*n] * B[k+j*n];
	}
	C[i+j*n] = cij;
	//count += 1; //for debug
	}
	}
	//if (127*127 == n*n) {
	//printf("  %d!:%d  ",count, n*n);
	//		}
	//printf("\n");
	//printf("\n");

	}

	/*int main(void) {
	float data[4096];
	float result[4096];
	for (int set = 0; set < 4096; set += 1) {
	data[set] = 1.0*set;
	result[set] = 0.0;
	}
	for (int i = 0; i < 4095; i += 1) {
	//printf("%f @@@ ", data[i]);
	//printf("%f : ", result[i]);
	}
	//printf("%f\n\n\n\n\n\n", result[4036]);
	square_sgemm(64, data, data, result);
	}*/
/*void square_sgemm( int n, float *A, float *B, float *C ) {
  int i, j , k;
  float At[n*n];
  __m128 x;
  __m128 y;
  __m128 partialSum;
  __m128 partialSum1;
  __m128 partialSum2;
  __m128 partialSum3;
  float pSum[4];
  float pSum1[4];
  float pSum2[4];
  float pSum3[4];

  //transpose A (naive, can be improved with register blocking)
  //I was unable to simd this without doing extra store/loads
  for (i = 0; i < n; i ++) {
  for (j = 0; j < n/4*4; j += 4) {
  At[i+j*n] = A[j+i*n];
  At[i+(j+1)*n] = A[j+1+i*n];
  At[i+(j+2)*n] = A[j+2+i*n];
  At[i+(j+3)*n] = A[j+3+i*n];
  }
  }
  /*for (i = 0; i < n; i ++) {
  for (j = i*n; j < i*n + n; j ++) {
  At[n*(j % n) + i] = A[j];
  //At[n*j + i] = A[i + j];

  }
  }*/
/*for (i = 0; i < n; i ++) {
  for (j = i*n; j < (i*n + n)/4*4; j += 4) {
  At[n*(j % n) + i] = A[j];
  At[n*((j + 1) % n) + i] = A[j + 1];
  At[n*((j + 2) % n) + i] = A[j + 2];
  At[n*((j + 3) % n) + i] = A[j + 3];
  }
  }*/
/*
  for (i = 0; i < n; i ++) {
  for (j = i*n; j < (i*n + n)/4*4; j += 4) {
  At[j] = A[n*(j % n) + i];
  At[j + 1] = A[n*((j + 1) % n) + i];
  At[j + 2] = A[n*((j + 2) % n) + i];
  At[j + 3] = A[n*((j + 3) % n) + i];
  }
  }*/
// For each row i of A 
/*for (j = 0; j < n; ++j)
// For each column j of B 
for (i = 0; i < n; i++)
{
// Compute C(i,j)
float cij = C[i+j*n];

//this will hold 4 floats which sum to the dot product
partialSum = _mm_setzero_ps();
partialSum1 = _mm_setzero_ps();
partialSum2 = _mm_setzero_ps();
partialSum3 = _mm_setzero_ps();
for(k = 0; k < n/16*16; k += 16) {
x = _mm_loadu_ps(At + k + i*n);
y = _mm_loadu_ps(B + k + j*n);
x = _mm_mul_ps(x, y);
//accumulate dot prduct
partialSum = _mm_add_ps(partialSum, x);
x = _mm_loadu_ps(At + k + 4 + i*n);
y = _mm_loadu_ps(B + k + 4 + j*n);
x = _mm_mul_ps(x, y);
//accumulate dot prduct
partialSum1 = _mm_add_ps(partialSum1, x);
x = _mm_loadu_ps(At + k + 8 + i*n);
y = _mm_loadu_ps(B + k + 8 + j*n);
x = _mm_mul_ps(x, y);
//accumulate dot prduct
partialSum2 = _mm_add_ps(partialSum2, x);
x = _mm_loadu_ps(At + k + 12 + i*n);
y = _mm_loadu_ps(B + k + 12 + j*n);
x = _mm_mul_ps(x, y);
//accumulate dot prduct
partialSum3 = _mm_add_ps(partialSum3, x);
}
partialSum = _mm_add_ps(partialSum, partialSum2);
partialSum1 = _mm_add_ps(partialSum1, partialSum3);
partialSum = _mm_add_ps(partialSum, partialSum1);
_mm_storeu_ps(pSum, partialSum);

for (k = 0; k < 4; k++) {
cij += pSum[k];
}
C[i+j*n] = cij;
}
}

/*void square_sgemm(int n, float *A, float *B, float *C) {
__m128 subRow;
__m128 subCol;
float rowContents[4];
//float *temp;
float temp[4];
float total;
int counter;
for (int h = 0; h < n; h += 1) {
for (int i = 0; i < n/4*4; i += 4) {
counter = 0;
for (int fillRow = i; counter < 4; fillRow += 1) {
rowContents[counter] = A[n*(fillRow - i)];
counter += 1;
}
subRow = _mm_loadu_ps(rowContents);
for (int j = 0; j < n*n; j += 4) {
subCol = _mm_loadu_ps(B + j); 
subCol = _mm_mul_ps(subRow, subCol);
_mm_storeu_ps(temp, subCol);	
total = 0;
for (int m = 0; m < 4; m += 1) {
total += temp[m];
}
C[h + j/n*n] += total;
}
}
}	
}
*/
