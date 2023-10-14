#pragma once
#ifndef ChessActor_h
#define ChessActor_h

/// Color of a chess piece(figure)
enum ChessActorColor {
    white,
    black,
    none
};

/// Name of a chess piece(figure)
enum ChessActorName {
    rook,
    knight,
    bishop,
    queen,
    king,
    pawn,
    empty
};

/// Reperesent a chess piece(figure)
class ChessActor {
public:
    typedef ChessActorColor Color;
    typedef ChessActorName Name;
    
    /// Initialize the chess piece
    ChessActor(Name name = empty, Color color = none) :
    pieceName(name),
    pieceColor(color) { }
    
    std::string description() {
        switch (pieceName) {
            case king:
                return "King";
            case queen:
                return "Queen";
            case rook:
                return "Rook";
            case bishop:
                return "Bishop";
            case knight:
                return "Knight";
            case pawn:
                return "Pawn";
            case empty:
                return "Empty";
        }
    }
    
    std::string name() {
        switch (pieceName) {
            case knight:
            case king:
                return ((pieceColor == white) ? "w" : "b") + description().substr(0, 2);
            case empty:
                return "   ";
            default:
                return ((pieceColor == white) ? "w" : "b")+ description().substr(0, 2);
        }
    }
    
    Color color() {
        return pieceColor;
    }
    
    Name idName() {
        return pieceName;
    }
    
private:
    Name pieceName;
    Color pieceColor;
    
};

#endif /* ChessActor_h */
