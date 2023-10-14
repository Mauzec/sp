#pragma once
#ifndef ChessBestChoice_h
#define ChessBestChoice_h

#include "ChessBoard.hpp"

class ChessBestChoice {
public:
    std::vector<std::vector<int>> choice;
    
    int maxHeight = 1;
    
    int evaluate(ChessBoard &board, bool &isOnlyKing) {
        int score = 0;
        
        std::unordered_map<ChessActorName, int> pieceValues;
        pieceValues[pawn] = 1;
        pieceValues[knight] = 3;
        pieceValues[bishop] = 3;
        pieceValues[rook] = 5;
        pieceValues[queen] = 9;
        pieceValues[king] = 200;
        
        int kingCount = 0;
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                auto piece = board.chessPieces[row][col];
                if (piece.idName() != empty) {
                    if (piece.color() == white) {
                        score += pieceValues[piece.idName()];
                    } else {
                        score -= pieceValues[piece.idName()];
                    }
                }
                
                if (piece.idName() == king) {
                    kingCount += 1;
                }
            }
        }
        
        if (kingCount == 2) { isOnlyKing = false; }
        else { isOnlyKing = true; }
        
        return score;
    }
    
    int minimax(ChessBoard &board,
                bool isWhite = true, int height = 0) {
        bool isOnlyKing = false;
        int eval = evaluate(board, isOnlyKing);
        if (height == maxHeight || isOnlyKing) {
            return eval;
            
        }
        
        int bestScore = INT_MIN;
        if (isWhite) {
            std::vector<std::vector<int>> moves = board.findPossibleMoves(white);
            for (int index = 0; index < moves.size(); index++) {

                auto move = moves[index];
                    
                std::vector<std::vector<ChessActor>> newPieces = board.chessPieces;
                newPieces[move[0]][move[1]] = ChessActor();
                newPieces[move[2]][move[3]] = board.chessPieces[move[0]][move[1]];
                
                if ( move[2] == 0 && newPieces[move[2]][move[3]].idName() == pawn ) {
                    newPieces[move[2]][move[3]] = ChessActor(rook, white);
                }
                
                ChessBoard newBoard = ChessBoard(newPieces);
                
                int score = minimax(newBoard, false, height + 1);
                
                if (score > bestScore) {
                    bestScore = score;
                }
            }
        } else {
            bestScore = INT_MAX;
            std::vector<std::vector<int>> moves = board.findPossibleMoves(black);
            for (int index = 0; index < moves.size(); index++) {
                auto move = moves[index];
                
                std::vector<std::vector<ChessActor>> newPieces = board.chessPieces;
                newPieces[move[0]][move[1]] = ChessActor();
                newPieces[move[2]][move[3]] = board.chessPieces[move[0]][move[1]];
                
                if ( move[2] == 7 && newPieces[move[2]][move[3]].idName() == pawn ) {
                    newPieces[move[2]][move[3]] = ChessActor(rook, black);
                }
                
                ChessBoard newBoard = ChessBoard(newPieces);
                
                int score = minimax(newBoard, true, height + 1);
                if (score < bestScore) {
                    bestScore = score;
                }
            }
        }
        return bestScore;
    }
    /// Simple method to determine the best move based on position analysis
    void best(ChessBoard &board, std::vector<std::vector<int>> ignoreChoice, bool draw = true) {
        std::vector<int> bestChoice;
        for (int index = 0; index < 3; index++) {
            int bestScore = INT_MIN;
            
            std::vector<std::vector<int>> moves = board.findPossibleMoves(white);
            
            for (int jndex = 0; jndex < moves.size(); jndex++) {
                auto move = moves[jndex];
                
                if (!ignoreChoice.empty() && move == ignoreChoice[index]) { continue; }
                std::vector<std::vector<ChessActor>> newPieces = board.chessPieces;
                newPieces[move[0]][move[1]] = ChessActor();
                newPieces[move[2]][move[3]] = board.chessPieces[move[0]][move[1]];
                ChessBoard newBoard = ChessBoard(newPieces);
                
                int score = minimax(newBoard, false, 0);
                
                if (score > bestScore) {
                    bestScore = score;
                    bestChoice = move;
                }
            }
            choice.push_back(bestChoice);
            ChessActor tmpActor = board.chessPieces[bestChoice[0]][bestChoice[1]];
            board.chessPieces[bestChoice[0]][bestChoice[1]] = ChessActor();
            board.chessPieces[bestChoice[2]][bestChoice[3]] = tmpActor;
            
            if ( bestChoice[2] == 0 && board.chessPieces[bestChoice[2]][bestChoice[3]].idName() == pawn ) {
                board.chessPieces[bestChoice[2]][bestChoice[3]] = ChessActor(rook, white);
            }
            
            if (draw) {
                std::cout << "\n choice: " << bestChoice[0] << ' ' << bestChoice[1] << ' ' << bestChoice[2] << ' ' << bestChoice[3] << "\n\n";
                
                board.description({bestChoice[0], bestChoice[1]});
                
            }
            bool isOnlyKing = false;
            evaluate(board, isOnlyKing);
            
            if (isOnlyKing) {
                break;
            }
        }
    }
};

#endif /* ChessBestChoice_h */
