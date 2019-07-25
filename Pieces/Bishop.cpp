//
//  Bishop.cpp
//  Chess
//
//  Created by Petr Janousek on 04/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#include "Bishop.hpp"


void Bishop :: print() const
{
    if (color == Color::black)
        printf("\x1B[31mB\033[0m");
    else
        std::cout << "B";
}
std::shared_ptr<Piece> Bishop :: clone() const
{
    return std:: make_shared<Bishop>(*this);
}

std::vector<Position> Bishop::MoveSet()
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

void Bishop :: Set_AlreadyMoved(bool moved)
{
    alreadyMoved = moved;
}
