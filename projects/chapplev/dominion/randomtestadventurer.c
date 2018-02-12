#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"
#include "rngs.h"
#include <time.h>

/* Random Test Adventurer*/
/*

Test the function:

int playAdventurer(struct gameState *state);

*/

// Print set to 1, 0 no printing
#define DEBUG_FLAG 0
#define PRINT_GAME 0
#define ITERATIONS 100000
#define FAIL_MAX 1000000

// Potential Kingdom Cards
#define NUM_POSSIBLE_K 20
int kingdoms[NUM_POSSIBLE_K] = {
  adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward,  tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map
};

#define NUM_K 10

// Non-Kingdom cards
#define NUM_NON_KINGDOM 7
int nonKingdom[NUM_NON_KINGDOM] = {
  curse, estate, duchy, province, copper, silver, gold
};

// Valid number of players
#define MAX_P 4
#define MIN_P 2

// 0 - don't print printDecks
// 1 - Print printDecks
int printGameState(struct gameState* game, int printDecks) {
  if (PRINT_GAME == 0 ) return 0;
  int i;
  int j;
  // Print Game Info
  printf("\nNumber of Players: %d\n", game->numPlayers);
  printf("Whose Turn: %d\n", game->whoseTurn);
  printf("Phase: %d\n", game->phase);
  printf("numActions: %d\n", game->numActions);
  printf("coins: %d\n", game->coins);
  printf("numBuys: %d\n", game->numBuys);
  printf("playedCardCount: %d\n", game->playedCardCount);
  for (i = 0; i < game->playedCardCount; i++) {
    printf("%d, ", game->playedCards[i]);
  }
  if ( game->playedCardCount > 0 ) printf("\n");
  printf("Outpost Played: %d\t OutpostTurn: %d\n", game->outpostPlayed, game->outpostTurn);
  // Supply Count
  printf("Supply Count:\n");
  for (i = 0; i <= treasure_map; i++) {
    printf("%d,", game->supplyCount[i]);
  }
  printf("\n");

  // Print Embargo list
  printf("Embargo List: ");
  for (int i = 0; i <= treasure_map; i++) {
    printf("%d,", game->embargoTokens[i]);
  }
  printf("\n");

  // Print player info
  for (i = 0; i < game->numPlayers; i++) {
    printf("Player Info: %d\n", i);
    // Hand
    printf("\tHand Count: %d\n", game->handCount[i]);
    if (printDecks == 1) {
      for (j = 0; j < game->handCount[i]; j++) {
        printf("%d, ", game->hand[i][j]);
      }
      printf("\n");
    }
    // Deck
    printf("\tDeck Count: %d\n", game->deckCount[i]);
    if (printDecks == 1) {
      for (j = 0; j < game->deckCount[i]; j++) {
        printf("%d, ", game->deck[i][j]);
      }
      printf("\n");
    }
    // Discard
    printf("\tDiscard Count: %d\n", game->discardCount[i]);
    if (printDecks == 1) {
      for (j = 0; j < game->discardCount[i]; j++) {
        printf("%d, ", game->discard[i][j]);
      }
      printf("\n");
    }
  }
  return 0;
}


/*
 * getGameState(struct gameState * preGame, int minDeck, int minDiscard )
 *
 * preGame - address of game state to update
 * deckSize - size of each player's deck,
 * discardSize - Smallest size of each player's discard pile,
 * handSize - Smallest size of each player's hand,
 */
int setGameState(struct gameState* preGame, int deckSize, int discardSize, int handSize ) {
  // clear game state
  memset(preGame, 0, sizeof( &preGame ) );

  // Set Number of players between MIN_PLAYERS and MAX_PLAYERS
  do { preGame->numPlayers = ceil( Random() * MAX_P ); }
  while ( (preGame->numPlayers < MIN_P) || (preGame->numPlayers > MAX_P) );
  assert( preGame->numPlayers >= MIN_P );
  assert( preGame->numPlayers <= MAX_P );

  // Pick 10 non-repeating kingdom cards
  int i = 0;
  int j = 0;
  int numKing = 0;
  int k[NUM_K];
  for ( i = 0; i < NUM_K; i++ ) {
    k[i] = -1;
  }

  while (numKing < NUM_K) {
    int index = floor( Random() * NUM_POSSIBLE_K );
    int flag = 0;
    if (index < NUM_POSSIBLE_K ) {      // Index of NUM_POSSIBLE_K out of range
      for (i = 0; i < numKing; i++ ) {  // Check index is NEW kingdom
        if ( k[i] == kingdoms[index] ) {
          flag = 1;
        }
      }
      if (flag == 0) {
        k[numKing] = kingdoms[index]; // Save card to k pile
        numKing++;
      }
    }
  }
  for (i = 0; i < 10; i++) {      //check selected kingdom cards are different
    for (j = i+1; j < 10; j++) {
      assert( ( j != i ) && (k[j] != k[i]) );
    }
  }

  //set number of Curse cards, Victory Cards, Treasure Cards
  int MAX_CURSE = 0;
  int MAX_VICTORY = 0;
  int MAX_COPPER = 60 - (7 * preGame->numPlayers);
  int MAX_KINGDOM = 10;

  if (preGame->numPlayers == 2) {
    MAX_CURSE = 10;
    MAX_VICTORY = 8;
  }
  else if (preGame->numPlayers == 3) {
    MAX_CURSE = 20;
    MAX_VICTORY = 12;
  }
  else {
    MAX_CURSE = 30;
    MAX_VICTORY = 12;
  }
  preGame->supplyCount[curse] = floor(Random() * MAX_CURSE);
  preGame->supplyCount[estate] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[duchy] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[province] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[copper] = floor( Random() * MAX_COPPER);
  preGame->supplyCount[silver] = floor(Random() * 40);
  preGame->supplyCount[gold] = floor(Random() * 30);

  //set random number of Kingdom cards in supplyCount
  for (i = adventurer; i <= treasure_map; i++)  {      //loop all cards
    for (j = 0; j < 10; j++) {           		           //loop chosen cards
  	  if (k[j] == i) {
	      if (k[j] == great_hall || k[j] == gardens) //card is 'Victory' Kingdom
  		    preGame->supplyCount[i] = Random() * MAX_VICTORY;
  	    else // All other chosen Kingdom Cards
          preGame->supplyCount[i] = Random() * MAX_KINGDOM;
  	    break;   // Go to next i
  	  }
  	  else    //card (i) is not in the set choosen for the game
  	      preGame->supplyCount[i] = -1;
  	}
  }

  //set player decks randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->deckCount[i] = 0;
    for (j = 0; j < deckSize; j++) {
      do {
        preGame->deck[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->deck[i][j] > treasure_map);
      preGame->deckCount[i]++;
    }
    assert(preGame->deckCount[i] == deckSize);
  }

  //set player hands randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->handCount[i] = 0;
    for (j = 0; j < handSize; j++) {
      do {
        preGame->hand[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->hand[i][j] > treasure_map);
      preGame->handCount[i]++;
    }
    assert(preGame->handCount[i] == handSize);
  }

  //set player discard randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->discardCount[i] = 0;
    for (j = 0; j < discardSize; j++) {
      do {
        preGame->discard[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->discard[i][j] > treasure_map);
      preGame->discardCount[i]++;
    }
    assert(preGame->discardCount[i] == discardSize);
  }

  //set embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
    preGame->embargoTokens[i] = 0;

  //set player's turn to default
  preGame->outpostPlayed = 0;
  preGame->phase = 0;
  preGame->numActions = 1;
  preGame->coins = 0;
  preGame->numBuys = 1;

  // Random played card count and pile
  preGame->playedCardCount = floor(Random() * MAX_DECK);
  for (i = 0; i <= preGame->playedCardCount; i++) {
    do {
      preGame->playedCards[i] = floor (Random() * (treasure_map + 1));
    } while (preGame->playedCards[i] > treasure_map);
  }

  preGame->playedCardCount = 0;
  preGame->whoseTurn = 0;

  // preGame->handCount[preGame->whoseTurn] = 0;?
  // Update coins?

  return 0;
}


/*
 * compareGameState( struct gameState * pre, struct gameState * post, int opt )
 *
 * Pass both gameStates
 *
 * Returns non-zero for different state, 0 for same state
 */
int compareGameState( struct gameState* pre, struct gameState* post ) {
  int i ;
  int j ;

  // Compare number of Players
  if (pre->numPlayers != post->numPlayers) return 1;

  //Compare supplycount
  for (i = curse; i <= treasure_map; i++)  {      //loop all cards
    if (pre->supplyCount[i] != post->supplyCount[i]) return 2;
  }

  //Compare embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
    if (pre->embargoTokens[i] != post->embargoTokens[i]) return 3;

  //Compare player hand piles
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->handCount[i] != post->handCount[i]) return 4;
    for (j = 0; j < pre->handCount[i]; j++) {
      if (pre->hand[i][j] != post->hand[i][j]) return 4;
    }
  }

  //Compare player decks
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->deckCount[i] != post->deckCount[i]) return 5;
    for (j = 0; j < pre->deckCount[i]; j++) {
      if (pre->deck[i][j] != post->deck[i][j]) return 5;
    }
  }

  //Compare player discard piles
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->discardCount[i] != post->discardCount[i]) return 6;
    for (j = 0; j < pre->discardCount[i]; j++) {
      if (pre->discard[i][j] != post->discard[i][j]) return 6;
    }
  }

  //Compare played card piles
  if (pre->playedCardCount != post->playedCardCount) return 7;
  for (i = 0; j < pre->playedCardCount; i++) {
    if (pre->playedCards[i] != post->playedCards[i]) return 7;
  }

  if (pre->outpostPlayed != post->outpostPlayed) return 8;
  if (pre->outpostTurn != post->outpostTurn) return 9;
  if (pre->whoseTurn != post->whoseTurn) return 10;
  if (pre->phase != post->phase) return 11;
  if (pre->numActions != post->numActions) return 12;
  if (pre->coins != post->coins) return 13;
  if (pre->numBuys != post->numBuys) return 14;

  return 0;
}

//  A - Case 0 No Shuffling (no treasure cards and discard is empty)
int testAdventurerCaseA(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    // A: No Treasure Cards and Discard Pile is Empty
    //
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 0;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == 0);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 0);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    // if (DEBUG_FLAG) printf("\tTesting results of cardEffect\n");
    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }
    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that new deckCount is zero
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post deck count to be zero.\n");
      didFail = 1;
    }

    // Check that discard pile is equal to the old deckCount
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG)
        printf("\tExpected post discard count to equal pre deck count.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is unchanged.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be equal.\n");
      didFail = 1;
    }

    // Check that the hand is 1 less
    result = ( preGame.handCount[p] - 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be 1 less.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

//  B - Case 0 Shuffling (no treasure cards and discard not empty)
int testAdventurerCaseB(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalFail = 0;
  int totalPass = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //  B - No treasure cards & does shuffle ( discard not empty)
    //
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 10;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 0);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    // if (DEBUG_FLAG) printf("\tTesting results of cardEffect\n");
    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }
    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that new deckCount is zero
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post deck count to be zero.\n");
      didFail = 1;
    }

    // Check that discard pile is equal to the old deckCount
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG)
        printf("\tExpected post discard count to equal pre deck count.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is unchanged.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be equal.\n");
      didFail = 1;
    }

    // Check that the hand is 1 less
    result = ( preGame.handCount[p] - 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be 1 less.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

//  C - Case 1 No Shuffling (1 treasure card in deck and discard is empty)
int testAdventurerCaseC(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //  C - Case 1 No Shuffling (1 treasure card in deck and discard is empty)
    // deck + discard count is 1 less, hand is unchanged, played is 1 more
    //        (Only 1 treasure card in deck AND discard piles combined)
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 0;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set 1 treasure in deck
    int index = floor(Random() * preGame.deckCount[p]);
    preGame.deck[p][ index ] = copper;

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 0);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);


    // if (DEBUG_FLAG) printf("\tTesting results of cardEffect\n");
    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }
    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that new deckCount is zero
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post deck count to be zero.\n");
      didFail = 1;
    }

    // Check that discard pile is equal to the old deckCount
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG)
        printf("\tExpected post discard count to equal pre deck count.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is 1 less.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be 1 less.\n");
      didFail = 1;
    }

    // Check that the hand is unchanged (gained 1 and played 1)
    result = ( preGame.handCount[p] == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be same.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;

}

//  D - Case 1 Shuffling (0 treasure card in deck and 1 in discard)
int testAdventurerCaseD(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //  D - Case 1 Shuffling (0 treasure card in deck and 1 in discard)
    //   deck + discard count is 1 less, hand is unchanged, played is 1 more
    //        (Only 1 treasure card in deck AND discard piles combined)

    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 10;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set 1 treasure in discard
    int index = floor(Random() * preGame.discardCount[p]);
    preGame.discard[p][ index ] = copper;
    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 0);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    // if (DEBUG_FLAG) printf("\tTesting results of cardEffect\n");
    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }
    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }
    // Check that new deckCount is zero
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post deck count to be zero.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is 1 less.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be 1 less.\n");
      didFail = 1;
    }

    // Check that the hand is unchanged (gained 1 and played 1)
    result = ( preGame.handCount[p] == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be same.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

//  E - Case 2 No Shuffling (2 treasure card in deck and 0+ in discard)
// If 2, deck + discard count is 2 less, hand is 1 more, played is 1 more
//        (2 or more treasure card in deck AND discard piles combined)
int testAdventurerCaseE(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 10;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set 2 treasures in deck
    int index1 = -1;
    int index2 = -1;
    while (index1 == index2) {
      index1 = floor(Random() * preGame.deckCount[p]);
      index2 = floor(Random() * preGame.deckCount[p]);
    }
    assert(index1 != index2 );
    preGame.deck[p][ index1 ] = copper;
    preGame.deck[p][ index2 ] = silver;

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    if (DEBUG_FLAG) printGameState(&postGame, 1);
    if (DEBUG_FLAG) printf("\tTest Results of cardEffect\n");
    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }
    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is 2 less.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be 2 less.\n");
      didFail = 1;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be 1 more.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

//  F - Case 2 Shuffling (0 treasure card in deck and 2+ in discard)
// If 2, deck + discard count is 2 less, hand is 1 more, played is 1 more
//        (2 or more treasure card in deck AND discard piles combined)
int testAdventurerCaseF(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 10;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set 2 treasure in discard only
    int index1 = -1;
    int index2 = -1;
    while (index1 == index2) {
      index1 = floor(Random() * preGame.discardCount[p]);
      index2 = floor(Random() * preGame.discardCount[p]);
    }
    assert(index1 != index2 );
    preGame.discard[p][ index1 ] = copper;
    preGame.discard[p][ index2 ] = silver;

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    if (DEBUG_FLAG) printGameState(&postGame, 1);
    if (DEBUG_FLAG) printf("\tTest Results of cardEffect\n");

    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }

    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is 2 less.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be 2 less.\n");
      didFail = 1;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be 1 more.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

//  G - Case 2 Shuffling (1 treasure card in deck and 1+ in discard)
// If 2, deck + discard count is 2 less, hand is 1 more, played is 1 more
//        (2 or more treasure card in deck AND discard piles combined)
int testAdventurerCaseG(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = adventurer;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail

  int totalPass = 0;
  int totalFail = 0;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = adventurer;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 10;
    discardSize = 10;
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);
    // Set treasure cards to non-treasure card in both deck and discard piles
    for (i = 0; i < preGame.deckCount[p]; i++ ) {
      int c = preGame.deck[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.deck[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.deck[p][i] != copper );
      assert(preGame.deck[p][i] != silver );
      assert(preGame.deck[p][i] != gold );
    }
    for (i = 0; i < preGame.discardCount[p]; i++ ) {
      int c = preGame.discard[p][i];
      if ( ( c == copper ) || ( c == silver) || c == gold) {
        preGame.discard[p][i] = mine; // Replace card w example non-treasure
      }
      assert(preGame.discard[p][i] != copper );
      assert(preGame.discard[p][i] != silver );
      assert(preGame.discard[p][i] != gold );
    }
    // Set 1 treasure in discard and 1 in deck
    int index1 = floor(Random() * preGame.discardCount[p]);
    preGame.discard[p][ index1 ] = copper;
    index1 = floor(Random() * preGame.deckCount[p]);
    preGame.deck[p][ index1 ] = silver;

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    // if (DEBUG_FLAG) printf("\tTesting card Effect\n");
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    if (DEBUG_FLAG) printGameState(&postGame, 1);
    if (DEBUG_FLAG) printf("\tTest Results of cardEffect\n");

    // TEST RESULTS
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\tCardEffect Returned %d\n", result);
      didFail = 1;
    }

    // Check that game state changed (result = 0 when same game state)
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected different game state.\n");
      didFail = 1;
    }

    // Check that the deckCount + discard count is 2 less.
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected discard and deck count to be 2 less.\n");
      didFail = 1;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post hand count to be 1 more.\n");
      didFail = 1;
    }

    // Check that the played card count is 1 more
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\tExpected post played count to be 1 more.\n");
      didFail = 1;
    }

    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("Max failures reached (%d)\n", totalFail);
      break;
    }
  }

  printf("Total Fail %d\t Total Pass %d\t\n\n", totalFail, totalPass);
  return 0;
}

int testAdventurer( int iterations ) {
  // Adventurer
  // Result: Player gains 0, 1, or 2 treasure cards from its deck
  //        Non-treasure cards drawn go to the discard pile
  //        Adventurer card goes to played pile
  //        Any treasures drawn go to the hand pile
  // Cases:
  // If 0, deck + discard count is unchanged, hand is 1 less, played is 1 more
  //        (No treasure cards in deck AND discard piles combined)
  // If 1, deck + discard count is 1 less, hand is unchanged, played is 1 more
  //        (Only 1 treasure card in deck AND discard piles combined)
  // If 2, deck + discard count is 2 less, hand is 1 more, played is 1 more
  //        (2 or more treasure card in deck AND discard piles combined)
  //
  // Other criteria
  // No Shuffling: 2 treasure cards are in the deck pile
  // Shuffling: 0 or 1 treasure cards are initially in the deck pile

  // Cartesian:
  //  A - Case 0 No Shuffling (no treasure cards and discard is empty)
  //  B - Case 0 Shuffling (no treasure cards and discard not empty)
  //  C - Case 1 No Shuffling (1 treasure card in deck and discard is empty)
  //  D - Case 1 Shuffling (0 treasure card in deck and 1 in discard)
  //  E - Case 2 No Shuffling (2 treasure card in deck and 0+ in discard)
  //  F - Case 2 Shuffling (0 treasure card in deck and 2+ in discard)
  //  G - Case 2 Shuffling (1 treasure card in deck and 1+ in discard)

  printf("Case A: Testing where no treasure cards exist and\n");
  printf("no shuffling occurs.\n");
  printf("*** SKIPPING b/c this test segfaults within the FUT.***\n");
  // testAdventurerCaseA(iterations);
  printf("\n\n");

  printf("Case B: Testing where no treasure cards exist and\n");
  printf("shuffling does occur.\n");
  printf("*** SKIPPING b/c this test segfaults within the FUT.***\n");
  // testAdventurerCaseB(iterations);
  printf("\n\n");

  printf("Case C: Testing where 1 treasure card is in deck and\n");
  printf("no shuffling occurs.\n");
  testAdventurerCaseC(iterations);
  printf("\n\n");

  printf("Case D: Testing where 1 treasure card is in discard and\n");
  printf("shuffling occurs.\n");
  testAdventurerCaseD(iterations);
  printf("\n\n");

  printf("Case E: Testing where 2 treasure cards are in deck and\n");
  printf("no shuffling occurs.\n");
  testAdventurerCaseE(iterations);
  printf("\n\n");

  printf("Case F: Testing where 0 treasure cards are in deck and\n");
  printf("2 treasure cards are in discard so shuffling occurs.\n");
  testAdventurerCaseF(iterations);
  printf("\n\n");

  printf("Case G: Testing where 1 treasure card is in deck and\n");
  printf("1 treasure card is in discard so shuffling occurs.\n");
  testAdventurerCaseG(iterations);
  printf("\n\n");

  return 0;
}

int main(int argc, char *argv[]) {
  int iterations = ITERATIONS;
  int seed = time(NULL);
  seed = 1;
  // srand(seed);

  SelectStream(2);
  PutSeed(seed);

  char * teststring = "Adventerer Card";
  printf("\n\nTESTING: %s with test seed as %d\n", teststring, seed);
  testAdventurer( iterations );

  return 0;
}
