/*******************************************************************
* Name: randomtestadventurer.c
* Author: Matt Nutsch
* Date: 2-11-2018
* Description:
* This code randomly tests the function adventurer_card().
*
********************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <assert.h>
#include "rngs.h"
#include <omp.h> // for tracking run time
#include <stdio.h>

int main() 
{
	int returnValue = NULL; //used to store return value of functions
	int tempArrayPosition = 0; //used in randomly selecting cards
	int loopCounter = 0; //used to keep track of test loop iterations
	int timesToLoop = 25; //set this to control how many random tests to perform
	int numberOfErrorsFound = 0;
	int handPositionCardTesting = 0; //used to track the position in the hand of the card being tested
	
	int i; //used as a count
    int seed = 1000;
    int numPlayer = 5; //assumes a maximum of 5 players
    struct gameState G;
	
	int p, r, handCount;
    int bonus;
	
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
	
	int maxHandCount = 5;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
    }
	
	int continueLoop = 1;
	
	double start_time; //for tracking run time
	double end_time; //for tracking run time
	double actual_time_sec; //for calculating the difference in run time
	
	int deckCount = 0;
	int initActions = 0;
	int maxCardsInHand = 3;
	int bonusPlaceholder = 0;
	int currentPlayer = 0;
	int startingCardCount = 0;
	int temphandPlaceholder = 0;
	
	start_time = omp_get_wtime(); //record the start time

	printf ("TESTING adventurer_card():\n");
	
	while(continueLoop == 1)
	{
		printf("Performing test # %d.\n", loopCounter);
	
		/**********************************************************************************/
		
		//An over view of the random generator structure
		
		//1. Identify the method under test/ (CUT)
			//adventurer_card()
			//this card will draw treasure from the discard pile and put them in the player's hand
		
		//2. Identify all the dependencies (parameters)
			//reset the game state
		
			//values to modify 
			//int currentPlayer
			//handCount
			//numActions
			
		//3. Write code to generate random inputs for the chosen method
			//If the input is a primitive data type, generate a random primitive value, etc.
			//If the input is an array, create an array and initialize it with some random values, etc.
			
			handCount = maxHandCount;
	
			memset(&G, 23, sizeof(struct gameState));   // clear the game state
			r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
			
			//randomly select which player to check
			
			//select the current player randomly
			currentPlayer = rand () % 4;
			printf("currentPlayer == %d.\n", currentPlayer);
			
			//initialize the played card count and played cards var, so that the game engine will not fail during testing
			G.playedCardCount = 0;
			G.playedCards[0] = 0;
			
			//put cards in the player's deck
			deckCount = rand () % 20;
			G.deckCount[currentPlayer] = deckCount;
			tempArrayPosition = 0;
			for (i = 0; i < (deckCount); i++)
			{
				tempArrayPosition = rand () % 10;
				G.deck[currentPlayer][i] = k[tempArrayPosition]; 
				printf("deck card # %d == %d.\n", i, G.deck[currentPlayer][i]);
			}
			//add a copper card as the first card in the deck
			G.deck[currentPlayer][0] = copper; 
			printf("update: deck card # %d == %d.\n", 0, G.deck[currentPlayer][0]);
			
			/***/
			
			//randomly select starting cards for that player's hand
			
			//select a random number of starting cards for the player's hand
			maxCardsInHand = 3;
			printf("maxCardsInHand == %d.\n", maxCardsInHand);
			startingCardCount = rand () % (maxCardsInHand - 1); //random up to 1 less than the hand count, so that we can also add the card being tested
			startingCardCount = startingCardCount + 1; //increment the card count by 1 for the card being tested
			printf("startingCardCount == %d.\n", startingCardCount);
			G.handCount[currentPlayer] = startingCardCount + 1; //start the player with a random number of cards plus the card being tested
			//draw random cards to fill the player's hand to set up the test
			tempArrayPosition = 0;
			for (i = 0; i < (startingCardCount - 1); i++)
			{
				tempArrayPosition = rand () % 10;
				G.hand[currentPlayer][i] = k[tempArrayPosition]; 
				printf("hand card # %d == %d.\n", i, k[i]);
			}
			
			/***/
			
			//set a random number of actions for the player
			initActions = rand () % 3; 
			printf("initActions == %d.\n", initActions);
			initActions = initActions + 1; //increment by 1 to ensure that there is at least one action to play the card
			G.numActions = initActions;
			printf("G.numActions == %d.\n", G.numActions);
			printf("Player %d's handcount == %d.\n", currentPlayer, G.handCount[currentPlayer]);
			printf("Player %d's actions == %d.\n", currentPlayer, G.numActions);

			//set the position that we will check for the copper card 
			handPositionCardTesting = G.handCount[currentPlayer];
			
		//4. Invoke the method (execute the method)
		
			//test the value
			printf("Calling the function...\n"); 
			returnValue = adventurer_card(0, &temphandPlaceholder, 0, 0, currentPlayer, 0, 0, 0, &G, 0, &bonusPlaceholder);
			printf("The return value == %d.\n", returnValue); 
			
			//check if the value is what is expected
			printf("Player %d's handcount == %d.\n", currentPlayer, G.handCount[currentPlayer]);
			//printf("Player %d's actions == %d.\n", currentPlayer, G.numActions);
			printf("handPositionCardTesting == %d.\n", handPositionCardTesting);
			printf("G.hand[currentPlayer][handPositionCardTesting] == %d.\n", G.hand[currentPlayer][handPositionCardTesting]);
			//checking for an error: confirming that the number of cards in hand is correct
			if(G.hand[currentPlayer][handPositionCardTesting] != (copper))
			{
				printf("Error found! The copper card did not get put in the player's hand.\n");
				numberOfErrorsFound++;
			}

		//5. Check if stopping criterion (like time or number of loops) is not satisfied go to step 2.
	
			loopCounter++; //increase the loopCounter;
			if(loopCounter >= timesToLoop)
			{
				printf("Reached the maximum number of tests.\n"); 
				continueLoop = 0;
			}
			
			end_time = omp_get_wtime();
			actual_time_sec = (double)(end_time - start_time); //calculate the elapsed time
			
			//stop the loop if it exceeds 60 seconds
			if(actual_time_sec >= 300)
			{
				continueLoop = 0;
			}
		
		/**********************************************************************************/
			
	}
	
    printf("Finished Testing.\nFound %d errors from %d tests.\n", numberOfErrorsFound, timesToLoop);

	printf("Elapsed time is %.f seconds.\n", actual_time_sec);
	
    return 0;
}

