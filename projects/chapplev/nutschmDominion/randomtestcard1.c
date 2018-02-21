/*******************************************************************
* Name: randomtestcard1.c
* Author: Matt Nutsch
* Date: 2-11-2018
* Description:
* This code randomly tests the function village_card.
*
********************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h> // for tracking run time

int main() 
{
	srand(time(NULL)); //set the random seed
	
	int returnValue = NULL; //used to store return value of functions
	int tempArrayPosition = 0; //used in randomly selecting cards
	int loopCounter = 0; //used to keep track of test loop iterations
	int timesToLoop = 10000; //set this to control how many random tests to perform
	int numberOfErrorsFound = 0;

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
	
	handCount = maxHandCount;
	
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	
	int continueLoop = 1;
	
	int deckCount = 0;
	int initActions = 0;
	int maxCardsInHand = 5;
	int bonusPlaceholder = 0;
	int currentPlayer = 0;
	
	clock_t start_time; //for tracking run time
	clock_t end_time; //for tracking run time
	double actual_time_sec; //for calculating the difference in run time
	
	start_time = clock(); //record the start time
	
	printf ("TESTING village_card():\n");
	
	while(continueLoop == 1)
	{
		printf("Performing test # %d.\n", loopCounter);
	
		/**********************************************************************************/
		
		//An over view of the random generator structure
		
		//1. Identify the method under test/ (CUT)
			//village_card()
			//this card will draw a new card and increase the user's actions by 2
		
		//2. Identify all the dependencies (parameters)
			//reset the game state
		
			//values to modify 
			//int currentPlayer
			//handCount
			//numActions
			
		//3. Write code to generate random inputs for the chosen method
			//If the input is a primitive data type, generate a random primitive value, etc.
			//If the input is an array, create an array and initialize it with some random values, etc.
			
			//randomly select which player to check
			
			//select the current player randomly
			currentPlayer = rand () % 4;
			printf("currentPlayer == %d.\n", currentPlayer);
			
			
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
			/***/
			
			//randomly select starting cards for that player's hand
			
			//select a random number of starting cards for the player's hand
			
			printf("maxCardsInHand == %d.\n", maxCardsInHand);
			int startingCardCount = rand () % (maxCardsInHand - 1); //random up to 1 less than the hand count, so that we can also add the card being tested
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
			//add the village card as the last card in the hand
			G.hand[currentPlayer][i] = village; 
			printf("card # %d == %d.\n", i, k[i]);
			i++;
			
			/***/
			
			//set a random number of actions for the player
			initActions = rand () % 3; 
			printf("initActions == %d.\n", initActions);
			initActions = initActions + 1; //increment by 1 to ensure that there is at least one action to play the card
			G.numActions = initActions;
			printf("G.numActions == %d.\n", G.numActions);
			printf("Player %d's handcount == %d.\n", currentPlayer, G.handCount[currentPlayer]);
			printf("Player %d's actions == %d.\n", currentPlayer, G.numActions);
		
		//4. Invoke the method (execute the method)
		
			//test the value
			
			printf("Calling the function...\n"); 
			returnValue = village_card(currentPlayer, 0, 0, 0, &G, 0, &bonusPlaceholder); //int village_card(int currentPlayer, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
			printf("The return value == %d.\n", returnValue); 
			
			//check if the value is what is expected
			printf("Player %d's handcount == %d.\n", currentPlayer, G.handCount[currentPlayer]);
			printf("Player %d's actions == %d.\n", currentPlayer, G.numActions);
			//checking for an error: confirming that the number of cards in hand is correct
			if(G.handCount[currentPlayer] != (startingCardCount + 1))
			{
				printf("Error found! The number of cards in player's hand is not correct.\n");
				numberOfErrorsFound++;
			}
			//checking for an error: confirming that the number of actions is correct
			if(G.numActions != (initActions + 2))
			{
				printf("Error found! The number of actions is not correct.\n");
				numberOfErrorsFound++;
			}
		
		//5. Check if stopping criterion (like time or number of loops) is not satisfied go to step 2.
		
			loopCounter++; //increase the loopCounter;
			if(loopCounter >= timesToLoop)
			{
				printf("Reached the maximum number of tests.\n"); 
				continueLoop = 0;
			}
		
		end_time = clock();
		actual_time_sec = (double)(end_time - start_time) / CLOCKS_PER_SEC; //calculate the elapsed time
		
		//stop the loop if it exceeds 60 seconds
		if(actual_time_sec >= 300)
		{
			continueLoop = 0;
		}
		
		/**********************************************************************************/
		
	}
	
    printf("Finished Testing.\nFound %d errors from %d tests.\n", numberOfErrorsFound, timesToLoop);
	
	printf("Elapsed time is %f seconds.\n", actual_time_sec);

    return 0;
}