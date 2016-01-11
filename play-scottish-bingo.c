#include <stdio.h>
#include <stdlib.h>
#include "playing-cards.h"
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define HAND_SIZE 8

const char *help_text = "\
Usage: play-scottish-bingo [options] num_players num_rounds \n\
Play num_rounds of scottish bingo with num_players. \n\
\n\
-s\t Shuffle type (deck-per-hand, exhaust-deck) \n\
\n\
Examples: \n\
  play-scottish-bingo 5 10                  Play 10 rounds of scottish bingo with 5 players. \n\
  play-scottish-bingo -s exhaust-deck 7 12  Play 12 rounds of scottish bingo with 7 players, using a deck-exhaust shuffling method.\n\
";

const unsigned char HANDS_IN_A_DECK = DECK_SIZE / HAND_SIZE;

enum SHUFFLE_TYPE {
  deck_per_hand,
  exhaust_deck
};

struct hand {
  struct card cards[HAND_SIZE];
  bool cardscalled[HAND_SIZE];
};

// Returns the number of cards drawn from 
// `gamedeck` before one of the players in `playersv` won
// the game. Returns -1 if no player won.
unsigned char playscottishbingo (const size_t playersc, 
				 struct hand *playersv, 
				 const struct deck *deck) {
  
  for (unsigned char drawidx = 0; drawidx < DECK_SIZE; drawidx++) {
    struct card drawncard = deck->cards[drawidx];
      
    // Go through each player's hand, marking cards
    // that have been called. If all their cards have
    // been called then they win.
    for (size_t player = 0; player < playersc; player++) {
      struct hand *playerhand = &playersv[player];
      bool playerhascard = false;

      for (unsigned char playercard = 0; playercard < HAND_SIZE; playercard++) {
	if (cmpcard(playerhand->cards[playercard], drawncard)) {
	  playerhand->cardscalled[playercard] = true;
	  playerhascard = true;
	  break;
	}
      }

      // Check to see if, after making their card, they've won
      if (playerhascard) {
	bool playerwins = true;
	unsigned char playercard;
	for (playercard = 0; playercard < HAND_SIZE; playercard++) {
	  if (!playerhand->cardscalled[playercard]) {
	    playerwins = false;
	    break;
	  }
	}
	if (playerwins) {
	  return drawidx;
	}
      }
    }
  }
  return -1;
}

int generateuniquenumbers (const unsigned char a, 
			   const unsigned char b, 
			   const unsigned char m, 
			   unsigned char ret[]) {
  // Knuth algorithim for solving the 'cover' problem
  // in which you need m random unique items that are
  // a subset of some set.

  // In this case, the superset is all integers between
  // `a` and `b` e.g. S = { a..b }. So ret = M (subset of) S

  // sequence of numbers in range
  int n = b - a;

  if(m > n) return -1;

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
    if(rand() % rn < rm) ret[im++] = in + a;
  }

  return 1;
}

void shuffle_deckpershuffle (const size_t playerc, struct hand *playerhands) {
  for (size_t player = 0; player < playerc; player++) {
    unsigned char playerhand[HAND_SIZE];
    generateuniquenumbers(0, 12, 2, playerhand);
    generateuniquenumbers(13, 25, 2, playerhand + 2);
    generateuniquenumbers(26, 38, 2, playerhand + 4);
    generateuniquenumbers(39, 51, 2, playerhand + 6);

    for (unsigned char j = 0; j < 8; j++) {
      playerhands[player].cards[j] = cardnumbertocard(playerhand[j]);
      playerhands[player].cardscalled[j] = false;
    }
  }
}

void populatecards (enum SUITS suit, unsigned char n, struct card cards[]) {
  unsigned char vals[n];
  generateuniquenumbers(0, n, n, vals);
  shufflearray(13, vals);
  for (unsigned char i = 0; i < n; i++) {
    cards[i].suit = suit;
    cards[i].value = vals[i];
  }
}

// In this shuffle, players are dealt cards from one deck
// until it is exhausted of cards or there are too few cards
// remaining in the deck to deal. At that point, another bunch
// of cards are dealt
void shuffle_exhaustdeck (const size_t playerc, struct hand *playerhandsv) {
  size_t i = 0;
  // Iterate through each deck needed.
  // Number of decks needed is ((#players * hand size) / (deck size)) + 1 (integer division)
  for (size_t deck = 0; deck < ((playerc * HAND_SIZE) / DECK_SIZE) + 1 &&  i < playerc; deck++) {
    // Separately shuffle the spades, hearts, clubs, and diamonds
    struct card spades[13];
    populatecards(spade, 13, spades);
    struct card hearts[13];
    populatecards(heart, 13, hearts);
    struct card clubs[13];
    populatecards(club, 13, clubs);
    struct card diamonds[13];
    populatecards(diamond, 13, diamonds);

    for (size_t hand = 0; hand < HANDS_IN_A_DECK && i < playerc; hand++) {
      playerhandsv[i].cards[0] = spades[hand];
      playerhandsv[i].cardscalled[0] = false;
      playerhandsv[i].cards[1] = spades[hand + HANDS_IN_A_DECK];
      playerhandsv[i].cardscalled[1] = false;
      playerhandsv[i].cards[2] = hearts[hand];
      playerhandsv[i].cardscalled[2] = false;
      playerhandsv[i].cards[3] = hearts[hand + HANDS_IN_A_DECK];
      playerhandsv[i].cardscalled[3] = false;
      playerhandsv[i].cards[4] = clubs[hand];
      playerhandsv[i].cardscalled[4] = false;
      playerhandsv[i].cards[5] = clubs[hand + HANDS_IN_A_DECK];
      playerhandsv[i].cardscalled[5] = false;
      playerhandsv[i].cards[6] = diamonds[hand];
      playerhandsv[i].cardscalled[6] = false;
      playerhandsv[i].cards[7] = diamonds[hand + HANDS_IN_A_DECK];
      playerhandsv[i].cardscalled[7] = false;

      i++;
    }
  }
}

int main (int argc, char **argv) {
  int c;
  enum SHUFFLE_TYPE shuffletype = deck_per_hand;
  if ( (c = getopt(argc, argv, "s:")) != -1 && c == 's') {
    // only deck-per-hand and exhaust-deck allowed
    if (!strcmp(optarg, "deck-per-hand")) {
      shuffletype = deck_per_hand;
    } else if (!strcmp(optarg, "exhaust-deck")) {
      shuffletype = exhaust_deck;
    } else {
      fprintf(stderr, "Invalid shuffle type, %s, provided. Allowed values: -s deck-per-hand, -s exhaust-deck", optarg);
      return 1;
    }
  }
  
  FILE *fp = fopen("/dev/urandom", "r");
  unsigned int seed;
  fread(&seed, sizeof(unsigned int), 1, fp);
  srand(seed);
  fclose(fp);

  if (argc - optind < 2) {
    fprintf(stderr, "Invalid number of arguments. Usage: %s num_players num_rounds\n", argv[0]); 
    return 1;
  }

  unsigned int players = strtol(argv[optind], NULL, 10);
  unsigned int rounds = strtol(argv[optind + 1], NULL, 10);

  struct hand *playerhands = malloc(sizeof(struct hand) * players);

  if (!playerhands) {
    fprintf(stderr, "Error: Not enough memory to track %u players", players);
    return 1;
  }

  for (size_t round = 0; round < rounds; round++) {
    // Deal out the players' hands
    switch (shuffletype) {
    case deck_per_hand:
      shuffle_deckpershuffle(players, playerhands);
      break;
    case exhaust_deck:
      shuffle_exhaustdeck(players, playerhands);
      break;
    }

    // Now all players have a hand, time to play.
    struct deck gamedeck = generaterandomdeck();

    unsigned char winninground = playscottishbingo(players, playerhands, &gamedeck);
    
    // + 1 because index -> round number
    printf("%u\n", winninground + 1);
  }

  return 0;
}
