#ifndef DFSOPT_PLAYERDATA_HPP
#define DFSOPT_PLAYERDATA_HPP

#include <cmath>
#include <string>
#include <vector>

namespace DfsOpt {

struct Player {
  std::string name_{};
  std::string team_{};
  int salary_{};
  double expectedPts_{};
};

inline bool operator==(const Player& l, const Player& r) {
  return l.name_ == r.name_ && l.team_ == r.team_ && l.salary_ == r.salary_
    && std::abs(l.expectedPts_ - r.expectedPts_) < 1.e-14;
}

struct Dst {
  std::string team_{};
  int salary_{};
  double expectedPts_{};


};

inline bool operator==(const Dst& l, const Dst& r) {
  return l.team_ == r.team_ && l.salary_ == r.salary_
    && std::abs(l.expectedPts_ - r.expectedPts_) < 1.e-14;
}

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

  std::vector<Player> reqQbs_{};
  std::vector<Player> reqRbs_{};
  std::vector<Player> reqWrs_{};
  std::vector<Player> reqTes_{};
  std::vector<Dst> reqDsts_{};
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