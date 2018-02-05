CFLAGS = -Wall -fpic -coverage -lm

# rngs.o: rngs.h rngs.c
# 	gcc -c rngs.c -g  $(CFLAGS)

testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)

test: testme
	./testme

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
