#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"


/* Unit Test #2 Dominion */

/*

Test the function:

int shuffle(int player, struct gameState *state)

INPUT Game State with variety of çards in deck and deckCounts
* DeckCounts -1, 0, MAX_DECK+1 will Return -1
* DeckCounts 1, 100, MAX_DECK
* Deck with variety of cards

OUTPUT Game State with cards in deck and in a different order
* Card total count is the Same
* Card count of each card is the Same
* Return value is zero for valid input, -1 for invalid deckCount
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
  char * test = "Invalid Player Deck Count (0)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = 0;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;   // Expect Rejected Input
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");


  // TEST #2
  test = "Invalid Player Deck Count (-1)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = -1;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;   // Expect Rejected Input
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");


  // TEST #3
  test = "Invalid Player Deck Count (MAX_DECK+1)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = MAX_DECK+1;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = -1;   // Expect Rejected Input
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #4
  test = "Valid Player Deck Count (MAX_DECK)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = MAX_DECK;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");


  // TEST #5
  test = "Valid Player Deck Count (1)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = 1;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");

  // TEST #6
  test = "Valid Player Deck Count (100)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  gamePre.deckCount[0] = 100;  //set player deck to zero
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Game State", "PASS", "FAIL");




  // TEST #7
  test = "Valid Player Starting Deck (10)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  int numCard1 = 3;
  int numCard2 = 7;
  int numCards = numCard1 + numCard2;
  gamePre.deckCount[0] = 0;
  for (j = 0; j < numCard1; j++) {
    gamePre.deck[0][j] = estate;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1; j < numCard1 + numCard2; j++) {
    gamePre.deck[0][j] = copper;
    gamePre.deckCount[0]++;
  }
  assert(gamePre.deckCount[0] == numCards); // Valid Input is Correct
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State (Shuffled)", "PASS", "FAIL");
  myAssert(gamePre.deckCount[0] == gamePost.deckCount[0], "Same Number of Cards", "PASS", "FAIL");
    // Check sorted decks are the same
  qsort ((void*)(gamePre.deck[0]), gamePre.deckCount[0], sizeof(int), compare);
  qsort ((void*)(gamePost.deck[0]), gamePost.deckCount[0], sizeof(int), compare);
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Cards", "PASS", "FAIL");

  // TEST #8
  test = "Valid Player Deck (15)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numCard1 = 3;
  numCard2 = 7;
  int numCard3 = 5;
  numCards = numCard1 + numCard2 + numCard3;
  gamePre.deckCount[0] = 0;
  for (j = 0; j < numCard1; j++) {
    gamePre.deck[0][j] = estate;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1; j < numCard1 + numCard2; j++) {
    gamePre.deck[0][j] = copper;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1 + numCard2; j < numCard1 + numCard2 + numCard3; j++) {
    gamePre.deck[0][j] = province;
    gamePre.deckCount[0]++;
  }
  assert(gamePre.deckCount[0] == numCards); // Valid Input is Correct
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State (Shuffled)", "PASS", "FAIL");
  myAssert(gamePre.deckCount[0] == gamePost.deckCount[0], "Same Number of Cards", "PASS", "FAIL");
    // Check sorted decks are the same
  qsort ((void*)(gamePre.deck[0]), gamePre.deckCount[0], sizeof(int), compare);
  qsort ((void*)(gamePost.deck[0]), gamePost.deckCount[0], sizeof(int), compare);
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Cards", "PASS", "FAIL");

  // TEST #8
  test = "Valid Player Deck (150)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numCard1 = 30;
  numCard2 = 70;
  numCard3 = 50;
  numCards = numCard1 + numCard2 + numCard3;
  gamePre.deckCount[0] = 0;
  for (j = 0; j < numCard1; j++) {
    gamePre.deck[0][j] = estate;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1; j < numCard1 + numCard2; j++) {
    gamePre.deck[0][j] = copper;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1 + numCard2; j < numCard1 + numCard2 + numCard3; j++) {
    gamePre.deck[0][j] = duchy;
    gamePre.deckCount[0]++;
  }
  assert(gamePre.deckCount[0] == numCards); // Valid Input is Correct
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State (Shuffled)", "PASS", "FAIL");
  myAssert(gamePre.deckCount[0] == gamePost.deckCount[0], "Same Number of Cards", "PASS", "FAIL");
    // Check sorted decks are the same
  qsort ((void*)(gamePre.deck[0]), gamePre.deckCount[0], sizeof(int), compare);
  qsort ((void*)(gamePost.deck[0]), gamePost.deckCount[0], sizeof(int), compare);
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Cards", "PASS", "FAIL");

  // TEST #8
  test = "Valid Player Deck (500)";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  numCard1 = 450;
  numCard2 = 25;
  numCard3 = 25;
  numCards = numCard1 + numCard2 + numCard3;
  gamePre.deckCount[0] = 0;
  for (j = 0; j < numCard1; j++) {
    gamePre.deck[0][j] = estate;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1; j < numCard1 + numCard2; j++) {
    gamePre.deck[0][j] = copper;
    gamePre.deckCount[0]++;
  }
  for (j = numCard1 + numCard2; j < numCard1 + numCard2 + numCard3; j++) {
    gamePre.deck[0][j] = duchy;
    gamePre.deckCount[0]++;
  }
  assert(gamePre.deckCount[0] == numCards); // Valid Input is Correct
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;
  result = shuffle( 0, &gamePost);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State (Shuffled)", "PASS", "FAIL");
  myAssert(gamePre.deckCount[0] == gamePost.deckCount[0], "Same Number of Cards", "PASS", "FAIL");
    // Check sorted decks are the same
  qsort ((void*)(gamePre.deck[0]), gamePre.deckCount[0], sizeof(int), compare);
  qsort ((void*)(gamePost.deck[0]), gamePost.deckCount[0], sizeof(int), compare);
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) == 0), "Same Cards", "PASS", "FAIL");

  printf("\n\n");
  return 0;
}
