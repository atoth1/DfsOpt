#include "DfsOpt.hpp"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

int main(int argc, char* argv[]) {
  std::vector<std::string_view> args(argv+1, argv+argc);
  // Default 1 lineup
  int nLineups = 1;
  std::string_view inFile{};
  std::string_view outFile{};
  auto parseCommandLine = [] (
    const std::vector<std::string_view>& args, 
    int& n,
    std::string_view& in,
    std::string_view& out
  ) {
    int parsedN = 0;
    for (auto it = args.cbegin(); it != args.cend(); ++it) {
      if (*it == "-n") {
        if (parsedN > 0) {
          throw std::runtime_error("ERROR: Lineup count provided multiple times.\n");
        }
        parsedN = std::atoi((it+1)->data());
        if (parsedN <= 0) {
          throw std::runtime_error("ERROR: Lineup count must be a positive number.\n");
        }
      }
      if (*it == "-i") {
        if (!in.empty()) {
          throw std::runtime_error("ERROR: Input file provided multiple times.\n");
        }
        in = *(it+1);
      }
      if (*it == "-o") {
        if (!out.empty()) {
          throw std::runtime_error("ERROR: Output file provided multiple times.\n");
        }
        out = *(it+1);
      }
    }
    if (parsedN > 0) {
      n = parsedN;
    }
    if (in.empty()) {
      throw std::runtime_error("ERROR: Input file must be specified with the \"-i\" flag\n");
    }
  };
  try {
    parseCommandLine(args, nLineups, inFile, outFile);
  } catch (const std::runtime_error& e) {
    std::cout << "ERROR: While parsing command line the following exception was reported:\n"
      << e.what() << '\n';
    return 0;
  }

  DfsOpt::InputData inData{};
  try {
    inData = DfsOpt::XmlParser::parse(inFile);
  } catch (const std::exception& e) {
    std::cout << "ERROR: During input parsing/validation the following exception was reported:\n"
      << e.what() << '\n';
    return 0;
  }

  auto preSolve = std::chrono::steady_clock::now();
  auto outData = DfsOpt::Solver::solve(nLineups, inData);
  auto postSolve = std::chrono::steady_clock::now();
  std::chrono::duration<double> time = postSolve - preSolve;

  try {
    DfsOpt::Outputter::output(outFile, outData, nLineups, time.count());
  } catch (const std::exception& e) {
    std::cout << "ERROR: While outputting solution data the following exception was reported:\n"
      << e.what() << '\n';
  }
  return 0;
}
