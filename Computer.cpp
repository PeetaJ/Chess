//
//  Computer.cpp
//  Chess
//
//  Created by Petr Janousek on 22/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Computer.hpp"
#include <chrono>
#include <thread>

void Computer :: Input(std::string & from, std::string & to, Board & board)
{
    std::pair<Position, Position> ChosenMove = std::pair<Position, Position>(Position(0,0), Position(0,0));
    
    if ( difficulty == 1 )
        ChosenMove = RandomMoves(board);
    if (difficulty == 2)
        ChosenMove = MinMaxMoves(board);
    
    from = MoveToString(ChosenMove.first);
    to = MoveToString(ChosenMove.second);
    
    // since computer actually doesnt write anything do cin, it has to be newlined for sake of the right format
    std::cout << std::endl;
}

std::shared_ptr<Player> Computer :: clone() const
{
    return std::make_shared<Computer>(*this);
}

int Computer :: Get_Difficulty() const
{
    return difficulty;
}

// ----------------------------------------------------------------

// PRIVATE FUNCTIONS
std::pair<Position, Position> Computer :: RandomMoves(Board & board)
{
    Color color = board.Get_Turn();

    std::vector<Position> piecesPos = board.findAllPiecesOf(color);

    std::vector< std::pair<Position, Position> > MovesToChooseFrom;

    for (const Position & from : piecesPos)
    {
        for( const Position & legalMove : board.Piece_GenerateLegalMoves(from) )
        {
            MovesToChooseFrom.push_back( std::pair<Position, Position>(from, legalMove) );
        }
    }

    int possibleMovesCount = (int)MovesToChooseFrom.size();
    int randomMove = std::rand() % possibleMovesCount + 1;
    
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::system_clock;
    sleep_for(0.5s);
    
    return MovesToChooseFrom.at(randomMove);
}

std::pair<Position, Position> Computer :: MinMaxMoves(Board & board)
{
    
    std::pair<Position, Position> bestMove = std::pair<Position, Position>(Position(0,0), Position(0,0));
    MinMax(3, board.Get_Turn() , true, board, bestMove);
    
    while (board.TryIfCheck(bestMove.first, bestMove.second) )
        MinMax(3, board.Get_Turn() , true, board, bestMove);
    return bestMove;
}

// ----------------------------------------------------------------

// HELPER FUNCTIONS

int Computer :: MinMax (int depth, const Color & playerColor, bool maximizingPlayer, Board & board, std::pair<Position, Position> & bestMove)
{
    if ( depth == 0 || board.Get_CheckMate() || board.GenerateLegalMoves(playerColor).size() == 0 )
    {
        Color color = playerColor == Color::white ? Color::white : Color::black;
        return EvaluateSideValue(playerColor, board) / EvaluateSideValue(color, board);
    }
    
    if ( maximizingPlayer )
    {
        int biggestValue = -10000; // Should be -infinity, but this value is big enough and so it's sufficient
        
        Color color = playerColor == Color::white ? Color::black : Color::white;
        
        //for ( auto & move : board.GenerateLegalMoves(playerColor) ) // randomize the LegalMoves positions in vector TODO
        std::vector< std::pair<Position, Position> > legalMoves = board.GenerateLegalMoves(playerColor);
        std::random_shuffle(legalMoves.begin(), legalMoves.end(), [](int i){ return std::rand() % i; } );
 
        for ( auto & move : legalMoves )
        {
            Board copiedBoard = board;
            copiedBoard.Move(move.first, move.second);
            int value = MinMax(depth - 1, color, false, copiedBoard, bestMove);
            biggestValue = biggestValue < value ? value : biggestValue;
            
            if (biggestValue == value && depth == 3)
                bestMove = move;
        }
        return biggestValue;
    }
    else
    {
        int smallestValue = 10000; // Should be infinity, but this value is big enough and so it's sufficient
        
        Color color = playerColor == Color::white ? Color::black : Color::white;
        
        std::vector< std::pair<Position, Position> > legalMoves = board.GenerateLegalMoves(playerColor);
        std::random_shuffle(legalMoves.begin(), legalMoves.end(), [](int i){ return std::rand() % i; } );
        for ( auto & move : legalMoves )
        {
            Board copiedBoard = board;
            copiedBoard.Move(move.first, move.second);
            int value = MinMax(depth - 1, color, true, copiedBoard, bestMove);
            
            smallestValue = smallestValue > value ? value : smallestValue;
            
            if ( smallestValue == value && depth == 3)
                bestMove = move;
        }
        return smallestValue;
    }
}


std::string Computer :: MoveToString(Position & pos)
{
    std::stringstream move;

    char letters[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    for (int i = 0; i < 8; i++)
    {
        if (i == pos.width)    move << letters[i];
    }

    move << pos.height + 1;

    return move.str();
}

int Computer :: EvaluateSideValue(const Color side, Board & board)
{
    int totalValue = 0;
    
    int pieceValue[6] = {1,3,3,5,9,50};
    PieceType pieceType[6] = {PieceType::pawn, PieceType::horse, PieceType::bishop, PieceType::rook, PieceType::queen, PieceType::king};
    
    for ( const Position & piece : board.findAllPiecesOf(side) )
    {
        for (int i = 0 ; i < 6 ; i ++)
        {
            if ( board.PieceAt(piece)->pieceType == pieceType[i] )
            {
                totalValue += pieceValue[i];
                break;
            }
        }
    }
    if ( side == Color::black )
        totalValue *= -1;
    
    return totalValue;
}

// ----------------------------------------------------------------
