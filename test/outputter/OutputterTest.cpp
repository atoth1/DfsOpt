#include "DfsOpt.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <string_view>

TEST(OutputterTests, NumRequested) {
  std::string_view outFile =  "num-requested.txt";
  std::string_view goldFile = "num-requested.txt.gold";
  const int nLineups = 2;
  const double time = 0.5;

  auto getOutData = []() {
    return DfsOpt::OutputData{{
      DfsOpt::Roster{6, 6., {{"A", "A", 1, 1.}}, {{"B", "B", 1, 1.}}, {{"C", "C", 1, 1.}},
        {{"D", "D", 1, 1.}}, {{"E", "E", 1, 1.}}, {{"F", 1, 1.}}},
      DfsOpt::Roster{6, 6., {{"G", "G", 1, 1.}}, {{"G", "G", 1, 1.}}, {{"I", "I", 1, 1.}},
        {{"J", "J", 1, 1.}}, {{"K", "K", 1, 1.}}, {{"L", 1, 1.}}}
      }};
  };
  DfsOpt::Outputter::output(outFile, getOutData(), nLineups, time);

  std::ifstream output(outFile.data(), std::ifstream::binary|std::ifstream::ate);
  std::ifstream gold(goldFile.data(), std::ifstream::binary|std::ifstream::ate);
  ASSERT_FALSE(output.fail());
  ASSERT_FALSE(gold.fail());
  ASSERT_EQ(output.tellg(), gold.tellg());
  output.seekg(0, std::ifstream::beg);
  gold.seekg(0, std::ifstream::beg);
  using Iter = std::istreambuf_iterator<char>;
  EXPECT_TRUE(std::equal(Iter(output.rdbuf()), Iter(), Iter(gold.rdbuf())));
}

TEST(OutputterTests, FewerThanRequested) {
  std::string_view outFile  = "fewer-num-requested.txt";
  std::string_view goldFile = "fewer-num-requested.txt.gold";
  const int nLineups = 2;
  const double time = 0.5;

  auto getOutData = []() {
    return DfsOpt::OutputData{{
      DfsOpt::Roster{6, 6., {{"A", "A", 1, 1.}}, {{"B", "B", 1, 1.}}, {{"C", "C", 1, 1.}},
        {{"D", "D", 1, 1.}}, {{"E", "E", 1, 1.}}, {{"F", 1, 1.}}},
    }};
  };
  DfsOpt::Outputter::output(outFile, getOutData(), nLineups, time);

  std::ifstream output(outFile.data(), std::ifstream::binary|std::ifstream::ate);
  std::ifstream gold(goldFile.data(), std::ifstream::binary|std::ifstream::ate);
  ASSERT_FALSE(output.fail());
  ASSERT_FALSE(gold.fail());
  ASSERT_EQ(output.tellg(), gold.tellg());
  output.seekg(0, std::ifstream::beg);
  gold.seekg(0, std::ifstream::beg);
  using Iter = std::istreambuf_iterator<char>;
  EXPECT_TRUE(std::equal(Iter(output.rdbuf()), Iter(), Iter(gold.rdbuf())));
}
