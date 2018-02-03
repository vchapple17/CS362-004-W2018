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

int playSmithy(struct gameState *state, int handPos);
*** depends on drawCard() and discard card

INPUT - vary the following
* game state with various current players (1/2, 2/2, 1/4, 2/4, 3/4, 4/4)
* handPos of the smithy card
* small number of cards total (run out of cards to draw)
        0 discard, 0 deck

OUTPUT
* Current player has 3 cards added to hand
* Current player has 3 cards removed from deck
* Shouldn't there be a play area? no immediately discard
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


  // TEST #1
  char * test = "No shuffling necessary.";
  printf("\n======TEST: %s======\n", test );
  memcpy(&gamePre, &game, sizeof(struct gameState));  // Set PRE
  // INPUT
  // Set Num Players and current player
  int player = 0;   // current player
  game.numPlayers = 2;
  game.whoseTurn = player;
  int handPos = 0;
  game.deckCount[player] = 10;
  game.discardCount[player] = 10;
  game.handCount[player] = 10;
  for (i = 0; i < game.discardCount[player]; i++ ) {
    gamePre.discard[player][i] = i % (treasure_map+1); // Set discard pile
  }
  for (i = 0; i < game.deckCount[player]; i++ ) {
    gamePre.deck[player][i] = i % (treasure_map+1); // Set deck pile
  }
  for (i = 0; i < game.handCount[player]; i++ ) {
    gamePre.hand[player][i] = i % (treasure_map+1); // Set hand pile
  }
  memcpy(&gamePost, &gamePre, sizeof(struct gameState)); // Set POST
  expected = 0;    // Accept
  // result = playSmithy(&gamePost, handPos);

  // Sub Tests
  myAssert( (result == expected) , "Return Value", "PASS", "FAIL");
  myAssert( (memcmp(&gamePre, &gamePost, sizeof(struct gameState)) != 0), "Different Game State", "PASS", "FAIL");


  printf("\n\n");
  return 0;
}
