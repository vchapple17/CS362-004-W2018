#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"


/* Card Test #1 Dominion */
/*

Test the function:

int playGreatHall(struct gameState *state, int handPos);
*** depends on drawCard() and discard() card

INPUT
No shuffle and can draw 1
Shuffle and can draw 1
No cards to draw, same state

OUTPUT - Check
hand count is unchanged (draw 1 and discard 1)
deck count decreases by 1
discard count increases by 1
numActions increases by 1
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

  char * teststring = "Great Hall Card";
  printf("\n\nTESTING: %s\n", teststring);

  // TEST #1
  char * test = "cardEffect(): No shuffling necessary.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  int player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  int handPos = 0;
  int card = great_hall;
  int coin_bonus = 0;
  int coin_bonus_pre = 0;
  gamePre.deckCount[player] = 10;
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  gamePre.numActions = 1;

  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card
  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // hand count is same
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count is same", "PASS", "FAIL");
    // 1 fewer card in deck
  myAssert( (gamePre.deckCount[player] - 1 == gamePost.deckCount[player]) , "Deck Count decrease by 1", "PASS", "FAIL");
  // playedCardCount increase by 1
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");
    // Actions increased by 1
  myAssert( gamePre.numActions + 1 == gamePost.numActions , "Action count increases by 1.", "PASS", "FAIL");

  // TEST #2
  test = "cardEffect(): Deck shuffled, but has enough cards total.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  handPos = 0;
  coin_bonus = 0;
  coin_bonus_pre = 0;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card

  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  // result = playSmithy(&gamePost, handPos);
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // hand count is same
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count is same", "PASS", "FAIL");
    // Deck count is now 9
  myAssert( (9 == gamePost.deckCount[player]) , "Deck Count is 9", "PASS", "FAIL");
    // 1 more played card
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");
    // Actions increased by 1
  myAssert( gamePre.numActions + 1 == gamePost.numActions , "Action count increases by 1.", "PASS", "FAIL");


  // TEST #3
  test = "cardEffect(): Deck shuffled, but not enough cards total.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  handPos = 0;
  coin_bonus = 0;
  coin_bonus_pre = 0;
  int cardsDrawn = 1;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = cardsDrawn;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card

  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  // result = playSmithy(&gamePost, handPos);
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // more cards in hand, but played smithy
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count Same", "PASS", "FAIL");
    // 1 fewer card in deck
  myAssert( (gamePre.deckCount[player] == gamePost.deckCount[player]) , "Deck Count zero", "PASS", "FAIL");
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");

  // TEST #4
  test = "playGreatHall(): No shuffling necessary.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  handPos = 0;
  coin_bonus = 0;
  coin_bonus_pre = 0;
  gamePre.deckCount[player] = 10;
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  gamePre.numActions = 1;

  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card
  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = playGreatHall(&gamePost, handPos);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // hand count is same
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count is same", "PASS", "FAIL");
    // 1 fewer card in deck
  myAssert( (gamePre.deckCount[player] - 1 == gamePost.deckCount[player]) , "Deck Count decrease by 1", "PASS", "FAIL");
  // playedCardCount increase by 1
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");
    // Actions increased by 1
  myAssert( gamePre.numActions + 1 == gamePost.numActions , "Action count increases by 1.", "PASS", "FAIL");

  // TEST #5
  test = "playGreatHall(): Deck shuffled, but has enough cards total.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  handPos = 0;
  coin_bonus = 0;
  coin_bonus_pre = 0;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card

  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = playGreatHall(&gamePost, handPos);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // hand count is same
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count is same", "PASS", "FAIL");
    // Deck Count is 9
  myAssert( (9 == gamePost.deckCount[player]) , "Deck Count is 9", "PASS", "FAIL");
    // 1 more played card
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");
    // Actions increased by 1
  myAssert( gamePre.numActions + 1 == gamePost.numActions , "Action count increases by 1.", "PASS", "FAIL");


  // TEST #6
  test = "playGreatHall(): Deck shuffled, but not enough cards total.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  handPos = 0;
  coin_bonus = 0;
  coin_bonus_pre = 0;
  cardsDrawn = 1;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = cardsDrawn;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set hand pile smithy card

  for (i = 0; i < gamePre.playedCardCount; i++ ) {
    gamePre.playedCards[i] = i % (treasure_map+1); // Set discard pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = playGreatHall(&gamePost, handPos);
  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // more cards in hand, but played smithy
  myAssert( (gamePre.handCount[player] == gamePost.handCount[player]) , "Hand Count Same", "PASS", "FAIL");
    // Deck count is zero
  myAssert( (gamePre.deckCount[player] == gamePost.deckCount[player]) , "Deck Count zero", "PASS", "FAIL");
  myAssert( (gamePre.playedCardCount + 1 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");


  printf("\n\n");
  return 0;
}
