#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"


/* Unit Test #3 Dominion */

/*

Test the function:

int updateCoins(int player, struct gameState *state, int bonus)

INPUT
* Player hand treasure count varies (0 to MAX_HAND)
* Various Bonus (negative, zero, positive)

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
  char * teststring = "updateCoins()";
  // TEST #1
  char * test = "Copper 1, Silver 0, Gold 0, Bonus 0";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  int numC = 1;
  int numS = 0;
  int numG = 0;
  int bonus = 0;
  int numNonTreasure = 150;
  int numCards = numC + numS + numG + numNonTreasure;
  int coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[0];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 0;  // Init to zero for comparison
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

  // TEST #2
  test = "Copper 0, Silver 1, Gold 0, Bonus 0";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numC = 0;
  numS = 1;
  numG = 0;
  bonus = 0;
  numNonTreasure = 150;
  numCards = numC + numS + numG + numNonTreasure;
  coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[0];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 0;  // Init to zero for comparison
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

  // TEST #3
  test = "Copper 0, Silver 0, Gold 1, Bonus 0";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numC = 0;
  numS = 0;
  numG = 1;
  bonus = 0;
  numNonTreasure = 150;
  numCards = numC + numS + numG + numNonTreasure;
  coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[0];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 0;  // Init to zero for comparison
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

  // TEST #4
  test = "Copper 0, Silver 0, Gold 0, Bonus 0";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numC = 0;
  numS = 0;
  numG = 0;
  bonus = 0;
  numNonTreasure = 150;
  numCards = numC + numS + numG + numNonTreasure;
  coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[0];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 10;  // Init for comparison
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

  // TEST #5
  test = "Copper 1, Silver 12, Gold 35, Bonus 0";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numC = 1;
  numS = 12;
  numG = 25;
  bonus = 0;
  numNonTreasure = 150;
  numCards = numC + numS + numG + numNonTreasure;
  coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[0];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 0;  // Init for comparison
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

  // TEST #6
  test = "Copper 0, Silver 0, Gold 0, Bonus 150";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numC = 0;
  numS = 0;
  numG = 0;
  bonus = 150;
  numNonTreasure = 150;
  numCards = numC + numS + numG + numNonTreasure;
  coins = numC * 1 + numS * 2 + numG * 3 + bonus;
  gamePre.handCount[0] = 0;
  for (j = 0; j < numC; j++) {
    gamePre.hand[0][j] = copper;
    gamePre.handCount[0]++;
  }
  for (j = numC; j < numC + numS; j++) {
    gamePre.hand[0][j] = silver;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS; j < numC + numS + numG; j++) {
    gamePre.hand[0][j] = gold;
    gamePre.handCount[0]++;
  }
  for (j = numC + numS + numG; j < numC + numS + numG + numNonTreasure; j++) {
    gamePre.hand[0][j] = k[3];
    gamePre.handCount[0]++;
  }
  gamePre.coins = 0;  // Init for comparison
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

  printf("\n");
  return 0;
}
