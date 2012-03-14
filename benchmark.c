#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cblas.h>

/* Your function must have the following signature: */

void square_sgemm( int n, float *A, float *B, float *C );

// NOTE : this constant varies per machine
// this is for the hive machines (Intel Xeon E5620 @ 2.4GHz)
#define TSC_PER_SECOND (2400*1e6)
static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void die (const char* message)
{
  perror (message);
  exit (EXIT_FAILURE);
}

void fill (float* p, int n)
{
  for (int i = 0; i < n; ++i)
    p[i] = 2 * drand48() - 1; // Uniformly distributed over [-1, 1]
}

void absolute_value (float *p, int n)
{
  for (int i = 0; i < n; ++i)
    p[i] = fabsf (p[i]);
}

/* The benchmarking program */

int main( int argc, char **argv )
{
  srand(time(NULL));

  /* Multiples-of-32, +/- 1 */
  int test_sizes[] = 
      { 64, 65, 96, 97, 127, 128, 129, 191, 192, 229, 255, 256, 257,
      319, 320, 321, 417, 479, 480, 511, 512, 639, 640, 767, 768, 769 };

  int nsizes = sizeof(test_sizes)/sizeof(test_sizes[0]);

  /* assume last size is also the largest size */
  int nmax = test_sizes[nsizes-1];

  /* allocate memory for all problems */
  float* buf = NULL;
  buf = (float*) malloc (3 * nmax * nmax * sizeof(float));
  if (buf == NULL) die ("failed to allocate largest problem size");

  /* For each test size */
  for (int isize = 0; isize < sizeof(test_sizes)/sizeof(test_sizes[0]); ++isize)
  {
    int n = test_sizes[isize];
    if (argc > 1)
      n = 64;

    /* Create and fill 3 random matrices A,B,C*/
    float* A = buf + 0;
    float* B = A + nmax*nmax;
    float* C = B + nmax*nmax;

    fill (A, n*n);
    fill (B, n*n);
    fill (C, n*n);
    
    double Gflop_s, seconds = -1.0;
    for( int n_iterations = 1; seconds < 0.1; n_iterations *= 2 ) 
    {
      /* warm-up */
      square_sgemm( n, A, B, C );
      
      /* measure time */
      unsigned long long start, end;
      start = rdtsc();
      for( int i = 0; i < n_iterations; i++ )
	square_sgemm( n, A, B, C );
      end = rdtsc();
      seconds = (end - start)/TSC_PER_SECOND;
      
      /* compute Gflop/s rate */
      Gflop_s = 2e-9 * n_iterations * n * n * n / seconds;
    }
    /*    if (n == 3) {
	for (int printer = 0; printer < n*n; printer += 1) {
	if (printer % 3 == 0) {
	    printf("\n");
	}
	printf("%f ", *(C+printer));
    }
		printf("\n");

	for (int printer = 0; printer < n*n; printer += 1) {
	if (printer % 3 == 0) {
	    printf("\n");
	}
	printf("%f ", *(A+printer));
    }
	printf("\n");
	for (int printer = 0; printer < n*n; printer += 1) {
	if (printer % 3 == 0) {
	    printf("\n");
	}
	printf("%f ", *(B+printer));
    }
	printf("\n\n");
    }*/
    printf( "%d by %d matrix \t %g Gflop/s\n", n, n, Gflop_s );
    
    /* Ensure that error does not exceed the theoretical error bound */
		
    /* Set initial C to 0 and do matrix multiply of A*B */
    memset( C, 0, sizeof( float ) * n * n );
    square_sgemm( n, A, B, C );

    /* Subtract A*B from C using standard sgemm (note that this should be 0 to within machine roundoff) */
    cblas_sgemm( CblasColMajor,CblasNoTrans,CblasNoTrans, n,n,n, -1, A,n, B,n, 1, C,n );

    /* Subtract the maximum allowed roundoff from each element of C */
    absolute_value (A, n * n);
    absolute_value (B, n * n);
    absolute_value (C, n * n);

    cblas_sgemm( CblasColMajor,CblasNoTrans,CblasNoTrans, n,n,n, -3.0*FLT_EPSILON*n, A,n, B,n, 1, C,n );

    /* After this test if any element in C is still positive something went wrong in square_sgemm */
    for( int i = 0; i < n * n; i++ )
      if( C[i] > 0 ) {
	printf( "FAILURE: error in matrix multiply exceeds an acceptable margin\n" );
	return -1;
      }
    if (argc > 1)
      exit(0);
  }

  free(buf);
  
  return 0;
}
