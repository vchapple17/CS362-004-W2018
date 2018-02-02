#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"


/* Unit Test #4 Dominion */

/*

Test the function:

int buyCard(int supplyPos, struct gameState *state)

INPUT - Buy card at supplyPos index
* Buy out of phase (phase 0, 2)
* Buy when in phase (phast 1)
* Buy when supply is good
* Buy when supply is gone
* Buy when enough coins
* Buy when not enough coins

OUTPUT
* Game State coins updated properly

*/

int main(int argc, char *argv[]) {
  // Setup gameStates Variables
  struct gameState game;
  struct gameState gamePre;
  struct gameState gamePost;
  int result = -1;
  int expected = -1;
  int i = 0, j = 0;

  // Kingdom Cards
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};

  // Non-Kingdom cards excluding "province"
  int nonKingdom[6] = {curse, estate, duchy, copper, silver, gold};

  //check selected kingdom cards are different
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      if (j != i && k[j] == k[i]) {
        printf("Aborted: kingdom cards not setup correctly.\n" );
        return -1;
      }
    }
  }

  // Set Num Players to 1
  game.numPlayers = 1;

  // TEST #1
  char * test = "";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  int supplyPos = 1;
  // Set supply count -1, 0, or 1
  gamePre.supplyCount[0] = -1;
  // Ensure enough coins in state


  assert(gamePre.handCount[0] == numCards); // Valid Input is Correct
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = updateCoins(0, &gamePost, bonus);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert(gamePre.handCount[0] == gamePost.handCount[0], "Same Number of Cards", "PASS", "FAIL");
  myAssert(gamePre.coins != gamePost.coins, "Different number of coins.", "PASS", "FAIL");
  myAssert(coins == gamePost.coins, "Correct calculations.", "PASS", "FAIL");
    // Check ONLY gamePost.coins has changed
  gamePost.coins = gamePre.coins;
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Rest of Game State is same", "PASS", "FAIL");



  printf("\n\n");
  return 0;
}
