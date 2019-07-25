//
//  Piece.hpp
//  Chess
//
//  Created by Petr Janousek on 23/04/2019.
//  Copyright © 2019 Petr Janousek. All rights reserved.
//

#ifndef Piece_hpp
#define Piece_hpp

#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <vector>
#include "Position.hpp"

/** enum for the two possible colors (sides)*/
enum class Color {white, black};

/** enum for all the possible types of pieces */
enum class PieceType {king, queen, rook, bishop, horse, pawn};

/** Abstract class representing the pieces on baord */
class Piece
{
protected:
public:
    Color color; /*!< color of the piece */
    PieceType pieceType; /*!< type of the piece */
    bool alreadyMoved; /*!< if the piece was moved this game */
    
    /** constructor for abstract class piece.
     @param[in] color The color that piece will be.
     @param[in] pieceType The type of piece it will be.
     @param[in] alreadyMoved = false The bool if the piece was moved.
     */
    Piece(const Color & color, const PieceType pieceType, const bool alreadyMoved = false) : color(color), pieceType(pieceType), alreadyMoved(alreadyMoved) {};
    virtual ~Piece() {};
    
    /** Clones the piece of type so it can be used in vector. */
    virtual std:: shared_ptr<Piece> clone() const = 0;
    
    /** Prints the character assigned to indicate the type of piece on board. */
    virtual void print() const = 0;
    virtual void Set_AlreadyMoved(bool moved = true) = 0;
    
    /** Returns vector of the possible directions piecetype can move to. */
    virtual std::vector<Position> MoveSet() = 0;
};

#endif /* Piece_hpp */
