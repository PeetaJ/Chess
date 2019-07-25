//
//  Pawn.cpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Pawn.hpp"


void Pawn :: print() const
{
    if (color == Color::black)
    {
        printf("\x1B[31mP\033[0m");
    }
    if (color == Color::white)
        std :: cout << "P";
}

std::shared_ptr<Piece> Pawn :: clone() const
{
    return std::make_shared<Pawn>(*this);
}

std::vector<Position> Pawn::MoveSet()
{
    std::vector<Position> tmp;
    
    if (color == Color::white)
    {
        tmp.emplace_back(-1,0);
        if ( !alreadyMoved ) tmp.emplace_back(-2,0);
    }
    else
    {
        tmp.emplace_back(1,0);
        if ( !alreadyMoved ) tmp.emplace_back(2,0);
    }
    
    return tmp;
}

bool Pawn :: Get_AlreadyMoved()
{
    return alreadyMoved;
}

void Pawn :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
