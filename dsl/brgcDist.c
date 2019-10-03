/*===================================================================*/
/* C program for distribution from the Combinatorial Object Server.  */
/* Generates the binary reflected Gray code (BRGC).                  */
/* No input error checking.  Assumes 0 <= NN <= MAX.                 */
/* Outputs both the transition sequence and the BRGC list.           */
/* Algorithm is CAT (Constant Amortized Time).                       */
/* The program can be modified, translated to other languages, etc., */
/* so long as proper acknowledgement is given (author and source).   */
/* Written by Frank Ruskey, 1994.                                    */
/* The latest version of this program may be found at the site       */
/* http://sue.uvic.ca/~cos/inf/perm/SubsetInfo.html                  */
/*===================================================================*/
#include <stdio.h>

#define MAX 50 /* maximum value of NN */

int NN;         /* generate all subsets of [NN] */
int g[MAX + 1]; /* bitstring representation of subset */

void PrintIt(int n) {
  int i;
  printf("%d  : ", n); /* transition sequence */
  for (i = 1; i <= NN; ++i)
    printf("%d ", g[i]); /* bitstring */
  printf("\n");
} /* end PrintIt */

void flip(int n) {
  PrintIt(n);
  g[n] = 1 - g[n];
} /* end flip */

void BRGC(int n) {
  if (n > 0) {
    BRGC(n - 1);
    flip(n);
    BRGC(n - 1);
  }
} /* end BRGC */

void Initialize() {
  int i;
  for (i = 1; i <= NN; ++i)
    g[i] = 0; /* but any other bitstring would work */
} /* end Initialize */

int main() {
  printf("Enter n: ");
  scanf("%d", &NN);
  Initialize();
  BRGC(NN);
  PrintIt(NN);
  return 0;
}
