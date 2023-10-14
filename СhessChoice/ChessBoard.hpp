#pragma once
#ifndef ChessBoard_h
#define ChessBoard_h

#include "ChessActor.hpp"
#include <set>
#include <random>

/// Represent a chessboard
class ChessBoard {
public:
    friend class ChessBestChoice;
    
    typedef ChessActorColor Color;
    typedef ChessActorName Name;
    
    /// Initialize a chessboard
    /// - Parameters:
    ///     pieces: 8x8 vector representation of a default chessboard
    /// - Returns: void
    ///     it will cause a fatal error if vector is not 8x8in size
    
    ChessBoard(const std::vector<std::vector<ChessActor>> &pieces = std::vector<std::vector<ChessActor>>()) {
        if (pieces.size() == 8) {
            chessPieces = pieces;
        } else {
            chessPieces.resize(8);
            
            /// appending pawns
            for (int index = 0; index < 8; index++) {
                chessPieces[1].push_back(ChessActor(pawn, black));
                chessPieces[6].push_back(ChessActor(pawn, white));
            }
            
            /// appending other pieces
            for (int piece = rook; piece <= king; piece++) {
                chessPieces[0].push_back(ChessActor(Name(piece), black));
            }
            for (int piece = bishop; piece >= rook; piece--) {
                chessPieces[0].push_back(ChessActor(Name(piece), black));
            }
            
            for (int piece = rook; piece <= king; piece++) {
                chessPieces[7].push_back(ChessActor(Name(piece), white));
            }
            for (int piece = bishop; piece >= rook; piece--) {
                chessPieces[7].push_back(ChessActor(Name(piece), white));
            }
            
            /// appending empty pieces
            for (int row = 2; row < 6; row++) {
                for (int index = 0; index < 8; index++) {
                    chessPieces[row].push_back(ChessActor());
                }
            }
        }
    }
    
    static ChessBoard emptyBoard() {
        std::vector<std::vector<ChessActor>> emptyPieces;
        for (int row = 0; row < 8; row++) {
            emptyPieces.push_back(std::vector<ChessActor>());
            for (int col = 0; col < 8; col++) {
                emptyPieces[row].push_back(ChessActor());
            }
        }
        return ChessBoard(emptyPieces);
    }
    
    
    /// Display the current board
    /// - Returns: String description of the current board
    void description(std::vector<int> from = {-1}) {
        for (int row = 0; row < 8; row++) {
            
            
            if (row == 0) {
                std::cout << " ";
                for (int index = 0; index < 8; index++) {
                    std::cout << " " << index << "  ";
                }
                std::cout << '\n';
            }
            
            std::cout << "–";
            for (int index = 0; index < 8; index++) {
                std::cout << "––––";
            } std::cout << '\n';
            
            for (int col = 0; col < 8; col++) {
                std::cout << "|";
                
                if (from[0] != -1 && from[0] == row && from[1] == col) {
                    std::cout << " * ";
                } else {
                    std::cout << chessPieces[row][col].name();
                }
            }
            std::cout << "|" << " " << row << '\n' ;
        }
        std::cout << "–";
        for (int index = 0; index < 8; index++) {
            std::cout << "––––";
        } std::cout << '\n';
    }
    
    /// Generate random chessboard by relying on the current one
    /// - Returns: new generated chessboard
    static ChessBoard randomBoard(const std::vector<int> &pawnCount,
                           const std::vector<int> &bishopCount,
                           const std::vector<int> &knightCount,
                           const std::vector<int> &queenCount,
                           const std::vector<int> &rookCount) {
        ChessBoard newBoard = ChessBoard::emptyBoard();
        
        std::vector<ChessActor> whitePieces;
        std::vector<ChessActor> blackPieces;
        
        pushBlackWhitePieces(pawnCount, pawn, whitePieces, blackPieces);
        pushBlackWhitePieces(bishopCount, bishop, whitePieces, blackPieces);
        pushBlackWhitePieces(knightCount, knight, whitePieces, blackPieces);
        pushBlackWhitePieces(queenCount, queen, whitePieces, blackPieces);
        pushBlackWhitePieces(rookCount, rook, whitePieces, blackPieces);
        pushBlackWhitePieces({1, 1}, king, whitePieces, blackPieces);
        
        shufflePieces(whitePieces, blackPieces);
        placePieces(newBoard, whitePieces, blackPieces);
        
        return newBoard;
    }
    
    std::vector<std::vector<ChessActor>> pieces() {
        return chessPieces;
    }
    
    std::vector<std::vector<int>> findPossibleMoves(Color color) {
        std::vector<std::vector<int>> possibleMoves;
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (chessPieces[row][col].color() != color) { continue; }
                switch (chessPieces[row][col].idName()) {
                    case pawn:
                        findPossibleMovesForPawn(possibleMoves, row, col, color);
                        break;
                    case rook:
                        findPossibleMovesForRook(possibleMoves, row, col, color);
                        break;
                    case knight:
                        findPossibleMovesForKnight(possibleMoves, row, col, color);
                        break;
                    case bishop:
                        findPossibleMovesForBishop(possibleMoves, row, col, color);
                        break;
                    case queen:
                        findPossibleMovesForQueen(possibleMoves, row, col, color);
                        break;
                    case king:
                        findPossibleMovesForKing(possibleMoves, row, col, color);
                        break;
                    default:
                        break;
                }
            }
        }
        return possibleMoves;
    }
    
private:
    std::vector<std::vector<ChessActor>> chessPieces;
    
    void findPossibleMovesForKing(std::vector<std::vector<int>> &moves, int row, int col, Color color) {
        int kingMoves[8][2] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
        
        for (int index = 0; index < 8; index++) {
            int newRow = row + kingMoves[index][0];
            int newCol = col + kingMoves[index][1];
            
            if (newRow >= 0 && newRow <= 7 &&
                newCol >= 0 && newCol <= 7) {
                int ignoreRowCol[2] = {row, col};
                if (chessPieces[newRow][newCol].color() != color &&
                    !ChessBoard::isSideBySideKings(*this, newRow, newCol, ignoreRowCol)) {
                    moves.push_back({row, col, newRow, newCol});
                }
            }
        }
    }
    
    void findPossibleMovesForQueen(std::vector<std::vector<int>> &moves, int row, int col, Color color) {
        findPossibleMovesForBishop(moves, row, col, color);
        findPossibleMovesForRook(moves, row, col, color);
    }
    
    void findPossibleMovesForBishop(std::vector<std::vector<int>> &moves, int row, int col, Color color) {
        /// up-left
        for (int index = 1; row - index >= 0 && col - index >= 0; index++) {
            if (chessPieces[row - index][col - index].color() == color) {
                break;
            }
            moves.push_back({row, col, row - index, col - index});
            if (chessPieces[row - index][col - index].color() == ( (color == white) ? black : white )) { break; }
        }
        /// up-right
        for (int index = 1; row - index >= 0 && col + index <= 7; index++) {
            if (chessPieces[row - index][col + index].color() == color) {
                break;
            }
            moves.push_back({row, col, row - index, col + index});
            if (chessPieces[row - index][col + index].color() == ( (color == white) ? black : white )) { break; }
        }
        /// down-left
        for (int index = 1; row + index <= 7 && col - index >= 0; index++) {
            if (chessPieces[row + index][col - index].color() == color) {
                break;
            }
            moves.push_back({row, col, row + index, col - index});
            if (chessPieces[row + index][col - index].color() == ( (color == white) ? black : white )) { break; }
        }
        /// down-right
        for (int index = 1; row + index <= 7 && col + index <= 7; index++) {
            if (chessPieces[row + index][col + index].color() == color) {
                break;
            }
            moves.push_back({row, col, row + index, col + index});
            if (chessPieces[row + index][col + index].color() == ( (color == white) ? black : white )) { break; }
        }
    }
    
    void findPossibleMovesForKnight(std::vector<std::vector<int>> &moves, int row, int col, Color color) {
        int knightMoves[8][2] = { {-2, -1}, { -2, 1 }, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {-1, 2}, {1, 2} };
        
        for (int index = 0; index < 8; index++) {
            int newRow = row + knightMoves[index][0];
            int newCol = col + knightMoves[index][1];
            
            if (newRow >= 0 && newRow <= 7 &&
                newCol >= 0 && newCol <= 7) {
                if (chessPieces[newRow][newCol].color() != color) {
                    moves.push_back({row, col, newRow, newCol});
                }
            }
        }
    }
    
    void findPossibleMovesForRook(std::vector<std::vector<int>> &moves, int row, int col, Color color) {

        for (int index = row - 1; index >= 0; index--) {
            if (chessPieces[index][col].color() == color) {
                break;
            }
            moves.push_back({row, col, index, col});
            if (chessPieces[index][col].color() == ( (color == white) ? black : white )) { break; }
        }
        for (int index = row + 1; index <= 7; index++) {
            if (chessPieces[index][col].color() == color) {
                break;
            }
            moves.push_back({row, col, index, col});
            if (chessPieces[index][col].color() == ( (color == white) ? black : white )) { break; }
        }
        
        for (int jndex = col - 1; jndex >= 0; jndex--) {
            if (chessPieces[row][jndex].color() == color) {
                break;
            }
            moves.push_back({row, col, row, jndex});
            if (chessPieces[row][jndex].color() == ( (color == white) ? black : white )) { break; }
        }
        for (int jndex = col + 1; jndex <= 7; jndex++) {
            if (chessPieces[row][jndex].color() == color) {
                break;
            }
            moves.push_back({row, col, row, jndex});
            if (chessPieces[row][jndex].color() == ( (color == white) ? black : white )) { break; }
        }
    }
    
    void findPossibleMovesForPawn(std::vector<std::vector<int>> &moves, int row, int col, Color color) {
        if (color == white) {
            if (row - 1 < 0) {
                return;
            }
            if (chessPieces[row - 1][col].idName() == empty) {
                moves.push_back({row, col, row - 1, col});
            }
            if (col - 1 >= 0 && chessPieces[row - 1][col - 1].color() == black) {
                moves.push_back({row, col, row - 1, col - 1});
            }
            if (col + 1 <= 7 && chessPieces[row - 1][col + 1].color() == black) {
                moves.push_back({row, col, row - 1, col + 1});
            }
        } else if (color == black) {
            if (row + 1 > 7) {
                return;
            }
            if (chessPieces[row + 1][col].idName() == empty) {
                moves.push_back({row, col, row + 1, col});
            }
            if (col - 1 >= 0 && chessPieces[row + 1][col - 1].color() == white) {
                moves.push_back({row, col, row + 1, col - 1});
            }
            if (col + 1 <= 7 && chessPieces[row + 1][col + 1].color() == white) {
                moves.push_back({row, col, row + 1, col + 1});
            }
        }
    }
    
    static void placePieces(ChessBoard &board, std::vector<ChessActor> &whiteVector, std::vector<ChessActor> &blackVector) {
        std::vector<std::set<int>> boardIndices;
        for (int row = 0; row < 8; row++) {
            boardIndices.push_back(std::set<int>());
            for (int col = 0; col < 8; col++) {
                boardIndices[row].insert(col);
            }
        }
        
        randomAndPlace(board, whiteVector, boardIndices);
        randomAndPlace(board, blackVector, boardIndices);
    }
    
    static void randomAndPlace(ChessBoard &board, std::vector<ChessActor> &pieces, std::vector<std::set<int>> &indices) {
        while (pieces.empty() == false) {
            std::random_device device;
            std::mt19937 mtRandom(device());
            std::uniform_int_distribution<int> distribution(0, (int)(indices.size() - 1));
            int randomRow = distribution(mtRandom);
            
            mtRandom = std::mt19937(device());
            distribution = std::uniform_int_distribution<int>(0, (int)(indices[randomRow].size() - 1));
            auto colBegin = indices[randomRow].begin();
            std::advance(colBegin, distribution(mtRandom));
            int randomCol = *colBegin;
            
            ChessActor piece = pieces.back();
            if (piece.idName() == king && isSideBySideKings(board, randomRow, randomCol)) {
                continue;
            }
            if (randomRow == 0 && piece.color() == white && piece.idName() == pawn) {
//                std::cout << "oh white\n";
                continue;
            }
            if (randomRow == 7 && piece.color() == black && piece.idName() == pawn) {
//                std::cout << "oh black\n";
                continue;
            }
            
            board.chessPieces[randomRow][randomCol] = piece;
            pieces.pop_back();
            
            indices[randomRow].erase(randomCol);
            if (indices[randomRow].empty()) {
                std::vector<std::set<int>> tmp;
                for (int index = 0; index < indices.size(); index++) {
                    if (index == randomRow) { continue; }
                    tmp.push_back(indices[index]);
                }
                indices = tmp;
            }
        }
    }
    
    static bool isSideBySideKings(ChessBoard &board, int row, int col, const int *ignoreRowCol = nullptr) {
        int kingMoves[8][2] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
        
        for (int index = 0; index < 8; index++) {
            int newRow = row + kingMoves[index][0];
            int newCol = col + kingMoves[index][1];
            
            if (ignoreRowCol != nullptr && ignoreRowCol[0] == newRow && ignoreRowCol[1] == newCol) {
                continue;
            }
            
            if (newRow >= 0 && newRow <= 7 &&
                newCol >= 0 && newCol <= 7) {
                if (board.chessPieces[newRow][newCol].idName() == king) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    static void shufflePieces(std::vector<ChessActor> &whiteVector, std::vector<ChessActor> &blackVector) {
        std::mt19937 mtRandom((unsigned int)time(nullptr));
        std::shuffle(whiteVector.begin(), whiteVector.end(), mtRandom);
        mtRandom = std::mt19937((unsigned int)time(nullptr));
        std::shuffle(blackVector.begin(), blackVector.end(), mtRandom);
    }
    
    static void pushBlackWhitePieces(const std::vector<int> &repeatCounts, Name name, std::vector<ChessActor> &whiteVector, std::vector<ChessActor> &blackVector) {
        for (int count = 0; count < repeatCounts[0]; count++) {
            whiteVector.push_back(ChessActor(name, white));
        }
        for (int count = 0; count < repeatCounts[1]; count++) {
            blackVector.push_back(ChessActor(name, black));
        }
    }
};

#endif /* ChessBoard_h */
