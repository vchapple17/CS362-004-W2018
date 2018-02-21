CFLAGS = -Wall -std=c99 -fpic -coverage -lm

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)	
	
unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)	
	
unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)	
	
unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)	
	
cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)	

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)	

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)	

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)	

randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)	
	
randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)	
	
randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer -lgomp -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)	
	
	
runUnitTests: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 
	
	./unittest1 &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	
	./unittest2 &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out 
	
	./unittest3 &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	
	./unittest4 &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	
	./cardtest1 &>> cardtestresult.out
	gcov dominion.c >> cardtestresult.out
	cat dominion.c.gcov >> cardtestresult.out
	
	./cardtest2 &>> cardtestresult.out
	gcov dominion.c >> cardtestresult.out
	cat dominion.c.gcov >> cardtestresult.out
	
	./cardtest3 &>> cardtestresult.out
	gcov dominion.c >> cardtestresult.out
	cat dominion.c.gcov >> cardtestresult.out
	
	./cardtest4 &>> cardtestresult.out
	gcov dominion.c >> cardtestresult.out
	cat dominion.c.gcov >> cardtestresult.out
	
runRandomTests: randomtestcard1 randomtestcard2 randomtestadventurer
	
	./randomtestcard1 &>> randomtestresult.out
	gcov dominion.c >> randomtestresult.out
	cat dominion.c.gcov >> randomtestresult.out
	
	./randomtestcard2 &>> randomtestresult.out
	gcov dominion.c >> randomtestresult.out
	cat dominion.c.gcov >> randomtestresult.out 
	
	./randomtestadventurer &>> randomtestresult.out
	gcov dominion.c >> randomtestresult.out
	cat dominion.c.gcov >> randomtestresult.out 