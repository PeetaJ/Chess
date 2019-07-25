//
//  Horse.hpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Horse_hpp
#define Horse_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class Horse : public Piece
{
private:
    /** Represents all the direction the piece can make */
    Position Moves[8] = { Position(2,1),
        Position(2, -1),
        Position(-2,1),
        Position(-2,-1),
        Position(1,-2),
        Position(1,2),
        Position(-1,-2),
        Position(-1,2),
    };
    
public:
    Horse(const Color & color, const PieceType pieceType = PieceType::horse, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~Horse() {};
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
  //  virtual bool CanMove(const Position & from, const Position & to);
    virtual void Set_AlreadyMoved(bool moved = true);
    
    virtual std::vector<Position> MoveSet();
};
#endif /* Horse_hpp */
