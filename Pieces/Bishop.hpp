//
//  Bishop.hpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Bishop_hpp
#define Bishop_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class Bishop : public Piece
{
private:
    /** Represents all the direction the piece can make */
    Position Moves[4] = { Position(1,1),
        Position(1, -1),
        Position(-1,1),
        Position(-1,-1)
    };
    
public:
    Bishop(const Color & color, const PieceType pieceType = PieceType::bishop, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~Bishop() {};
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
    //virtual bool CanMove(const Position & from, const Position & to);
    virtual void Set_AlreadyMoved(bool moved = true);
    
    virtual std::vector<Position> MoveSet();
};

#endif /* Bishop_hpp */
