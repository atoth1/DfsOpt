#ifndef DFSOPT_SOLVER_HPP
#define DFSOPT_SOLVER_HPP

namespace DfsOpt {
  struct InputData;
  struct OutputData;

  class Solver {
    public:
      static OutputData solve(const int nLineups, const InputData& data);

  };
}

#endif