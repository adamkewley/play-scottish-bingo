/*
 * Calculates the derangement distribution of a deck shuffle. A full derangement
 * of a set is when all elements of the set are in a different position from when
 * they started, after a shuffle.
 */

#include <stdio.h>
#include <stdlib.h>
#include "playing-cards.h"

#define HAND_SIZE 8

struct hand {
  struct card cards[HAND_SIZE];
};

int generateuniquenumbers (const int a, const int b, const int m, int ret[]) {
  if(b - a > m) return -1;

  // sequence of numbers in range
  int n = b - a;

  // loop count for whole sequence (in) and result (im)
  int in = 0;
  int im = 0;

  for(in = 0; in < n && im < m; ++in) {
    // remaining numbers in sequence (rn) and
    // remaining numbers to return (rm)
    int rm = m - im;
    int rn = n - in;

    // with a probability of rm/rn, select numbers
    // randomly. same as "chance of number x in rn being
    // chosen when rm choices to be made"
    if(rand() % rn < rm)
      ret[im++] = in + a;
  }

  return 1;
}

int countcardoverlap(const struct deck a, const struct deck b) {
  int j = 0;
  for(int i = 0; i < DECK_SIZE; i++) {
    if (a.cards[i].value == b.cards[i].value && 
	a.cards[i].suit == b.cards[i].suit)
      j++;
  }

  return j;
}

int main () {  
  FILE *fp = fopen("/dev/urandom", "r");
  unsigned int seed;
  fread(&seed, 4, 1, fp);
  srand(seed);
  fclose(fp);

  int count[DECK_SIZE] = { 0 };

  for (int i = 0; i < 1000000; i++) {
    const struct deck a = generaterandomdeck();
    const struct deck b = generaterandomdeck();

    int overlap = countcardoverlap(a, b);

    count[overlap]++;
  }

  for(int i = 0; i < DECK_SIZE; i++)
    printf("%d %d\n", i, count[i]);

  return 0;
}
