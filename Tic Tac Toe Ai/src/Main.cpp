#include <iostream>
#include <stdexcept>
#include "include/Main.h"
#include "include/Ai_module.h"

Move bestMove; // Stores the best move for the AI
win_pos position1; // Represents the position of winning move
win_pos position2; // Represents the position of winning move
win_pos position3; // Represents the position of winning move

Base::Base() {
    // Initialize winning positions to -1
    position1.row = position2.row = position3.row = -1;
    position1.col = position2.col = position3.col = -1;
}

// Displays the game UI
void Base::showUi(bool playerTurn) {
    // Array to store the board state
    int size = 9;
    char Uib[size]; // Actual board state
    char UIchoose[size]; // Available choices for the player

    // Populate arrays with board state and available choices
    for (int i = 1; i < 10; i++) {
        Uib[i - 1] = *translateBoard(p, i);
        UIchoose[i - 1] = *translateBoard(num_p, i);
    }

    // Display the UI
    std::cout << std::endl;
    if (isFinished) {
        std::cout << "          Final match               Winner moves       \n\n";
    } else {
        std::cout << "          Game match             Available choices       \n\n";
    }
    std::cout << "            |     |                   |     |             \n";
    std::cout << "         " << Uib[0] << "  |  " << Uib[1] << "  |  " << Uib[2] << "             " << UIchoose[0] << "  |  " << UIchoose[1] << "  |  " << UIchoose[2] << "       \n";
    std::cout << "        ____|_____|_____          ____|_____|_____        \n";
    std::cout << "            |     |                   |     |             \n";
    std::cout << "         " << Uib[3] << "  |  " << Uib[4] << "  |  " << Uib[5] << "     ==>     " << UIchoose[3] << "  |  " << UIchoose[4] << "  |  " << UIchoose[5] << "       \n";
    std::cout << "        ____|_____|_____          ____|_____|_____        \n";
    std::cout << "            |     |                   |     |             \n";
    std::cout << "         " << Uib[6] << "  |  " << Uib[7] << "  |  " << Uib[8] << "             " << UIchoose[6] << "  |  " << UIchoose[7] << "  |  " << UIchoose[8] << "       \n";
    std::cout << "            |     |                   |     |             \n\n\n";
    if (playerTurn) {
        std::cout << "\033[0m";
        std::cout << "     \033[41m You : " << showTurn("pl") << " \033[0m      \033[41m Bot :" << showTurn("op") << " ";
        std::cout << "\033[0m";
        std::cout << "\033[31m\n";
    }

    if (isFinished) {
        std::cout << "\033[0m Final result: ";
        std::cout << "     \033[33;44m Bot :" << player_won[0] << " \033[0m    vs    \033[33;44m You :" << player_won[1] << " ";
        std::cout << "\033[0m";
    }
}

// Determines whose turn it is
char Base::showTurn(const std::string st) {
    if (st == "pl")
        return symbol.opponent;

    return symbol.Bot;
}

// Checks if there's a winner or if the game is a draw
void Base::checkWinner() {
    // Check rows and columns for a winning combination
    for (int i = 0; i < 3; i++) {
        if (p[i][0] == p[i][1] && p[i][1] == p[i][2] && p[i][0] != '_') {
            isFinished = true;
            position1.row = position2.row = position3.row = i;
            position1.col = 0;
            position2.col = 1;
            position3.col = 2;
            set_winner(p[i][0]);
            break;
        } else if (p[0][i] == p[1][i] && p[1][i] == p[2][i] && p[0][i] != '_') {
            isFinished = true;
            position1.col = position2.col = position3.col = i;
            position1.row = 0;
            position2.row = 1;
            position3.row = 2;
            set_winner(p[0][i]);
            break;
        }
    }

    // Check diagonals for a winning combination
    if (p[0][0] == p[1][1] && p[1][1] == p[2][2] && p[0][0] != '_') {
        isFinished = true;
        position1.row = position1.col = 0;
        position2.row = position2.col = 1;
        position3.row = position3.col = 2;
        set_winner(p[0][0]);
    } else if (p[0][2] == p[1][1] && p[1][1] == p[2][0] && p[0][2] != '_') {
        isFinished = true;
        position1.row = 0;
        position1.col = 2;
        position2.row = 1;
        position2.col = 1;
        position3.row = 2;
        position3.col = 0;
        set_winner(p[0][2]);
    } else if (isMovesLeft(p) == false) {
        isFinished = true;
        set_winner('N');
    }
}

// Gets the position where the player wants to place their move
int Base::getPosition() {
    std::string ask;
    int pos;

    while (true) {
        std::cout << "Please enter the number where you want to place your move: ";
        std::cin >> ask;
        try {
            pos = std::stoi(ask);

            if (pos < 1 || pos > 9) {
                throw std::out_of_range("Integer value out of range");
            }
            if (isIndexFull(p, pos)) {
                throw std::invalid_argument("It was chosen before");
            }
        } catch (const std::exception &e) {
            std::cout << "Invalid input: " << e.what() << std::endl;
            continue;
        }

        return pos;
    }
}

// Takes action based on the player type and position chosen
void Base::takeAction(Players playerType, int position) {
    *translateBoard(p, position) = playerType.opponent;
    *translateBoard(num_p, position) = ' ';
}

// Player's move execution
void Base::playerExecution() {
    system("clear");
    showUi(true);
    takeAction(symbol, getPosition());
    checkWinner();
    if (!isFinished) {
        lead_road(symbol.Bot);
    }
}

// AI's move execution
void Base::bot_execute() {
    system("clear");
    showUi();

    if (mode == "med") {
        if (med_switch == "hard") {
            mode = "hard";
            bestMove = findBestMove(p);
            p[bestMove.row][bestMove.col] = symbol.Bot;
            num_p[bestMove.row][bestMove.col] = ' ';
            mode = "med";
            med_switch = "easy";
        } else {
            mode = "easy";
            bestMove = findBestMove(p);
            p[bestMove.row][bestMove.col] = symbol.Bot;
            num_p[bestMove.row][bestMove.col] = ' ';
            mode = "med";
            med_switch = "hard";
        }
    } else {
        bestMove = findBestMove(p);
        p[bestMove.row][bestMove.col] = symbol.Bot;
        num_p[bestMove.row][bestMove.col] = ' ';
    }
    checkWinner();
    if (!isFinished) {
        lead_road(symbol.opponent);
    }
}

// Determines whose turn it is and proceeds accordingly
void Base::lead_road(char turn) {
    if (turn == symbol.Bot) {
        bot_execute();
    } else {
        playerExecution();
    }
}

// Sets the winner of the game
void Base::set_winner(char result) {
    std::cout << "\033[33m";

    if (result == symbol.Bot) {
        system("clear");
        std::cout << "\n        You lose !!!\n\n\t\tBot won the match\n\n";
        player_won[0] += 1;
        showWin();
        showUi();
        reset_game();
    } else if (result == symbol.opponent) {
        system("clear");
        std::cout << "\n        Congratulations !!!\n\n\t\tYou won the match\n\n";
        player_won[1] += 1;
        showWin();
        showUi();
        reset_game();
    } else if (result == 'N') {
        system("clear");
        std::cout << "\n        Draw !\n\n\t\tGame ends without a winner\n\n";
        showUi();
        reset_game();
    }
}

// Displays the winning moves
void Base::showWin() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (num_p[i][j] != ' ') {
                num_p[i][j] = ' ';
            }
        }
    }
    num_p[position1.row][position1.col] = '*';
    num_p[position2.row][position2.col] = '*';
    num_p[position3.row][position3.col] = '*';
}

// Starts the game
void Base::start_game() {
    char ask = '_';
    std::cout << "\033[31m";

    std::cout << "Who would start first?\n\t";
    std::cout << "Enter 'b' for Bot | Enter any character for you to start the game: ";
    std::cin >> ask;

    std::cout << "\n\nAnd which mode?\n\t";
    std::cout << "Enter 'b' for beginner | Enter 'i' for intermediate | Enter 'p' for professional: ";
    std::cin >> mode;

    if (mode == "b") {
        mode = "easy";
    } else if (mode == "p") {
        mode = "hard";
    } else {
        mode = "med";
    }

    if (ask == 'b') {
        lead_road(symbol.Bot);
    } else {
        lead_road(symbol.opponent);
    }
}

// Resets the game
void Base::reset_game() {
    std::string ask_again = "default";
    bool play_again = false;
    int k = 1;

    while (true) {
        try {
            std::cout << "\n\n\tDo you want to play again (y/n)? ";
            std::cin >> ask_again;

            if (ask_again == "yes" || ask_again == "y") {
                play_again = true;
            } else if (ask_again == "no" || ask_again == "n") {
                std::cout << "Goodbye!";
                exit(0);
            } else {
                throw std::invalid_argument("Invalid entry!!!");
            }
        } catch (const std::exception &er) {
            std::cout << er.what();
            continue;
        }

        break;
    }

    if (play_again) {
        system("clear");
        isFinished = false;

        // Reset the game board
        for (auto &row : p) {
            for (auto &elem : row) {
                elem = '_';
            }
        }

        // Reset the number board
        for (auto &row_p : num_p) {
            for (auto &elem_p : row_p) {
                elem_p = static_cast<char>(k + '0');
                k += 1;
            }
        }

        start_game();
    }
}

// Main function
int main() {
    Base game;
    game.start_game();
    return 0;
}
