#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "myAssert.h"


/* Card Test #2 Dominion */
/*

Test the implementation of Adventerer
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

  char * teststring = "Adventerer Card";

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
  gamePre.deckCount[player] = 10;      // treasure at index 4, 5, 6
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  gamePre.playedCardCount = 0;
  int handPos = 0;
  int coin_bonus = 0;
  int coin_bonus_pre = 0;
  int card = adventurer;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set handPos card as adventurer
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);

  // result = adventurer_card(int z, int * temphand, int cardDrawn, int drawntreasure, int currentPlayer, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)

  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
    // 1 more card in hand (2 drawn, smithy played);
  myAssert( (gamePre.handCount[player] + 1 == gamePost.handCount[player]) , "Hand Count Increased by 1", "PASS", "FAIL");
    // 2 fewer cards in deck if deck has 2 treasures
  myAssert( (gamePre.deckCount[player] > gamePost.deckCount[player]) , "Deck Count decrease", "PASS", "FAIL");
  // Treasure is 4th and 5th card drawn (index 9, 8, 7 are not treasures)
  myAssert( (gamePre.deckCount[player] - 5 == gamePost.deckCount[player]) , "Deck Count decrease by 5", "PASS", "FAIL");
  // Cards played increase by 4 (3 drawn and adventurer played)
  myAssert( (gamePre.playedCardCount + 4 == gamePost.playedCardCount) , "playedCardCount increased by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");


  // TEST #2
  test = "cardEffect(): Shuffling necessary.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = 10;
  gamePre.handCount[player] = 10;
  handPos = 0;
  coin_bonus = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set handPos card as adventurer
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);

  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert( (gamePre.handCount[player] + 1 == gamePost.handCount[player]) , "Hand Count Increased by 1", "PASS", "FAIL");
    // Deck Count less than 9 and non-negative
  myAssert( (9 > gamePost.deckCount[player]) , "Deck Count 8 or less", "PASS", "FAIL");
  myAssert( (0 <= gamePost.deckCount[player]) , "Deck Count positive value", "PASS", "FAIL");
  // Cards played increase by at least 1
  myAssert( (gamePre.playedCardCount < gamePost.playedCardCount) , "playedCardCount increased by at least 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");

  // TEST #3
  test = "cardEffect(): No cards to draw.";
  printf("\n======TEST: %s: %s======\n", teststring, test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  player = 0;   // current player
  gamePre.numPlayers = 2;
  gamePre.whoseTurn = player;
  gamePre.deckCount[player] = 0;
  gamePre.discardCount[player] = 0;
  gamePre.handCount[player] = 10;
  handPos = 0;
  coin_bonus = 0;
  for (i = 0; i < gamePre.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < gamePre.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < gamePre.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  gamePre.hand[player][handPos] = card; // Set handPos card as adventurer
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  result = cardEffect(card, -1, -1, -1, &gamePost, handPos, &coin_bonus);

  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");
  myAssert( (gamePre.handCount[player] + 1 == gamePost.handCount[player]) , "Hand Count Increased by 1", "PASS", "FAIL");
    // Deck Count same
  myAssert( (gamePre.deckCount[player] == gamePost.deckCount[player]) , "Deck Count same", "PASS", "FAIL");
  // Cards played increase by 1
  myAssert( (gamePre.playedCardCount +1 == gamePost.playedCardCount) , "Played card Count increase by 1", "PASS", "FAIL");
  myAssert( (coin_bonus_pre == coin_bonus) , "Coin bonus same.", "PASS", "FAIL");


  printf("\n\n");
  return 0;
}
