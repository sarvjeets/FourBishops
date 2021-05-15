#include <cassert>
#include <iostream>

#include "bishop.hpp"
#include "utils.hpp"

using namespace std;

void TestSetGetBoard() {
  Board board;

  LOG(board.ToString());
  Board board_cpy(board.ToUlong());
  LOG(board_cpy.ToString());

  // Check if you got the pieces back.
  PiecesPosition white_ret = board.GetWhitePositions();
  for (int i = 0; i < 4; ++i) {
    assert((white_ret[i].x == 0) && (white_ret[i].y == i));
  }

  PiecesPosition black_ret = board.GetBlackPositions();
  for (int i = 0; i < 4; ++i) {
    assert((black_ret[i].x == 4) && (black_ret[i].y == i));
  }

  PiecesPosition white_pos;
  white_pos.emplace_back(0,0);
  white_pos.emplace_back(1,1);
  white_pos.emplace_back(2,2);
  white_pos.emplace_back(3,3);

  PiecesPosition black_pos;
  black_pos.emplace_back(4,3);
  black_pos.emplace_back(3,2);
  black_pos.emplace_back(2,1);
  black_pos.emplace_back(1,0);

  board.SetBoard(white_pos, black_pos);

  LOG(board.ToString());

  // Check if you got the pieces back.
  white_ret = board.GetWhitePositions();
  for (int i = 0; i < 4; ++i) {
    assert((white_ret[i].x == i) && (white_ret[i].y == i));
  }

  black_ret = board.GetBlackPositions();
  for (int i = 0; i < 4; ++i) {
    assert((black_ret[i].x == (i+1)) && (black_ret[i].y == i));
  }
}

void TestUlongSize() {
  assert(sizeof(unsigned long) >= 5);
}

void TestNextBoards() {
  PiecesPosition white_pos;
  white_pos.emplace_back(0,0);
  white_pos.emplace_back(1,1);
  white_pos.emplace_back(2,2);
  white_pos.emplace_back(3,3);

  PiecesPosition black_pos;
  black_pos.emplace_back(4,3);
  black_pos.emplace_back(3,2);
  black_pos.emplace_back(2,1);
  black_pos.emplace_back(1,0);

  Board board(white_pos, black_pos);

  LOG("Original" << board.ToString());

  vector<Board> next = board.GetNextBoards(true);
  for (const Board& b : next) {
    LOG(b.ToString());
  }

  LOG("Black moves");
  next = board.GetNextBoards(false);
  for (const Board& b : next) {
    LOG(b.ToString());
  }

  white_pos.clear();
  black_pos.clear();
  white_pos.emplace_back(0,0);
  white_pos.emplace_back(0,1);
  white_pos.emplace_back(0,2);
  white_pos.emplace_back(0,3);

  black_pos.emplace_back(4,0);
  black_pos.emplace_back(4,1);
  black_pos.emplace_back(4,2);
  black_pos.emplace_back(4,3);

  board.SetBoard(white_pos, black_pos);

  LOG("Original" << board.ToString());

  next = board.GetNextBoards(true);
  for (const Board& b : next) {
    LOG(b.ToString());
  }

  LOG("Black moves");
  next = board.GetNextBoards(false);
  for (const Board& b : next) {
    LOG(b.ToString());
  }
}

int main() {
  TestSetGetBoard();
  TestUlongSize();
  TestNextBoards();
  return 0;
}

