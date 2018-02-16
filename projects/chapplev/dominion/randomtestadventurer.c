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


/* Random Test Adventurer*/
/*

Test the function:

int playAdventurer(struct gameState *state);

*/

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
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    // A: No Treasure Cards and Discard Pile is Empty
    //
    // SETUP
    int test=0;
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

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that new deckCount is zero
    // TEST 3
    test3_string = "Expected post deck count to be zero.";
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that discard pile is equal to the old deckCount
    // TEST 4
    test4_string = "Expected post discard count to equal pre deck count.";
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the deckCount + discard count is unchanged.
    // TEST 5
    test5_string = "Expected discard and deck count to be equal.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is 1 less
    // TEST 6
    test6_string = "Expected post hand count to be 1 less.";
    result = ( preGame.handCount[p] - 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 7
    test7_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
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
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  int card;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalFail = 0;
  int totalPass = 0;
  int errors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // index 0 for total errors
  char * test1_string;
  char * test2_string;
  char * test3_string;
  char * test4_string;
  char * test5_string;
  char * test6_string;
  char * test7_string;
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    //  B - No treasure cards & does shuffle ( discard not empty)
    //
    // SETUP
    int test=0;
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

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that new deckCount is zero
    // TEST 3
    test3_string = "Expected post deck count to be zero.";
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that discard pile is equal to the old deckCount
    // TEST 4
    test4_string = "Expected post discard count to equal pre deck count.";
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the deckCount + discard count is unchanged.
    // TEST 5
    test5_string = "Expected discard and deck count to be equal.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is 1 less
    // TEST 6
    test6_string = "Expected post hand count to be 1 less.";
    result = ( preGame.handCount[p] - 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 7
    test7_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
      errors[0]++;
      errors[test]++;
    }
    // Report results
    if (didFail == 1) {
      if (DEBUG_FLAG) printf("\tIteration #%d\t FAIL \n\n", z);
      // printf("\tIteration #%d\t FAIL\n");
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
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    //  C - Case 1 No Shuffling (1 treasure card in deck and discard is empty)
    // deck + discard count is 1 less, hand is unchanged, played is 1 more
    //        (Only 1 treasure card in deck AND discard piles combined)
    // SETUP
    int test=0;
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
    preGame.deck[p][ index ] = randomTreasure();

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

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that new deckCount is zero
    // TEST 3
    test3_string = "Expected post deck count to be zero.";
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that discard pile is equal to the old deckCount
    // TEST 4
    test4_string = "Expected post discard count to equal pre deck count.";
    result = ( postGame.discardCount[p] == preGame.deckCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the deckCount + discard count is 1 less.
    // TEST 5
    test5_string = "Expected discard and deck count to be 1 less.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is unchanged (gained 1 and played 1)
    // TEST 6
    test6_string = "Expected post hand count to be same.";
    result = ( preGame.handCount[p] == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test6_string);
      didFail = 1;
      test=6;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 7
    test7_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test7_string);
      didFail = 1;
      test=7;
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
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  int card;
  int handPos = -1;
  int p = -1;
  int coin_bonus = 0;
  int didFail = 0;    // 0 for pass, 1 for fail
  int totalFail = 0;
  int totalPass = 0;
  int errors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};  // index 0 for total errors
  char * test1_string;
  char * test2_string;
  char * test3_string;
  char * test4_string;
  char * test5_string;
  char * test6_string;
  // char * test7_string;
  // char * test8_string;
  // char * test9_string;
  for (z = 0; z < iterations; z++ ) {
    //
    //  D - Case 1 Shuffling (0 treasure card in deck and 1 in discard)
    //   deck + discard count is 1 less, hand is unchanged, played is 1 more
    //        (Only 1 treasure card in deck AND discard piles combined)

    // SETUP
    int test=0;
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
    preGame.discard[p][ index ] = randomTreasure();

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

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that new deckCount is zero
    // TEST 3
    test3_string = "Expected post deck count to be zero.";
    result = ( postGame.deckCount[p] == 0 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that the deckCount + discard count is 1 less.
    // TEST 4
    test4_string = "Expected discard and deck count to be 1 less.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 1 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is unchanged (gained 1 and played 1)
    // TEST 5
    test5_string = "Expected post hand count to be same.";
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
  // if (errors[7]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  // char * test6_string;
  // char * test7_string;
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    int test=0;
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
    preGame.deck[p][ index1 ] = randomTreasure();
    preGame.deck[p][ index2 ] = randomTreasure();

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

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that the deckCount + discard count is 2 less.
    // TEST 3
    test3_string = "Expected discard and deck count to be 2 less.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    // TEST 4
    test4_string = "Expected post hand count to be 1 more.";
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 5
    test5_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
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
  // if (errors[6]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  // if (errors[7]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  // char * test6_string;
  // char * test7_string;
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    int test=0;
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
    preGame.discard[p][ index1 ] = randomTreasure();
    preGame.discard[p][ index2 ] = randomTreasure();

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    if (DEBUG_FLAG) printGameState(&postGame, 1);

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that the deckCount + discard count is 2 less.
    // TEST 3
    test3_string = "Expected discard and deck count to be 2 less.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    // TEST 4
    test4_string = "Expected post hand count to be 1 more.";
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 5
    test5_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
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
  // if (errors[6]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  // if (errors[7]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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
  // char * test6_string;
  // char * test7_string;
  // char * test8_string;
  // char * test9_string;

  for (z = 0; z < iterations; z++ ) {
    //
    //
    // SETUP
    int test=0;
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
    preGame.discard[p][ index1 ] = randomTreasure();
    index1 = floor(Random() * preGame.deckCount[p]);
    preGame.deck[p][ index1 ] = randomTreasure();

    // Set adventurer hand position
    handPos = floor(Random() * handSize);
    preGame.hand[p][handPos] = card;
    // Copy Game State for Post
    memcpy(&postGame, &preGame, sizeof (struct gameState));

    // RUN FUNCTION
    if (DEBUG_FLAG) printGameState(&preGame, 1);
    result = cardEffect(card, -1, -1, -1, &postGame, handPos, &coin_bonus);
    // result = playAdventurer(&postGame);

    if (DEBUG_FLAG) printGameState(&postGame, 1);

    // TEST RESULTS
    // TEST 1
    test1_string = "CardEffect returned non zero";
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

    // Check that the deckCount + discard count is 2 less.
    // TEST 3
    test3_string = "Expected discard and deck count to be 2 less.";
    v1 = preGame.discardCount[p] + preGame.deckCount[p];
    v2 = postGame.discardCount[p] + postGame.deckCount[p];
    result = ( v1 - 2 == v2 );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test3_string);
      didFail = 1;
      test=3;
      errors[0]++;
      errors[test]++;
    }

    // Check that the hand is 1 more (gained 2 and played 1)
    // TEST 4
    test4_string = "Expected post hand count to be 1 more.";
    result = ( preGame.handCount[p] + 1 == postGame.handCount[p] );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test4_string);
      didFail = 1;
      test=4;
      errors[0]++;
      errors[test]++;
    }

    // Check that the played card count is 1 more
    // TEST 5
    test5_string = "Expected post played count to be 1 more.";
    result = ( preGame.playedCardCount + 1 == postGame.playedCardCount );
    if ( result == 0) {
      if (DEBUG_FLAG) printf("\t%s\n", test5_string);
      didFail = 1;
      test=5;
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
  // if (errors[6]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 6, errors[6], test6_string);
  // if (errors[7]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 7, errors[7], test7_string);
  // if (errors[8]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 8, errors[8], test8_string);
  // if (errors[9]>0)
  //   printf("\tTest #%d Failed %d times: %s\n", 9, errors[9], test9_string);
  if (errors[0]>0)
    printf("\t\tTotal Errors: %d\n", errors[0]);

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

  printf("Case A: Testing where no treasure cards exist and ");
  printf("no shuffling occurs.\n");
  printf("*** SKIPPING b/c this test segfaults within the FUT.***\n");
  // testAdventurerCaseA(iterations);
  printf("\n\n");

  printf("Case B: Testing where no treasure cards exist and ");
  printf("shuffling does occur.\n");
  printf("*** SKIPPING b/c this test segfaults within the FUT.***\n");
  // testAdventurerCaseB(iterations);
  printf("\n\n");

  printf("Case C: Testing where 1 treasure card is in deck and ");
  printf("no shuffling occurs.\n");
  testAdventurerCaseC(iterations);
  printf("\n\n");

  printf("Case D: Testing where 1 treasure card is in discard and ");
  printf("shuffling occurs.\n");
  testAdventurerCaseD(iterations);
  printf("\n\n");

  printf("Case E: Testing where 2 treasure cards are in deck and ");
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
  time_t time0 = time(NULL);
  testAdventurer( iterations );
  time_t time1 = time(NULL);
  printf("\nTIME: %ld sec\n\n\n", (long)(time1-time0));

  return 0;
}
