//
//  Rook.cpp
//  Chess
//
//  Created by Petr Janousek on 25/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Rook.hpp"


void Rook :: print() const
{
    if (color == Color::black)
    {
        printf("\x1B[31mR\033[0m");
    }
    if (color == Color::white)
        std :: cout << "R";
}

std::shared_ptr<Piece> Rook :: clone() const
{
    return std::make_shared<Rook>(*this);
}

std::vector<Position> Rook::MoveSet()
{
    std::vector<Position> moveSet;
    
    for ( auto & move : Moves )
    {
        Position tmp = move;
        while( abs(tmp.height) != 8 && abs(tmp.width) != 8 )
        {
            moveSet.push_back(tmp);
            tmp = tmp + move;
        }
    }
    
    return moveSet;
}

void Rook :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
