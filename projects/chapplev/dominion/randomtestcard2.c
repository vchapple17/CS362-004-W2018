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
#include "randomtesthelper.h"

/* Random Test for Village Card*/
/*

Test the card implementation in cardEffect() with village card
 Draw 1
 Action +2

INPUT
No shuffle and can draw 1
Shuffle and can draw 1
No cards to draw, no draw

OUTPUT - Check
hand count is unchanged (draw 1 and discard 1)
deck count decreases by 1
discard count increases by 1
numActions increases by 2
*/


// Case A: Deck is Empty, Requires Shuffling of non-empty discard.
// Result: Player gains 1 cards from its shuffled deck
//         Hand changes by 0 (b/c of playing village)
//         Deck+Discard count decreases by 1
//         village card goes to played pile
//         Action Count increases by 2
int testVillageCaseA(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card = village;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalPass = 0;
  int totalFail = 0;
  int errors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // index 0 for total errors
  char * test1_string;
  char * test2_string;
  char * test3_string;
  char * test4_string;
  char * test5_string;
  char * test6_string;
  char * test7_string;
  char * test8_string;
  char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    // SETUP
    int test=0;
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = village;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail


    deckSize = 0;
    do {
      discardSize = ceil( Random() * MAX_DECK ); // greater than 0
    } while (( discardSize < 1) || (discardSize > MAX_DECK ));
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);

    // Set Action Count to > 0
    preGame.numActions = 1;

    // Set Played card count
    // preGame.playedCardCount = 1;
    // for (i = 0 ; i < preGame.playedCardCount; i++) {
    //   preGame.playedCards[i] = mine;    // ineffective card in played card
    // }

    // Set card hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;

    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));
    assert(memcmp(&postGame, &preGame, sizeof (struct gameState)) == 0);

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playVillage(&postGame, handPos);
    if (DEBUG_FLAG) printGameState(&postGame, 1);

    // TEST RESULTS
    // TEST 1
    test1_string = "cardEffect returned non zero.";
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test1_string);
      didFail = 1;
      test=1;
      errors[0]++;
      errors[test]++;
    }

    // Check that game state changed (result = 0 when same game state)
    // TEST 2
    test2_string = "Expected different game state.";
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test2_string);
      didFail = 1;
      test=2;
      errors[0]++;
      errors[test]++;
    }

    // Check that new discardCount is zero
    // TEST 3
    test3_string = "Expected post discard count to be zero.";
    result = ( postGame.discardCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that deckCount is equal to the old discardCount -1
    // TEST 4
    test4_string = "Expected pre discard count to be 1 more than post deck count.";
    result = ( preGame.discardCount[p] - 1 == postGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand count is same (gain 1 lose 1)
    // TEST 5
    test5_string = "Expected hand count to be equal.";
    result = ( preGame.handCount[p] == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 6
    test6_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Card goes into played pile
    // TEST 7
    test7_string = "Expected last played card to be right card.";
    result = ( postGame.playedCards[postGame.playedCardCount - 1] == card );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
      errors[0]++;
      errors[test]++;
    }

    // Action Count increases by 2
    // TEST 8
    test8_string = "Expected numActions to be 2 more.";
    result = ( preGame.numActions + 2 == postGame.numActions );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test8_string);
      didFail = 1;
      test=8;
      errors[0]++;
      errors[test]++;
    }

    //
    // Set PreGrame to Equal PostGame.
    //

    // Remove Card from hand, put in played pile
    preGame.playedCards[ preGame.playedCardCount ] = preGame.hand[p][ handPos ];
    preGame.playedCardCount++;

    // Copy Hand From PostGame (Don't know next drawn card)
    preGame.handCount[p] = 0;
    for (i = 0; i < postGame.handCount[p]; i++) {
      preGame.hand[p][i] = postGame.hand[p][i];
      preGame.handCount[p]++;
    }
    for (i = preGame.handCount[p]; i < MAX_DECK; i++) {
      // Ensure remaining slots in preGame are equal to -1
      preGame.hand[p][i] = -1;
    }

    // Copy Deck From PostGame (Don't know next drawn card)
    preGame.deckCount[p] = 0;
    for (i = 0 ; i < postGame.deckCount[p]; i++) {
      preGame.deck[p][i] = postGame.deck[p][i];
      preGame.deckCount[p]++;
    }
    for (i = preGame.deckCount[p]; i < MAX_DECK; i++) {
      // Ensure remaining slots in preGame are equal to -1
      preGame.deck[p][i] = -1;
    }

    // Set Discard Pile to -1
    for (i = 0 ; i < MAX_DECK; i++) {
      preGame.discard[p][i] = -1;
    }
    preGame.discardCount[p] = 0;

    // Action Count increases by 2
    preGame.numActions += 2;

    // if (DEBUG_FLAG) printGameState(&preGame, 0);

    // Check that game state is now same (result = 0 when same game state)
    // TEST 9
    test9_string = "Expected to pass game state adjustments.";
    result = compareGameState(&preGame, &postGame);
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test9_string);
      didFail = 1;
      test=9;
      errors[0]++;
      errors[test]++;
    }

    // Report results
    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      if (DEBUG_FLAG) printf("\tIteration #%d\t PASS \n\n", z);
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("\t***Max Iteration Failures Reached (max=%d)***\n", totalFail);
      break;
    }
  }

  printf("\tFailed %d\t Total Pass %d\t\n", totalFail, totalPass);

  if (errors[1]>0)
    printf("\tTest #%d Failed %d times: %s\n", 1, errors[1], test1_string);
  if (errors[2]>0)
    printf("\tTest #%d Failed %d times: %s\n", 2, errors[2], test2_string);
  if (errors[3]>0)
    printf("\tTest #%d Failed %d times: %s\n", 3, errors[3], test3_string);
  if (errors[4]>0)
    printf("\tTest #%d Failed %d times: %s\n", 4, errors[4], test4_string);
  if (errors[5]>0)
    printf("\tTest #%d Failed %d times: %s\n", 5, errors[5], test5_string);
  if (errors[6]>0)
    printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  if (errors[7]>0)
    printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  if (errors[8]>0)
    printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  if (errors[9]>0)
    printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

  return 0;
}

// Case B: Deck is Empty, Discard Empty. No Shuffling possible.
// Result: Player gains 0 cards so hand decreases by 1
//         Deck+Discard count still is zero
//         village card goes to played pile
//         Action Count increases by 2
int testVillageCaseB(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalPass = 0;
  int totalFail = 0;
  int errors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // index 0 for total errors
  char * test1_string;
  char * test2_string;
  char * test3_string;
  char * test4_string;
  char * test5_string;
  char * test6_string;
  char * test7_string;
  char * test8_string;
  char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    // SETUP
    int test=0;
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = village;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    deckSize = 0;
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

    // Set Action Count to > 0
    preGame.numActions = 1;

    // Set Played card count
    // preGame.playedCardCount = 1;
    // for (i = 0 ; i < preGame.playedCardCount; i++) {
    //   preGame.playedCards[i] = mine;    // ineffective card in played card
    // }

    // Set card hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;

    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));
    assert(memcmp(&postGame, &preGame, sizeof (struct gameState)) == 0);

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playVillage(&postGame, handPos);
    if (DEBUG_FLAG) printGameState(&postGame, 1);

    // TEST RESULTS
    // TEST 1
    test1_string = "cardEffect returned non zero.";
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test1_string);
      didFail = 1;
      test=1;
      errors[0]++;
      errors[test]++;
    }

    // Check that game state changed (result = 0 when same game state)
    // TEST 2
    test2_string = "Expected different game state.";
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test2_string);
      didFail = 1;
      test=2;
      errors[0]++;
      errors[test]++;
    }

    // Check that post discardCount is zero
    // TEST 3
    test3_string = "Expected post discard count to be zero.";
    result = ( postGame.discardCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that post deckCount is equal to 0
    // TEST 4
    test4_string = "Expected post deck count to be zero.";
    result = ( postGame.discardCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand count is decreased by 1 (gain 0 lose 1)
    // TEST 5
    test5_string = "Expected hand count to be 1 less.";
    result = ( preGame.handCount[p] - 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 6
    test6_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Card goes into played pile
    // TEST 7
    test7_string = "Expected last played card to be right card.";
    result = ( postGame.playedCards[postGame.playedCardCount - 1] == card );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
      errors[0]++;
      errors[test]++;
    }

    // Action Count increases by 2
    // TEST 8
    test8_string = "Expected numActions to be 2 more.";
    result = ( preGame.numActions + 2 == postGame.numActions );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test8_string);
      didFail = 1;
      test=8;
      errors[0]++;
      errors[test]++;
    }

    //
    // Set PreGrame to Equal PostGame.
    //

    // Remove Card from hand, put in played pile
    preGame.playedCards[ preGame.playedCardCount ] = preGame.hand[p][ handPos ];
    preGame.playedCardCount++;

    // Copy Hand From PostGame ( same cards minus played card )
    preGame.handCount[p] = 0;
    for (i = 0; i < postGame.handCount[p]; i++) {
      preGame.hand[p][i] = postGame.hand[p][i];
      preGame.handCount[p]++;
    }
    for (i = preGame.handCount[p]; i < MAX_DECK; i++) {
      // Ensure remaining slots in preGame are equal to -1
      preGame.hand[p][i] = -1;
    }

    // Set Deck Pile to -1 (still empty)
    for (i = 0 ; i < MAX_DECK; i++) {
      preGame.deck[p][i] = -1;
    }
    preGame.deckCount[p] = 0;

    // Set Discard Pile to -1 (still empty)
    for (i = 0 ; i < MAX_DECK; i++) {
      preGame.discard[p][i] = -1;
    }
    preGame.discardCount[p] = 0;

    // Action Count increases by 2
    preGame.numActions += 2;

    // if (DEBUG_FLAG) printGameState(&preGame, 0);

    // Check that game state is now same (result = 0 when same game state)
    // TEST 9
    test9_string = "Expected to pass game state adjustments.";
    result = compareGameState(&preGame, &postGame);
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test9_string);
      didFail = 1;
      test=9;
      errors[0]++;
      errors[test]++;
    }

    // Report results
    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      if (DEBUG_FLAG) printf("\tIteration #%d\t PASS \n\n", z);
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("\t***Max Iteration Failures Reached (max=%d)***\n", totalFail);
      break;
    }
  }

  printf("\tFailed %d\t Total Pass %d\t\n", totalFail, totalPass);

  if (errors[1]>0)
    printf("\tTest #%d Failed %d times: %s\n", 1, errors[1], test1_string);
  if (errors[2]>0)
    printf("\tTest #%d Failed %d times: %s\n", 2, errors[2], test2_string);
  if (errors[3]>0)
    printf("\tTest #%d Failed %d times: %s\n", 3, errors[3], test3_string);
  if (errors[4]>0)
    printf("\tTest #%d Failed %d times: %s\n", 4, errors[4], test4_string);
  if (errors[5]>0)
    printf("\tTest #%d Failed %d times: %s\n", 5, errors[5], test5_string);
  if (errors[6]>0)
    printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  if (errors[7]>0)
    printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  if (errors[8]>0)
    printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  if (errors[9]>0)
    printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

  return 0;
}

// Case C: Deck has 1 or more cards (No shuffling required)
// Result: Player gains 1 cards so hand stays same 1
//         Deck count is 1 less
//         Discard count unchanged
//         village card goes to played pile
//         Action Count increases by 2
int testVillageCaseC(int iterations) {
  struct gameState preGame;
  struct gameState postGame;
  int deckSize;
  int discardSize ;
  int handSize;
  int i;
  int z; // iteration count
  int v1, v2;
  int result = -1;
  int card;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalPass = 0;
  int totalFail = 0;
  int errors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // index 0 for total errors
  char * test1_string;
  char * test2_string;
  char * test3_string;
  char * test4_string;
  char * test5_string;
  char * test6_string;
  char * test7_string;
  char * test8_string;
  char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    // SETUP
    int test=0;
    i = 0;
    v1 = 0;
    v2 = 0;
    result = -1;
    card = village;
    handPos = -1;
    p = -1;
    coin_bonus = 0;
    didFail = 0;    // 0 for pass, 1 for fail

    do {
      deckSize = ceil( Random() * MAX_DECK ); // greater than 0
    } while (( deckSize < 1) || (deckSize > MAX_DECK ));
    do {
      discardSize = ceil( Random() * MAX_DECK ); // greater than 0
    } while (( discardSize < 1) || (discardSize > MAX_DECK ));
    handSize = 10;
    if (DEBUG_FLAG) printf("\tsetGameState #%d\n", z);
    setGameState(&preGame, deckSize, discardSize, handSize );

    // Set player
    p = floor (Random() * preGame.numPlayers);
    preGame.whoseTurn = p;
    assert(preGame.discardCount[p] == discardSize);
    assert(preGame.handCount[p] == handSize);
    assert(preGame.deckCount[p] == deckSize);

    // Set Action Count to > 0
    preGame.numActions = 1;

    // Set Played card count
    // preGame.playedCardCount = 1;
    // for (i = 0 ; i < preGame.playedCardCount; i++) {
    //   preGame.playedCards[i] = mine;    // ineffective card in played card
    // }

    // Set card hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;

    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));
    assert(memcmp(&postGame, &preGame, sizeof (struct gameState)) == 0);

    // RUN FUNCTION
    // if (DEBUG_FLAG) printGameState(&preGame, 1);
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playVillage(&postGame, handPos);
    // if (DEBUG_FLAG) printGameState(&postGame, 1);

    // TEST RESULTS
    // TEST 1
    test1_string = "cardEffect returned non zero.";
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test1_string);
      didFail = 1;
      test=1;
      errors[0]++;
      errors[test]++;
    }

    // Check that game state changed (result = 0 when same game state)
    // TEST 2
    test2_string = "Expected different game state.";
    result = compareGameState(&preGame, &postGame);
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test2_string);
      didFail = 1;
      test=2;
      errors[0]++;
      errors[test]++;
    }

    // Check that discardCount is same
    // TEST 3
    test3_string = "Expected discard count to be same.";
    result = ( preGame.discardCount[p] == postGame.discardCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that deckCount is equal to the old deckCount -1
    // TEST 4
    test4_string = "Expected deck count to be 1 less.";
    result = ( preGame.deckCount[p] - 1 == postGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand count is same (gain 1 lose 1)
    // TEST 5
    test5_string = "Expected hand count to be equal.";
    result = ( preGame.handCount[p] == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 6
    test6_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Card goes into played pile
    // TEST 7
    test7_string = "Expected last played card to be right card.";
    result = ( postGame.playedCards[postGame.playedCardCount - 1] == card );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
      errors[0]++;
      errors[test]++;
    }

    // Action Count increases by 2
    // TEST 8
    test8_string = "Expected numActions to be 2 more.";
    result = ( preGame.numActions + 2 == postGame.numActions );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test8_string);
      didFail = 1;
      test=8;
      errors[0]++;
      errors[test]++;
    }

    //
    // Set PreGrame to Equal PostGame.
    //

    // Remove Card from hand, put in played pile
    preGame.playedCards[ preGame.playedCardCount ] = preGame.hand[p][ handPos ];
    preGame.playedCardCount++;

    // Copy Hand From PostGame (Don't know next drawn card)
    preGame.handCount[p] = 0;
    for (i = 0; i < postGame.handCount[p]; i++) {
      preGame.hand[p][i] = postGame.hand[p][i];
      preGame.handCount[p]++;
    }
    for (i = preGame.handCount[p]; i < MAX_DECK; i++) {
      // Ensure remaining slots in preGame are equal to -1
      preGame.hand[p][i] = -1;
    }

    // Copy Deck From PostGame (Don't know next drawn card)
    preGame.deckCount[p] = 0;
    for (i = 0 ; i < postGame.deckCount[p]; i++) {
      preGame.deck[p][i] = postGame.deck[p][i];
      preGame.deckCount[p]++;
    }
    for (i = preGame.deckCount[p]; i < MAX_DECK; i++) {
      // Ensure remaining slots in preGame are equal to -1
      preGame.deck[p][i] = -1;
    }

    // Discard Pile should be unchanged

    // Action Count increases by 2
    preGame.numActions += 2;

    // if (DEBUG_FLAG) printGameState(&preGame, 0);

    // Check that game state is now same (result = 0 when same game state)
    // TEST 9
    test9_string = "Expected to pass game state adjustments.";
    result = compareGameState(&preGame, &postGame);
    if ( result != 0) {
      if (DEBUG_FLAG) printf("\n\n\n");
      if (DEBUG_FLAG) printf("\t%s (# %d)\n", test9_string, result);
      if (DEBUG_FLAG) printGameState(&preGame, 1);
      if (DEBUG_FLAG) printf("\n");
      if (DEBUG_FLAG) printGameState(&postGame, 1);
      if (DEBUG_FLAG) printf("\n\n\n");
      didFail = 1;
      test=9;
      errors[0]++;
      errors[test]++;
    }

    // Report results
    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      totalFail++;
    }
    else {
      if (DEBUG_FLAG) printf("\tIteration #%d\t PASS \n\n", z);
      totalPass++;
    }
    if (totalFail >= FAIL_MAX ) {
      printf("\t***Max Iteration Failures Reached (max=%d)***\n", totalFail);
      break;
    }
  }

  printf("\tFailed %d\t Total Pass %d\t\n", totalFail, totalPass);

  if (errors[1]>0)
    printf("\tTest #%d Failed %d times: %s\n", 1, errors[1], test1_string);
  if (errors[2]>0)
    printf("\tTest #%d Failed %d times: %s\n", 2, errors[2], test2_string);
  if (errors[3]>0)
    printf("\tTest #%d Failed %d times: %s\n", 3, errors[3], test3_string);
  if (errors[4]>0)
    printf("\tTest #%d Failed %d times: %s\n", 4, errors[4], test4_string);
  if (errors[5]>0)
    printf("\tTest #%d Failed %d times: %s\n", 5, errors[5], test5_string);
  if (errors[6]>0)
    printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  if (errors[7]>0)
    printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  if (errors[8]>0)
    printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  if (errors[9]>0)
    printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

  return 0;
}



int testVillage( int iterations ) {
  // Village
  // Result: Player gains 0 or 1 cards from its deck
  //         Hand changes by -1 or 0 (b/c of playing village)
  //         Deck+Discard count decreases by 0 or 1
  //         village card goes to played pile
  //         Action Count increases by 2

  // Cases:
  //  A - Deck Empty, Requires Shuffling of non-empty discard
  //  B - Deck Empty, Discard Empty No Shuffling possible.
  //  C - Deck has 1 or more cards

  printf("\n=============================================================\n\n");
  printf("Case A: Deck is Empty, Requires Shuffling of non-empty discard.\n");
  testVillageCaseA(iterations);

  printf("\n=============================================================\n\n");
  printf("Case B: Deck is Empty, Discard Empty. No Shuffling possible.\n");
  testVillageCaseB(iterations);
  printf("\n\n=============================================================\n\n");

  printf("Case C: Deck has 1 or more cards (No shuffling required).\n");
  testVillageCaseC(iterations);
  printf("\n\n");


  return 0;
}

int main(int argc, char *argv[]) {
  int iterations = ITERATIONS;
  int seed = time(NULL);
  seed = 140;
  // srand(seed);

  SelectStream(2);
  PutSeed(seed);

  char * teststring = "village Card";
  printf("\n\nTESTING: %s with test seed as %d\n\n\n", teststring, seed);
  time_t time0 = time(NULL);
  testVillage( iterations );
  time_t time1 = time(NULL);
  printf("\nTIME: %ld sec\n\n\n", (long)(time1-time0));

  return 0;
}
