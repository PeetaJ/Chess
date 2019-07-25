//
//  King.hpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef King_hpp
#define King_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class King : public Piece
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
    King(const Color & color, const PieceType pieceType = PieceType::king, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~King() {};
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
    virtual void Set_AlreadyMoved(bool moved = true);
    
    virtual std::vector<Position> MoveSet();
};
#endif /* King_hpp */
