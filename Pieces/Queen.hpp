//
//  Queen.hpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Queen_hpp
#define Queen_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class Queen : public Piece
{
private:
    /** Represents all the direction the piece can make */
    Position Moves[8] = { Position(1,0),
        Position(-1, 0),
        Position(0,1),
        Position(0,-1),
        Position(1,1),
        Position(1,-1),
        Position(-1,-1),
        Position(-1,1) };
    
public:
    Queen(const Color & color, const PieceType pieceType = PieceType::queen, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~Queen() {};
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
   // d virtual bool CanMove(const Position & from, const Position & to);
    virtual void Set_AlreadyMoved(bool moved = true);
    
    virtual std::vector<Position> MoveSet();
};
#endif /* Queen_hpp */
