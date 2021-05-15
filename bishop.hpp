// Code for solving the 4 bishops problem.

#ifndef cpp_bishop_bishop_hpp
#define cpp_bishop_bishop_hpp

#include <bitset>
#include <cassert>
#include <vector>

using namespace std;

// The position of a particular bishop on the board.
class Position {
 public:
  Position(unsigned char x1, unsigned char y1) : x(x1), y(y1) {
    assert((x1 < 5) && (y1 < 4));
  }

  unsigned char x;  // 0 <= x < 5
  unsigned char y;  // 0 <= y < 4
};

// This vector is always of size 4 -- this is position of all white or all black pieces.
using PiecesPosition = vector<Position>;

/// Represents a 5x4 (length, height) or (x_max, y_max) chess board
class Board {
 public:
  void SetBoard(const PiecesPosition& white_pieces, const PiecesPosition& black_pieces);

  Board(const PiecesPosition& white_pieces, const PiecesPosition& black_pieces) {
    SetBoard(white_pieces, black_pieces);
  }

  // Initializes board to the starting position.
  Board();

  PiecesPosition GetWhitePositions() const;

  PiecesPosition GetBlackPositions() const;

  unsigned long ToUlong() const {
    return board_.to_ulong();
  }

  // Reverse of the function above.
  Board(unsigned long board_ulong) : board_(board_ulong) {}

  // Returns an ASCII art string of the board.
  string ToString() const;

  // Returns new boards that represent all the valid moves starting from current board.
  vector<Board> GetNextBoards(bool white_to_move) const;

 private:
  string PosToString(Position pos) const;

  // Five bytes to represent the board. The representation uses 20 bits (5x4) to list
  // positions for white pieces, similarly 20 bits for black pieces. 20 + 20 = 40 bits or
  // 8 bytes. The white bits are the least significant bits, followed by black bits.
  std::bitset<40> board_;
};

#endif
