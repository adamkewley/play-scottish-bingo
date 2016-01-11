#include "playing-cards.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void shufflearray(const size_t n, unsigned char a[]) {
  // Fisher-Yates shuffle of an array
  if (n != 0) {
    for(unsigned char i = 0; i < n - 1; i++) {
      // i <= j < n
      unsigned char j = i + (rand() % (n - i));
      int tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
    }
  }
}

struct card cardnumbertocard (const unsigned char cardnum) {
  struct card ret = {};
  ret.value = cardnum % 13;

  switch(cardnum / 13) {
  case 0:
    ret.suit = spade;
    break;
  case 1:
    ret.suit = heart;
    break;
  case 2:
    ret.suit = club;
    break;
  case 3:
    ret.suit = diamond;
    break;
  }

  return ret;
}

bool cmpcard (const struct card a, const struct card b) {
  return a.value == b.value && a.suit == b.suit;
}

void printcard (const struct card card) {
  printf("%u of ", card.value);

  switch (card.suit) {
  case spade:
    printf("%s", "spades");
    break;
  case heart:
    printf("%s", "hearts");
    break;
  case club:
    printf("%s", "clubs");
    break;
  case diamond:
    printf("%s", "diamond");
    break;
  }

  putchar('\n');
}

struct deck generaterandomdeck () {
  unsigned char a[DECK_SIZE];
  struct deck ret;

  for(unsigned char i = 0; i < DECK_SIZE; i++)
    a[i] = i;

  shufflearray(DECK_SIZE, a);

  for(int i = 0; i < DECK_SIZE; i++) {
    int val = a[i];
    ret.cards[i] = cardnumbertocard(val);
  }

  return ret;
}
