#pragma once

#include <stddef.h>
#include <stdbool.h>

#define DECK_SIZE 52

enum SUITS {
  club,
  spade,
  heart,
  diamond
};

struct card {
  unsigned char value;
  enum SUITS suit;
};

struct deck {
  struct card cards[DECK_SIZE];
};

void shufflearray (const size_t n, unsigned char a[]);
struct card cardnumbertocard (const unsigned char cardnum);
void printcard (const struct card card);
struct deck generaterandomdeck ();
bool cmpcard (const struct card a, const struct card b);
