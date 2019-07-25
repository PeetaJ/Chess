TARGET_EXEC = janoup15
CXX := g++
CXXFLAGS := -g -c -std=c++14 -pedantic -Wall -Wextra

all: compile run

compile: $(TARGET_EXEC)

run:
	./$(TARGET_EXEC)

clean:
	rm -r *.o janoup15

doc:
	doxygen Doxyfile

$(TARGET_EXEC): main.o Board.o Game.o Player.o Piece.o King.o Rook.o Bishop.o Horse.o Pawn.o Queen.o Position.o Human.o Computer.o
	$(CXX) -g -std=c++14 main.o Board.o Game.o Player.o Piece.o King.o Rook.o Bishop.o Horse.o Pawn.o Queen.o Position.o Human.o Computer.o -o $@


Board.o: Board.cpp Board.hpp Player.hpp Piece_utility.hpp Piece.hpp Position.hpp Pieces/Rook.hpp Pieces/Horse.hpp Pieces/Bishop.hpp Pieces/Queen.hpp Pieces/King.hpp Pieces/Pawn.hpp Human.hpp
	$(CXX) $(CXXFLAGS) Board.cpp
Game.o: Game.cpp Game.hpp Board.hpp Player.hpp Piece_utility.hpp Piece.hpp Position.hpp Pieces/Rook.hpp Pieces/Horse.hpp Pieces/Bishop.hpp Pieces/Queen.hpp Pieces/King.hpp Pieces/Pawn.hpp Human.hpp Computer.hpp
	$(CXX) $(CXXFLAGS) Game.cpp
Piece.o: Piece.cpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Piece.cpp
Player.o: Player.cpp Player.hpp
	$(CXX) $(CXXFLAGS) Player.cpp
Position.o: Position.cpp Position.hpp
	$(CXX) $(CXXFLAGS) Position.cpp
main.o: main.cpp Board.hpp Player.hpp Piece_utility.hpp Piece.hpp Position.hpp Pieces/Rook.hpp Pieces/Horse.hpp Pieces/Bishop.hpp Pieces/Queen.hpp Pieces/King.hpp Pieces/Pawn.hpp Game.hpp Human.hpp Computer.hpp
	$(CXX) $(CXXFLAGS) main.cpp
Bishop.o: Pieces/Bishop.cpp Pieces/Bishop.hpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/Bishop.cpp
Horse.o: Pieces/Horse.cpp Pieces/Horse.hpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/Horse.cpp
King.o: Pieces/King.cpp Pieces/King.hpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/King.cpp
Pawn.o: Pieces/Pawn.cpp Pieces/Pawn.hpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/Pawn.cpp
Queen.o: Pieces/Queen.cpp Pieces/Queen.hpp Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/Queen.cpp
Rook.o: Pieces/Rook.cpp Pieces/Rook.hpp Pieces/../Piece.hpp Position.hpp
	$(CXX) $(CXXFLAGS) Pieces/Rook.cpp
Human.o: Human.cpp Human.hpp Player.hpp
	$(CXX) $(CXXFLAGS) Human.cpp
Computer.o: Computer.cpp Computer.hpp Player.hpp Position.hpp Board.hpp Piece_utility.hpp Piece.hpp Pieces/Rook.hpp Pieces/../Piece.hpp Pieces/Horse.hpp Pieces/Bishop.hpp Pieces/Queen.hpp Pieces/King.hpp Pieces/Pawn.hpp
	$(CXX) $(CXXFLAGS) Computer.cpp

