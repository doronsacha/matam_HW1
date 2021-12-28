COMP_FLAG = -std=c99 -Wall -Werror
EXEC= chess
CC = gcc


$(EXEC): chess.o test_example.o player.o tournament.o game.o statistic.o map.o
	gcc -o $(EXEC) $(COMP_FLAG) chess.o test_example.o player.o tournament.o game.o statistic.o map.o

test_example.o: ./tests/chessSystemTestsExample.c chessSystem.h test_utilities.h map.h
	$(CC) -c $(COMP_FLAG) ./tests/chessSystemTestsExample.c
	mv chessSystemTestsExample.o test_example.o

chess.o: chessSystem.c chessSystem.h chessDeclaration.h player.h tournament.h
	$(CC) -c $(COMP_FLAG) ./chessSystem.c
	mv chessSystem.o chess.o

tournament.o: tournament.c tournament.h chessSystem.h player.h map.h
	$(CC) -c $(COMP_FLAG) ./tournament.c

player.o:  player.c chessDeclaration.h player.h game.h tournament.h map.h statistic.h
	$(CC) -c $(COMP_FLAG) ./player.c

game.o: game.c game.h chessSystem.h player.h map.h tournament.h statistic.h
	$(CC) -c $(COMP_FLAG) ./game.c

statistic.o: statistic.c statistic.h chessDeclaration.h player.h tournament.h chessSystem.h game.h map.h
	$(CC) -c $(COMP_FLAG) ./statistic.c

map.o: map.c map.h
	$(CC) -c $(COMP_FLAG) map.c



clean:
	rm -f $(OBJS) $(EXEC)






