CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

myAssert.o: myAssert.h myAssert.c
	gcc -c myAssert.c -g $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

randomtesthelper.o: randomtesthelper.h randomtesthelper.c
	gcc -c randomtesthelper.c -g $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)



# My Unit tests
unittest1: unittest1.c dominion.o rngs.o myAssert.o
	gcc  -o unittest1 -g  unittest1.c dominion.o rngs.o myAssert.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o myAssert.o
	gcc  -o unittest2 -g  unittest2.c dominion.o rngs.o myAssert.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o myAssert.o
	gcc  -o unittest3 -g  unittest3.c dominion.o rngs.o myAssert.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o myAssert.o
	gcc  -o unittest4 -g  unittest4.c dominion.o rngs.o myAssert.o $(CFLAGS)

# My Card tests
cardtest1: cardtest1.c dominion.o rngs.o myAssert.o
	gcc  -o cardtest1 -g  cardtest1.c dominion.o rngs.o myAssert.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o myAssert.o
	gcc  -o cardtest2 -g  cardtest2.c dominion.o rngs.o myAssert.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o myAssert.o
	gcc  -o cardtest3 -g  cardtest3.c dominion.o rngs.o myAssert.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o myAssert.o
	gcc  -o cardtest4 -g  cardtest4.c dominion.o rngs.o myAssert.o $(CFLAGS)

# My Random Card tests
randomtestcard1: randomtestcard1.c dominion.o rngs.o myAssert.o randomtesthelper.o
	gcc  -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o myAssert.o randomtesthelper.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o myAssert.o randomtesthelper.o
	gcc  -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o myAssert.o randomtesthelper.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o myAssert.o randomtesthelper.o
	gcc  -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o myAssert.o randomtesthelper.o $(CFLAGS)

run1: unittest1
	./unittest1
	gcov dominion.c
	gcov -abcfu dominion.c

run2: unittest2
	./unittest2
	gcov dominion.c
	gcov -abcfu dominion.c

run3: unittest3
	./unittest3
	gcov dominion.c
	gcov -abcfu dominion.c

run4: unittest4
	./unittest4
	gcov dominion.c
	gcov -abcfu dominion.c

run1c: cardtest1
	./cardtest1
	gcov dominion.c
	gcov -abcfu dominion.c

run2c: cardtest2
	./cardtest2
	gcov dominion.c
	gcov -abcfu dominion.c

run3c: cardtest3
	./cardtest3
	gcov dominion.c
	gcov -abcfu dominion.c

run4c: cardtest4
	./cardtest4
	gcov dominion.c
	gcov -abcfu dominion.c

run1r: randomtestcard1
	./randomtestcard1
	gcov dominion.c
	gcov -abcfu dominion.c

run2r: randomtestcard2
	./randomtestcard2
	gcov dominion.c
	gcov -abcfu dominion.c

run3r: randomtestadventurer
	./randomtestadventurer
	gcov dominion.c
	gcov -abcfu dominion.c

unittestresults: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
	echo "UNIT TEST RESULTS" > unittestresult.out
	echo >> unittestresult.out
	echo "unittest1.c:" >> unittestresult.out
	./unittest1 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "unittest2.c:" >> unittestresult.out
	./unittest2 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "unittest3.c:" >> unittestresult.out
	./unittest3 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "unittest4.c:" >> unittestresult.out
	./unittest4 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "cardtest1.c:" >> unittestresult.out
	./cardtest1 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "cardtest2.c:" >> unittestresult.out
	./cardtest2 >> unittestresult.out
	echo >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo "cardtest3.c:" >> unittestresult.out
	./cardtest3 >> unittestresult.out
	gcov dominion.c >> unittestresult.out
	echo >> unittestresult.out
	echo "cardtest4.c:" >> unittestresult.out
	./cardtest4 >> unittestresult.out
	gcov dominion.c >> unittestresult.out

randomtestcard1out: randomtestcard1
	echo "RANDOM CARD #1 TEST RESULTS" > randomtestcard1.out
	echo >> randomtestcard1.out
	./randomtestcard1 >> randomtestcard1.out
	gcov dominion.c -abcfu >> randomtestcard1.out

randomtestcard2out: randomtestcard2
	echo "RANDOM CARD #2 TEST RESULTS" > randomtestcard2.out
	echo >> randomtestcard2.out
	./randomtestcard2 >> randomtestcard2.out
	gcov dominion.c -abcfu >> randomtestcard2.out

randomtestadventurerout: randomtestadventurer
	echo "RANDOM CARD #3 (adventurer) TEST RESULTS" > randomtestadventurer.out
	echo >> randomtestadventurer.out
	./randomtestadventurer >> randomtestadventurer.out
	gcov dominion.c -abcfu >> randomtestadventurer.out

randomout: randomtestcard1out randomtestcard2out randomtestadventurerout

# set noexpandtab
all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest2
