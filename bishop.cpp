// Board code for solving the 4 bishops problem.

#include <cassert>

#include "bishop.hpp"
#include "utils.hpp"

namespace {

// Returns bit inside board_ corresponding to pos.
unsigned char PositionToBit(Position pos) {
  assert( (pos.x < 5) & (pos.y < 4));
  unsigned char ret = 5 * pos.y + pos.x;
  assert(ret < 20);
  return ret;
}

// Inverse of above function.
Position BitToPosition(unsigned char pos) {
  assert(pos < 20);
  return Position(pos % 5 , pos / 5);
}

// Returns whether (x, y) is inside the board.
bool IsInside(int x, int y) {
  return ((x >= 0) &&
          (x < 5) &&
          (y >= 0) &&
          (y < 4));
}

// Returns all positions that are threatened by pos.
vector<Position> Threatened(const Position& pos) {
  int x = pos.x;
  int y = pos.y;

  vector<Position> ret;
  for (int i = 1; i <= 3; ++i) {
    if (IsInside(x + i, y + i)) ret.push_back(Position(x + i, y + i));
    if (IsInside(x + i, y - i)) ret.push_back(Position(x + i, y - i));
    if (IsInside(x - i, y + i)) ret.push_back(Position(x - i, y + i));
    if (IsInside(x - i, y - i)) ret.push_back(Position(x - i, y - i));
  }
  return ret;
}

bitset<20> PositionsToBitset(const vector<Position>& positions) {
  bitset<20> ret;
  for (const auto& pos : positions) {
    unsigned char i = PositionToBit(pos);
    assert(!ret[i]);
    ret.set(i);
  }
  return ret;
}

bitset<20> ThreatenedBits(const PiecesPosition& pieces) {
  bitset<20> ret = PositionsToBitset(pieces);

  for (const Position& pos : pieces) {
    vector<Position> threatened_poss = Threatened(pos);
    for (const Position& threatened : threatened_poss) {
      ret.set(PositionToBit(threatened));
    }
  }
  return ret;
}

vector<Position> GenerateMoves(const Position& me, const vector<Position>& rest) {
  bitset<20> rest_bits = PositionsToBitset(rest);

  int directions[] = {-1, +1};
  int x = me.x;
  int y = me.y;

  vector<Position> ret;
  for (int x_dir : directions) {
    for (int y_dir : directions) {
      for (int i = 1; i <= 3; ++i) {
        int new_x = x + x_dir * i;
        int new_y = y + y_dir * i;
        if (!IsInside(new_x, new_y)) break;  // outside the board. Done.
        Position new_pos(new_x, new_y);
        if (rest_bits[PositionToBit(new_pos)]) break; // hitting a piece. Done.
        // Otherwise, this is a valid move.
        ret.push_back(new_pos);
      } // i
    }  // y_dir
  }  // x_dir
  return ret;
}

void RemoveThreatenedPositions(vector<Position>* moves,
                               const bitset<20>& threatened_bits) {
  vector<Position> valid_moves;
  for (const Position& pos : *moves) {
    if (!threatened_bits[PositionToBit(pos)]) valid_moves.push_back(pos);
  }
  *moves = valid_moves;
}

}  // namespace

// **************** class Board **************************************

void Board::SetBoard(const PiecesPosition& white_pieces,
                     const PiecesPosition& black_pieces) {
  // Sanity checks.
  assert((white_pieces.size() == 4) && (black_pieces.size() == 4));

  board_.reset();

  // Set white pieces.
  for (int i = 0; i < 4 ; ++i) {
    board_.set(PositionToBit(white_pieces[i]));
  }

  // Set black pieces.
  for (int i = 0; i < 4 ; ++i) {
    board_.set(20 + PositionToBit(black_pieces[i]));
  }

  // Sanity checks.
  assert(board_.count() == 8);

  bitset<40> combined = (board_ >> 20) | board_;
  assert(combined.count() == 12);
}

Board::Board() {
  PiecesPosition white_pos;
  white_pos.emplace_back(0,0);
  white_pos.emplace_back(0,1);
  white_pos.emplace_back(0,2);
  white_pos.emplace_back(0,3);

  PiecesPosition black_pos;
  black_pos.emplace_back(4,0);
  black_pos.emplace_back(4,1);
  black_pos.emplace_back(4,2);
  black_pos.emplace_back(4,3);

  SetBoard(white_pos, black_pos);
}

PiecesPosition Board::GetWhitePositions() const {
  PiecesPosition ret;

  for (int i = 0; i < 20; ++i) {
    if (board_[i]) {
      ret.push_back(BitToPosition(i));
    }
  }

  assert(ret.size() == 4);
  return ret;
}

PiecesPosition Board::GetBlackPositions() const {
  PiecesPosition ret;

  for (int i = 20; i < 40; ++i) {
    if (board_[i]) {
      ret.push_back(BitToPosition(i - 20));
    }
  }

  assert(ret.size() == 4);
  return ret;
}

string Board::PosToString(Position pos) const {
  unsigned char white_bit = PositionToBit(pos);
  unsigned char black_bit = 20 + white_bit;

  if (board_[white_bit]) return "W";
  if (board_[black_bit]) return "B";
  return " ";
}

string Board::ToString() const {
  const string sep = "\n+---+---+---+---+---+\n";
  string board_str = sep;
  for (int y = 3; y >= 0; --y) {
    for(int x=0; x < 5; ++x) {
      board_str += "| " + PosToString(Position(x, y)) + " ";
    }
    board_str += "|" + sep;
  }
  return board_str;
}

vector<Board> Board::GetNextBoards(bool white_to_move) const {
  const PiecesPosition my_positions =
      white_to_move ? GetWhitePositions() : GetBlackPositions();
  const PiecesPosition other_positions =
      white_to_move ? GetBlackPositions() : GetWhitePositions();
  bitset<20> threatened_bits = ThreatenedBits(other_positions);

  vector<Board> ret;
  // Iterate through pieces and move them.
  for (int i = 0; i < 4; ++i) {
    const Position& me = my_positions[i];
    vector<Position> rest = my_positions;
    rest.erase(rest.begin() + i);

    // Generate all moves considering only pieces of my color. If there is no bug,
    // none of these positions should be on top of other pieces (as they were not
    // threatening me when we began a move).
    vector<Position> all_my_moves = GenerateMoves(me, rest);

    RemoveThreatenedPositions(&all_my_moves, threatened_bits);

    // Generate and add new boards to ret.
    for (const Position& pos : all_my_moves) {
      PiecesPosition my_new_positions = rest;
      my_new_positions.push_back(pos);

      if (white_to_move) {
        ret.emplace_back(my_new_positions, other_positions);
      } else {
        ret.emplace_back(other_positions, my_new_positions);
      }
    }
  }

  return ret;
}
