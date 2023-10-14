#include <iostream>
#include "ChessBestChoice.hpp"

int main() {
    
    
    ChessBoard board = ChessBoard::randomBoard({5, 5}, {1, 1}, {2,0}, {0,0}, {1,1});
    ChessBoard copyBoard = board;
    
    auto choice = ChessBestChoice();
    choice.maxHeight = 2;
    auto secChoice = ChessBestChoice();
    
    while (true) {
        choice.best(copyBoard, {}, false);
        if (choice.choice.size() >= 2) {
            break;
        } else {
            std::cout << "wrong board, random again...\n";
            
            board = ChessBoard::randomBoard({5, 5}, {1, 1}, {2,0}, {0,0}, {1,1});
            copyBoard = board;
            
            choice = ChessBestChoice();
            choice.maxHeight = 2;
        }
    }
    copyBoard = board;
    board.description();
    
    
    std::cout << " –– DEEPER –– \n\n";
    choice.best(board, {}, true);
    
    std::cout << " –– LOWER –– \n\n";
    copyBoard.description();
    secChoice.best(copyBoard, choice.choice, true);
    

}
