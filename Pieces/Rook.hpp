//
//  Rook.hpp
//  Chess
//
//  Created by Petr Janousek on 25/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Rook_hpp
#define Rook_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class Rook : public Piece
{
private:
    /** Represents all the direction the piece can make */
    Position Moves[4] = { Position(1,0),
        Position(-1, 0),
        Position(0,1),
        Position(0,-1)
    };
    
public:
    Rook(const Color & color, const PieceType pieceType = PieceType::rook, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~Rook() {};
    
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
    //virtual bool CanMove(const Position & from, const Position & to);
    virtual void Set_AlreadyMoved(bool moved = true);
    
    virtual std::vector<Position> MoveSet();
};
#endif /* Rook_hpp */
