// BFS for 4 bishops problem.

#include <cassert>
#include <map>
#include <vector>

#include "bishop.hpp"
#include "utils.hpp"

using namespace std;

namespace {

void bfs(unsigned long start, unsigned long end) {
  vector<unsigned long> next;
  map<unsigned long, unsigned long> visited;  // Map of board -> parent
  unsigned int moves = 0;

  next.push_back(start);
  visited[start] = 0;

  while (!next.empty()) {
    vector<unsigned long> new_next;

    for (unsigned long b_ulong : next) {
      if (b_ulong == end) {
        LOG("Found solution in " << moves << " moves.");
        vector<Board> solution;

        unsigned long parent = b_ulong;
        while (parent != 0) {
          solution.emplace(solution.begin(), parent);
          parent = visited[parent];
        }

        for (const Board& sol : solution) {
          LOG(sol.ToString());
        }
        return;
      }

      Board b(b_ulong);
      vector<Board> next_boards = b.GetNextBoards(!bool(moves % 2));

      for (const auto& curr : next_boards) {
        unsigned long curr_ulong = curr.ToUlong();

        if (visited.count(curr_ulong) == 0) {
          visited[curr_ulong] = b_ulong;
          new_next.push_back(curr_ulong);
        }
      }
    }

    next = new_next;
    ++moves;
  }

  assert(false);
}

}  // namespace

int main() {
  const Board start_board;
  const Board end_board(start_board.GetBlackPositions(), start_board.GetWhitePositions());

  bfs(start_board.ToUlong(), end_board.ToUlong());

  return 0;
}
