#include "dominion.h"

////////////////////////////////////// GLOBALS
// Print set to 1, 0 no printing
#ifndef DEBUG_FLAG
#define DEBUG_FLAG 0
#endif

#ifndef PRINT_GAME
#define PRINT_GAME 0
#endif

#ifndef ITERATIONS
#define ITERATIONS 1000000
#endif

#ifndef FAIL_MAX
#define FAIL_MAX 1000000
#endif

#ifndef RANDOMTESTHELPER_H
#define RANDOMTESTHELPER_H
#define NUM_POSSIBLE_K 20
#define NUM_NON_KINGDOM 7

// Number of supply piles for kingdoms
#define NUM_K 10

// Valid number of players
#define MAX_P 4
#define MIN_P 2

// Potential Kingdom Cards
int kingdoms[NUM_POSSIBLE_K];
int nonKingdom[NUM_NON_KINGDOM];

int printGameState(struct gameState* game, int printDecks);
int setGameState(struct gameState* preGame, int deckSize, int discardSize, int handSize );
int compareGameState( struct gameState* pre, struct gameState* post );
int randomTreasure();
#endif
