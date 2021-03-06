CC=gcc-8
LIBS=`pkg-config --libs gtk+-3.0 librsvg-2.0`
GLIB_LIBS=`pkg-config --libs glib-2.0 gio-2.0`
CFLAGS=-O3 -std=c99 -s

CFLAGS_GLIB=`pkg-config --cflags glib-2.0`
CGLAGS_GTK=`pkg-config --cflags gtk+-3.0`
CFLAGS_LIBRSVG=`pkg-config --cflags librsvg-2.0`


run: chess
	./chess

chess: main.o application.o notebook.o page_home.o page_engine.o fen_box.o uci_engine.o chessboard.o engine_box.o libchess.a
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

main.o: src/main.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

notebook.o: src/notebook/notebook.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

page_home.o: src/notebook/page_home.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

page_engine.o: src/notebook/page_engine.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

uci_engine.o: src/engine/uci_engine.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

application.o: src/application/application.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

chessboard.o: src/application/chessboard.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK) $(CFLAGS_LIBRSVG)

fen_box.o: src/application/fen_box.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

engine_box.o: src/application/engine_box.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CGLAGS_GTK)

# Lib Chess
##################################################
libchess.a: board.o game.o moves.o pgn.o
	ar rcs $@ $^

board.o: src/chess/board.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

game.o: src/chess/game.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

moves.o: src/chess/moves.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

pgn.o: src/chess/pgn.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

openning.o: src/chess/openning.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)
##################################################


test: test_board test_moves test_pgn
	./test_board && ./test_moves && ./test_pgn

test_board: test_board.o board.o moves.o
	$(CC) $(CFLAGS) $^ -o $@ $(GLIB_LIBS)

test_board.o: test/board_test.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

test_moves: test_moves.o board.o moves.o
	$(CC) $(CFLAGS) $^ -o $@ $(GLIB_LIBS)

test_moves.o: test/moves_test.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

test_pgn: test_pgn.o pgn.o board.o moves.o game.o
	$(CC) $(CFLAGS) $^ -o $@ $(GLIB_LIBS)

test_pgn.o: test/pgn_test.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)


play: playground
	./playground

playground: playground.o libchess.a
	$(CC) $(CFLAGS) $^ -o $@ $(GLIB_LIBS)

playground.o: test/playground.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CFLAGS_GLIB)

clean:
	rm -f chess playground test_* *.o *.a