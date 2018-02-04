#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"

/* Unit Test #1 Dominion */

/*

Test the function:

int isGameOver(struct gameState *state)

*/


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

  char * teststring = "isGameOver()";

  printf("\n\nTESTING: %s\n", teststring);

  // TEST #1
  char * test = "Returns 1 for empty Province stack.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
  gamePre.supplyCount[province] = 0;   // Force Province stack to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 1;   // Expect that game is over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #2
  test = "Returns 0 for non-empty Province stack.";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
  gamePre.supplyCount[province] = 1;   // Force Province stack to 1
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #3
  test = "Returns 1 for 3 empty (non-province).";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
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
  test = "Returns 0 for 2 empty (non-province).";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Exactly 2 supply piles are zero
  gamePre.supplyCount[nonKingdom[1]] = 0;
  gamePre.supplyCount[nonKingdom[province]] = 1;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #5
  test = "Returns 0 for 1 empty (non-province).";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
  gamePre.supplyCount[nonKingdom[0]] = 0;   // Exactly 1 supply pile is zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST (testcase)
  expected = 0;   // Expect that game is NOT over
  result = isGameOver(&gamePost);
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #6
  test = "Returns 1 for 4 empty (non-province).";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
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
  test = "Returns 1 for 5 empty (non-province).";
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  printf("\n======TEST: %s: %s======\n", teststring, test );
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
  printf("\n");

  return 0;
}


// //check selected kingdom cards are different
// for (i = 0; i < 10; i++) {
//   for (j = 0; j < 10; j++) {
//     if (j != i && k[j] == k[i]) {
//       return -1;
//     }
//   }
// }
//
// // Setup gameStates
// struct gameState game;
// struct gameState gamePre;
// struct gameState gamePost;
// int result = -1;
// int expected = -1;
// int i = 0;
//
// //set up random number generator
// SelectStream(1);
// PutSeed((long)randomSeed);
// // initializeGame(2, k, 1, &game);
//
// // Set Num Players and counts
// game.numPlayers = 2;
//
// // // Set Card Counts
// // game->supplyCount[curse] = 10;
// // game->supplyCount[estate] = 8;
// // game->supplyCount[duchy] = 8;
// // game->supplyCount[province] = 8;
// // game->supplyCount[copper] = 60 - (7 * 2);
// // game->supplyCount[silver] = 40;
// // game->supplyCount[gold] = 30;
// //
// // // Set Kingdom Card Count
// // for (i = adventurer; i <= treasure_map; i++)  {     	//loop all cards
// //   for (j = 0; j < 10; j++) {           		//loop chosen cards
// // 	  if (kingdomCards[j] == i) {
// //       //check if card is a 'Victory' Kingdom card
// //       if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens) {
// //         game->supplyCount[i] = 8;
// // 		  }
// // 	    else {
// // 		    game->supplyCount[i] = 10;
// // 		  }
// // 	    break;
// // 	  }
// // 	  else {    //card is not in the set choosen for the game
// // 	      game->supplyCount[i] = -1;
// // 	  }
// // 	}
// // }
