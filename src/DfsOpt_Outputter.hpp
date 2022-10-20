#ifndef DFSOPT_OUTPUTTER_HPP
#define DFSOPT_OUTPUTTER_HPP

#include <string_view>

namespace DfsOpt {
  struct OutputData;

  class Outputter
  {
  public:
    static void output(const std::string_view outFileName, 
                       const OutputData& outData,
                       const int nLineupsExpected,
                       const double solveTime);
  };

}

#endif