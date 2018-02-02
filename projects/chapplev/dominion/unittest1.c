#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"

/* Unit Test #1 Dominion */

/*

Test the function:

int isGameOver(struct gameState *state)

*/

int myAssert(int conditional, char * name, char * T, char * F) {
  if (conditional) {
    printf("%s\t%s\n", T, name);
  }
  else {
    printf("%s\t%s\n", F, name);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  // Kingdom Cards
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

  // Non-Kingdom cards excluding "province"
  int nonKingdom[6] = {curse, estate, duchy, copper, silver, gold};

  // Setup gameStates
  struct gameState game;
  struct gameState gamePre;
  struct gameState gamePost;
  int result = -1;
  int expected = -1;
  int i = 0;

  initializeGame(2, k, 1, &game);

  // Set Num Players and counts
  game.numPlayers = 2;
  for( i = 0; i < game.numPlayers; i++ ) {
    game.deckCount[i] =  1 * MAX_DECK;
    game.discardCount[i] = 1 * MAX_DECK;
    game.handCount[i] = 1 *  MAX_HAND;
    memset(game.hand[i], 0, sizeof(int) * MAX_HAND); // From testDrawCard.c
  }

  // TEST #1
  char * test = "Returns 1 for empty Province stack.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[province] = 0;   // Force Province stack to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 1;   // Expect that game is over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #2
  test = "Returns 0 for non-empty Province stack.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[province] = 1;   // Force Province stack to 1
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #3
  test = "Returns 1 for 3 empty (non-province) supply piles.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Force 3 supply piles to be zero
  gamePre.supplyCount[nonKingdom[1]] = 0;
  gamePre.supplyCount[nonKingdom[2]] = 0;
  gamePre.supplyCount[nonKingdom[province]] = 1;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 1;   // Expect that game is over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #4
  test = "Returns 0 for 2 empty (non-province) supply piles.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Exactly 2 supply piles are zero
  gamePre.supplyCount[nonKingdom[1]] = 0;
  gamePre.supplyCount[nonKingdom[province]] = 1;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #5
  test = "Returns 0 for 1 empty (non-province) supply piles.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Exactly 1 supply pile is zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #6
  test = "Returns 1 for 4 empty (non-province) supply piles.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Force 3 supply piles to be zero
  gamePre.supplyCount[nonKingdom[1]] = 0;
  gamePre.supplyCount[nonKingdom[2]] = 0;
  gamePre.supplyCount[nonKingdom[province]] = 1;   // Province card is 3rd index
  gamePre.supplyCount[nonKingdom[4]] = 0;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 1;   // Expect that game is over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");


  // TEST #7
  test = "Returns 1 for 5 empty (non-province) supply piles.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n\n======TEST: %s======\n", test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Force 5 supply piles to be zero
  gamePre.supplyCount[nonKingdom[1]] = 0;
  gamePre.supplyCount[nonKingdom[2]] = 0;
  gamePre.supplyCount[nonKingdom[province]] = 1;  // Province card is 3rd index
  gamePre.supplyCount[nonKingdom[4]] = 0;
  gamePre.supplyCount[nonKingdom[5]] = 0;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 1;   // Expect that game is over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");
  printf("\n\n");

  return 0;
}
