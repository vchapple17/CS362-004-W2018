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

int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)

INPUT - Gain card at supplyPos index
* Accept when supply is good
* Reject when supply is gone or not in play
* Test flag to Discard, Deck, Hand

OUTPUT
* Game State
Only supplyCount at supplyPos changed by decrease of 1
Player's card count increases by one (deck, hand, discard)
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
  int player = 0; // only player
  char * teststring = "gainCard()";
  // TEST #1
  char * test = "Reject a supply count of 0.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  int supplyPos = 1;
  int flag = 0;   // discard
  gamePre.supplyCount[supplyPos] = 0;  // Set supply count -1, 0, or 1
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;    // Reject
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #2
  test = "Reject a supply count of -1.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  supplyPos = 1;
  flag = 0;   // discard
  gamePre.supplyCount[supplyPos] = -1;  // Set supply count -1, 0, or 1
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;    // Reject
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #3
  test = "Accept a supply count of 1 and discard.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  supplyPos = 1;
  flag = 0;   // discard
  gamePre.supplyCount[supplyPos] = 1;   // Set valid supply count 1
  int discardCount = 10;
  int deckCount = 15;
  int handCount = 5;
  gamePre.discardCount[player] = discardCount;
  for (i = 0; i < discardCount; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  gamePre.deckCount[player] = deckCount;
  for (i = 0; i < deckCount; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  gamePre.handCount[player] = handCount;
  for (i = 0; i < handCount; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert( gamePost.discard[player][discardCount] == supplyPos, "Discard pile adds supply card", "PASS", "FAIL");
  myAssert( gamePre.discardCount[player] + 1 == gamePost.discardCount[player], "Discard Count +1", "PASS", "FAIL");
  myAssert( gamePost.supplyCount[supplyPos] + 1 == gamePre.supplyCount[supplyPos], "Supply Card count decreases by 1.", "PASS", "FAIL");
    // Check only these changes
  gamePre.discard[player][gamePre.discardCount[player]] = supplyPos;
  gamePre.discardCount[player] += 1;
  gamePre.supplyCount[supplyPos] -= 1;
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Otherwise Same Game State", "PASS", "FAIL");


  // TEST #4
  test = "Accept a supply count of 1 and deck.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  supplyPos = 1;
  flag = 1;   // discard
  gamePre.supplyCount[supplyPos] = 1;  // Set valid supply count 1
  discardCount = 10;
  deckCount = 15;
  handCount = 5;
  gamePre.discardCount[player] = discardCount;
  for (i = 0; i < discardCount; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  gamePre.deckCount[player] = deckCount;
  for (i = 0; i < deckCount; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  gamePre.handCount[player] = handCount;
  for (i = 0; i < handCount; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert( gamePost.deck[player][deckCount] == supplyPos, "deck pile adds supply card", "PASS", "FAIL");
  myAssert( gamePre.deckCount[player] + 1 == gamePost.deckCount[player], "deck Count +1", "PASS", "FAIL");
  myAssert( gamePost.supplyCount[supplyPos] + 1 == gamePre.supplyCount[supplyPos], "Supply Card count decreases by 1.", "PASS", "FAIL");
    // Check only these changes
  gamePre.deck[player][gamePre.deckCount[player]] = supplyPos;
  gamePre.deckCount[player] += 1;
  gamePre.supplyCount[supplyPos] -= 1;
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Otherwise Same Game State", "PASS", "FAIL");


  // TEST #5
  test = "Accept a supply count of 1 and hand.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  supplyPos = 1;
  flag = 2;   // hand
  gamePre.supplyCount[supplyPos] = 1;  // Set valid supply count
  discardCount = 10;
  deckCount = 15;
  handCount = 5;
  gamePre.discardCount[player] = discardCount;
  for (i = 0; i < discardCount; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  gamePre.deckCount[player] = deckCount;
  for (i = 0; i < deckCount; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  gamePre.handCount[player] = handCount;
  for (i = 0; i < handCount; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert( gamePost.hand[player][handCount] == supplyPos, "hand pile adds supply card", "PASS", "FAIL");
  myAssert( gamePre.handCount[player] + 1 == gamePost.handCount[player], "hand Count +1", "PASS", "FAIL");
  myAssert( gamePost.supplyCount[supplyPos] + 1 == gamePre.supplyCount[supplyPos], "Supply Card count decreases by 1.", "PASS", "FAIL");
    // Check only these changes
  gamePre.hand[player][gamePre.handCount[player]] = supplyPos;
  gamePre.handCount[player] += 1;
  gamePre.supplyCount[supplyPos] -= 1;
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Otherwise Same Game State", "PASS", "FAIL");


  // TEST #6
  test = "Reject a supply count of 1 and invalid flag.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  supplyPos = 1;
  flag = 3;   // invalid flag
  gamePre.supplyCount[supplyPos] = 1;
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;    // Reject
  result = gainCard(supplyPos,&gamePost, flag, player);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  printf("\n");
  return 0;
}
