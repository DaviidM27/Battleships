#include <iostream>
#include <vector>

class Ship {
private:
    int size;

public:
    Ship(int _size) : size(_size) {}

    friend std::ostream& operator<<(std::ostream &out, const Ship &ship) {
        out << "Size: " << ship.size;
        return out;
    }
    int getSize() const {
        return size;
    }
};

class Board {
private:
    std::vector<std::vector<bool>> grid;

public:
    Board(int rows, int cols) : grid(rows, std::vector<bool>(cols, false)) {}

    friend std::ostream& operator<<(std::ostream &out, const Board &board) {
        for (const auto &row : board.grid) {
            for (bool cell : row) {
                out << (cell ? "X " : "_ ");
            }
            out << "\n";
        }
        return out;
    }

    // Validarea pozițiilor navei
    bool isValidPosition(int row, int col, int size, bool horizontal) const {
        if (horizontal) {
            return col + size <= static_cast<int>(grid[0].size());
        } else {
            return row + size <= static_cast<int>(grid.size());
        }
    }

    void placeShip(const Ship &ship, int row, int col, bool horizontal) {
        int size = ship.getSize();
        if (isValidPosition(row, col, size, horizontal)) {
            if (horizontal) {
                for (int c = col; c < col + size; ++c) {
                    grid[row][c] = true;
                }
            } else {
                for (int r = row; r < row + size; ++r) {
                    grid[r][col] = true;
                }
            }
        } else {
            std::cerr << "Invalid ship position!\n";
        }
    }

    bool isHit(int row, int col) const {
        return grid[row][col];
    }

    void markHit(int row, int col) {
        grid[row][col] = false;
    }
};

class Player {
private:
    std::string name;
    Board board;
    std::vector<Ship> ships;

public:
    Player(const std::string &_name, int rows, int cols) : name(_name), board(rows, cols) {}

    Player(const Player &other) : name(other.name), board(other.board), ships(other.ships) {}

    Player& operator=(const Player &other) {
        if (this != &other) {
            name = other.name;
            board = other.board;
            ships = other.ships;
        }
        return *this;
    }

    ~Player()=default;

    friend std::ostream& operator<<(std::ostream &out, const Player &player) {
        out << "Player: " << player.name << "\n";
        out << "Board:\n" << player.board;
        for (const auto &ship : player.ships) {
            out << ship << "\n";
        }
        return out;
    }
    void placeShip(const Ship &ship, int row, int col, bool horizontal) {
        board.placeShip(ship, row, col, horizontal);
        ships.push_back(ship);
    }

    bool Attack(int row, int col) {
        if (board.isHit(row, col)) {
            board.markHit(row, col);
            return true; // Hit
        }
        return false; // Miss
    }
};

int main() {
    // Crearea jucătorilor și a tablei de joc
    Player player1("Player 1", 5, 5);
    Player player2("Player 2", 5, 5);

    // Plasarea navelor pentru fiecare jucător
    Ship ship1(3);
    Ship ship2(2);
    player1.placeShip(ship1, 1, 1, true); // Plasare orizontală
    player2.placeShip(ship2, 3, 3, false); // Plasare verticală

    // Simularea atacurilor
    std::cout << "Attack result: " << player1.Attack(1, 1) << "\n"; // Player 1 atacă o poziție care contine o nava proprie
    std::cout << "Attack result: " << player1.Attack(2, 2) << "\n"; // Player 1 atacă o poziție fără navă

    // Afișare stadiul jocului
    std::cout << player1 << "\n";
    std::cout << player2 << "\n";
    return 0;
}
