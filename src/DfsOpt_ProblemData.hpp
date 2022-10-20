#ifndef DFSOPT_PLAYERDATA_HPP
#define DFSOPT_PLAYERDATA_HPP

#include <string>
#include <vector>

namespace DfsOpt {

struct Player {
  std::string name_{};
  std::string team_{};
  int salary_{};
  double expectedPts_{};
};

struct Dst {
  std::string team_{};
  int salary_{};
  double expectedPts_{};
};

struct InputData {
  int budget_{};
  int numQbs_{};
  int numRbs_{};
  int numWrs_{};
  int numTes_{};
  int numFlex_{};
  int numDsts_{};

  std::vector<Player> qbs_{};
  std::vector<Player> rbs_{};
  std::vector<Player> wrs_{};
  std::vector<Player> tes_{};
  std::vector<Dst> dsts_{};
};

struct Roster {
  int cost_{};
  double expectedPts_{};
  std::vector<Player> qbs_{};
  std::vector<Player> rbs_{};
  std::vector<Player> wrs_{};
  std::vector<Player> tes_{};
  std::vector<Player> flex_{};
  std::vector<Dst> dsts_{};
};

struct OutputData {
  std::vector<Roster> rosters_{};
};

}

#endif