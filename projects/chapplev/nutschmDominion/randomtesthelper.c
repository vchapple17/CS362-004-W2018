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

// Non-Kingdom cards
int nonKingdom[NUM_NON_KINGDOM] = {
  curse, estate, duchy, province, copper, silver, gold
};

// Potential Kingdom Cards
int kingdoms[NUM_POSSIBLE_K] = {
  adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall, minion, steward,  tribute, ambassador, cutpurse, embargo, outpost, salvager, sea_hag, treasure_map
};


// 0 - don't print printDecks
// 1 - Print printDecks
int printGameState(struct gameState* game, int printDecks) {
 if (PRINT_GAME == 0 ) return 0;
 int i;
 int j;
 // Print Game Info
 printf("\n--------- GAME STATE:\n");
 printf("Number of Players: %d\n", game->numPlayers);
 printf("Whose Turn: %d\n", game->whoseTurn);
 printf("Phase: %d\n", game->phase);
 printf("numActions: %d\n", game->numActions);
 printf("coins: %d\n", game->coins);
 printf("numBuys: %d\n", game->numBuys);
 printf("playedCardCount: %d\n", game->playedCardCount);
 for (i = 0; i < game->playedCardCount; i++) {
   printf("%d, ", game->playedCards[i]);
 }
 if ( game->playedCardCount > 0 ) printf("\n");
 printf("\tPlayed Cards All:\t");
 for (j = 0; j < MAX_DECK; j++) {
   printf("%d, ", game->playedCards[j]);
 }
 printf("\n");

 printf("Outpost Played: %d\nOutpostTurn: %d\n", game->outpostPlayed, game->outpostTurn);
 // Supply Count
 printf("Supply Count:\t");
 for (i = 0; i <= treasure_map; i++) {
   printf("%d,", game->supplyCount[i]);
 }
 printf("\n");

 // Print Embargo list
 printf("Embargo List: ");
 for (int i = 0; i <= treasure_map; i++) {
   printf("%d,", game->embargoTokens[i]);
 }
 printf("\n");

 // Print player info
 for (i = 0; i < game->numPlayers; i++) {
   printf("Player Info: %d\n", i);
   // Hand
   printf("\tHand (%d):\t", game->handCount[i]);
   if (printDecks == 1) {
     for (j = 0; j < game->handCount[i]; j++) {
       printf("%d, ", game->hand[i][j]);
     }
     printf("\n");
     printf("\tHand All:\t");
     for (j = 0; j < MAX_DECK; j++) {
       printf("%d, ", game->hand[i][j]);
     }
     printf("\n");
   }
   // Deck
   printf("\tDeck (%d):\t", game->deckCount[i]);
   if (printDecks == 1) {
     for (j = 0; j < game->deckCount[i]; j++) {
       printf("%d, ", game->deck[i][j]);
     }
     printf("\n");
     printf("\tDeck All:\t");
     for (j = 0; j < MAX_DECK; j++) {
       printf("%d, ", game->deck[i][j]);
     }
     printf("\n");
   }
   // Discard
   printf("\tDiscard (%d):\t", game->discardCount[i]);
   if (printDecks == 1) {
     for (j = 0; j < game->discardCount[i]; j++) {
       printf("%d, ", game->discard[i][j]);
     }
     printf("\n");
     printf("\tDiscard All:\t");
     for (j = 0; j < MAX_DECK; j++) {
       printf("%d, ", game->discard[i][j]);
     }
     printf("\n");
   }
 }
 printf("-------------\n");
 return 0;
}

/*
 * getGameState(struct gameState * preGame, int minDeck, int minDiscard )
 *
 * preGame - address of game state to update
 * deckSize - size of each player's deck,
 * discardSize - Smallest size of each player's discard pile,
 * handSize - Smallest size of each player's hand,
 */
int setGameState(struct gameState* preGame, int deckSize, int discardSize, int handSize ) {
  // clear game state
  memset(preGame, 0, sizeof( &preGame ) );

  // Set Number of players between MIN_PLAYERS and MAX_PLAYERS
  do { preGame->numPlayers = ceil( Random() * MAX_P ); }
  while ( (preGame->numPlayers < MIN_P) || (preGame->numPlayers > MAX_P) );
  assert( preGame->numPlayers >= MIN_P );
  assert( preGame->numPlayers <= MAX_P );

  // Pick 10 non-repeating kingdom cards
  int i = 0;
  int j = 0;
  int numKing = 0;
  int k[NUM_K];
  for ( i = 0; i < NUM_K; i++ ) {
    k[i] = -1;
  }

  while (numKing < NUM_K) {
    int index = floor( Random() * NUM_POSSIBLE_K );
    int flag = 0;
    if (index < NUM_POSSIBLE_K ) {      // Index of NUM_POSSIBLE_K out of range
      for (i = 0; i < numKing; i++ ) {  // Check index is NEW kingdom
        if ( k[i] == kingdoms[index] ) {
          flag = 1;
        }
      }
      if (flag == 0) {
        k[numKing] = kingdoms[index]; // Save card to k pile
        numKing++;
      }
    }
  }
  for (i = 0; i < 10; i++) {      //check selected kingdom cards are different
    for (j = i+1; j < 10; j++) {
      assert( ( j != i ) && (k[j] != k[i]) );
    }
  }

  //set number of Curse cards, Victory Cards, Treasure Cards
  int MAX_CURSE = 0;
  int MAX_VICTORY = 0;
  int MAX_COPPER = 60 - (7 * preGame->numPlayers);
  int MAX_KINGDOM = 10;

  if (preGame->numPlayers == 2) {
    MAX_CURSE = 10;
    MAX_VICTORY = 8;
  }
  else if (preGame->numPlayers == 3) {
    MAX_CURSE = 20;
    MAX_VICTORY = 12;
  }
  else {
    MAX_CURSE = 30;
    MAX_VICTORY = 12;
  }
  preGame->supplyCount[curse] = floor(Random() * MAX_CURSE);
  preGame->supplyCount[estate] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[duchy] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[province] = floor(Random() * MAX_VICTORY);
  preGame->supplyCount[copper] = floor( Random() * MAX_COPPER);
  preGame->supplyCount[silver] = floor(Random() * 40);
  preGame->supplyCount[gold] = floor(Random() * 30);

  //set random number of Kingdom cards in supplyCount
  for (i = adventurer; i <= treasure_map; i++)  {      //loop all cards
    for (j = 0; j < 10; j++) {           		           //loop chosen cards
  	  if (k[j] == i) {
	      if (k[j] == great_hall || k[j] == gardens) //card is 'Victory' Kingdom
  		    preGame->supplyCount[i] = floor(Random() * MAX_VICTORY);
  	    else // All other chosen Kingdom Cards
          preGame->supplyCount[i] = floor(Random() * MAX_KINGDOM);
  	    break;   // Go to next i
  	  }
  	  else    //card (i) is not in the set choosen for the game
  	      preGame->supplyCount[i] = -1;
  	}
  }

  //set player decks randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->deckCount[i] = 0;
    for (j = 0; j < deckSize; j++) {
      do {
        preGame->deck[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->deck[i][j] > treasure_map);
      preGame->deckCount[i]++;
    }
    assert(preGame->deckCount[i] == deckSize);
  }

  //set player hands randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->handCount[i] = 0;
    for (j = 0; j < handSize; j++) {
      do {
        preGame->hand[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->hand[i][j] > treasure_map);
      preGame->handCount[i]++;
    }
    assert(preGame->handCount[i] == handSize);
  }

  //set player discard randomly
  for (i = 0; i < preGame->numPlayers; i++) {
    preGame->discardCount[i] = 0;
    for (j = 0; j < discardSize; j++) {
      do {
        preGame->discard[i][j] = floor (Random() * (treasure_map + 1));
      } while (preGame->discard[i][j] > treasure_map);
      preGame->discardCount[i]++;
    }
    assert(preGame->discardCount[i] == discardSize);
  }

  //set embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
    preGame->embargoTokens[i] = 0;

  //set player's turn to default
  preGame->outpostPlayed = 0;
  preGame->phase = 0;
  preGame->numActions = 1;
  preGame->coins = 0;
  preGame->numBuys = 1;

  // Random played card count and pile
  preGame->playedCardCount = floor(Random() * MAX_DECK);
  for (i = 0; i <= preGame->playedCardCount; i++) {
    do {
      preGame->playedCards[i] = floor (Random() * (treasure_map + 1));
    } while (preGame->playedCards[i] > treasure_map);
  }

  // preGame->playedCardCount = 0;
  preGame->whoseTurn = 0;

  // preGame->handCount[preGame->whoseTurn] = 0;?
  // Update coins?

  return 0;
}

/*
 * compareGameState( struct gameState * pre, struct gameState * post, int opt )
 *
 * Pass both gameStates
 *
 * Returns non-zero for different state, 0 for same state
 */
int compareGameState( struct gameState* pre, struct gameState* post ) {
  int i ;
  int j ;

  // Compare number of Players
  if (pre->numPlayers != post->numPlayers) return 1;

  //Compare supplycount
  for (i = curse; i <= treasure_map; i++)  {      //loop all cards
    if (pre->supplyCount[i] != post->supplyCount[i]) return 2;
  }

  //Compare embargo tokens to 0 for all supply piles
  for (i = 0; i <= treasure_map; i++)
    if (pre->embargoTokens[i] != post->embargoTokens[i]) return 3;

  //Compare player hand piles
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->handCount[i] != post->handCount[i]) return 4;
    for (j = 0; j < pre->handCount[i]; j++) {
      if (pre->hand[i][j] != post->hand[i][j]) return 4;
    }
  }

  //Compare player decks
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->deckCount[i] != post->deckCount[i]) return 5;
    for (j = 0; j < pre->deckCount[i]; j++) {
      if (pre->deck[i][j] != post->deck[i][j]) return 5;
    }
  }

  //Compare player discard piles
  for (i = 0; i < pre->numPlayers; i++) {
    if (pre->discardCount[i] != post->discardCount[i]) return 6;
    for (j = 0; j < pre->discardCount[i]; j++) {
      if (pre->discard[i][j] != post->discard[i][j]) return 6;
    }
  }

  //Compare played card piles
  if (pre->playedCardCount != post->playedCardCount) return 7;
  for (i = 0; j < pre->playedCardCount; i++) {
    if (pre->playedCards[i] != post->playedCards[i]) return 7;
  }

  if (pre->outpostPlayed != post->outpostPlayed) return 8;
  if (pre->outpostTurn != post->outpostTurn) return 9;
  if (pre->whoseTurn != post->whoseTurn) return 10;
  if (pre->phase != post->phase) return 11;
  if (pre->numActions != post->numActions) return 12;
  if (pre->coins != post->coins) return 13;
  if (pre->numBuys != post->numBuys) return 14;

  return 0;
}

int randomTreasure() {
  // Choose from copper, silver, gold
  int num_treasures = 3;
  int r;
  do {
    r = floor(Random() * num_treasures);
    assert(r != num_treasures);
  } while(r >= num_treasures);
  if ( r == 0 ) return copper;
  if ( r == 1 ) return silver;
  // if ( r == 2 ) return gold;
  else {
    return copper;
  }
}
