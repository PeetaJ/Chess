//
//  Position.hpp
//  Chess
//
//  Created by Petr Janousek on 05/05/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Position_hpp
#define Position_hpp

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>

/** Represents the single position on the 2D board */
class Position
{
public:
    Position();
    
    /** constructor for Position
     @param[in] height The height of the Position on board.
     @param[in] width The width of the Position on board.
     */
    Position(int height, int width) : height(height), width(width) {};
    int height;
    int width;
    
    /** Overloaded operator + that adds two positions. */
    Position operator + (const Position & pos) const;
    
    /** Overloaded operator == that checks the 2 positions and returns if true they are the same. */
    bool operator == (const Position & pos) const;
    
    /** Overloaded operator < that checks if one of the positions is smaller than the second one */
    bool operator < (const Position & pos) const;
};

#endif /* Position_hpp */
