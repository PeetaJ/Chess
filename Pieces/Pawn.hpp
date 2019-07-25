//
//  Pawn.hpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Pawn_hpp
#define Pawn_hpp

#include "../Piece.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

class Pawn : public Piece
{
public:
    Pawn(const Color & color, const PieceType pieceType = PieceType::pawn, const bool alreadyMoved = false) : Piece(color, pieceType, alreadyMoved) {};
    virtual ~Pawn() {};
    virtual void print() const;
    virtual std::shared_ptr<Piece> clone() const;
   // virtual bool CanMove(const Position & from, const Position & to);
    virtual void Set_AlreadyMoved(bool moved = true);
    
    bool Get_AlreadyMoved();
    
    virtual std::vector<Position> MoveSet();
};

#endif /* Pawn_hpp */
