#include "DfsOpt.hpp"
#include "gtest/gtest.h"

#include <cmath>
#include <exception>

TEST(SolverTests, ExpectedOutputs) {
  auto getInData = []() {
    static constexpr int budget = 1000;
    static constexpr int numQbs = 1;
    static constexpr int numRbs = 1;
    static constexpr int numWrs = 1;
    static constexpr int numTes = 1;
    static constexpr int numFlex = 1;
    static constexpr int numDsts = 1;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 2.}, {"b", "2", 2, 3.}, {"c", "3", 3, 1.}};
    std::vector<DfsOpt::Player> rbs{{"d", "4", 4, 10.}, {"e", "5", 5, 5.}};
    std::vector<DfsOpt::Player> wrs{{"f", "6", 6, 0.}, {"g", "7", 7, 10.}};
    std::vector<DfsOpt::Player> tes{{"h", "8", 8, 10.}, {"i", "9", 9, 0.}};
    std::vector<DfsOpt::Player> ks{};
    std::vector<DfsOpt::Dst> dsts{{"j", 10, 0.}, {"k", 11, 10.}};
    return DfsOpt::InputData{budget, numQbs, numRbs, numWrs, numTes, numFlex, numDsts, qbs, rbs, wrs, tes, ks, dsts};
  };

  auto inData = getInData();
  auto outData = DfsOpt::Solver::solve(2, inData);
  EXPECT_EQ(outData.rosters_.size(), 2);

  auto& r1 = outData.rosters_[0];
  EXPECT_EQ(r1.cost_, 37);
  EXPECT_NEAR(r1.expectedPts_, 48., 1.0e-14);
  EXPECT_EQ(r1.qbs_.size(), 1);
  EXPECT_TRUE((r1.qbs_[0] == DfsOpt::Player{"b", "2", 2, 3.}));
  EXPECT_EQ(r1.rbs_.size(), 1);
  EXPECT_TRUE((r1.rbs_[0] == DfsOpt::Player{"d", "4", 4, 10.}));
  EXPECT_EQ(r1.wrs_.size(), 1);
  EXPECT_TRUE((r1.wrs_[0] == DfsOpt::Player{"g", "7", 7, 10.}));
  EXPECT_EQ(r1.tes_.size(), 1);
  EXPECT_TRUE((r1.tes_[0] == DfsOpt::Player{"h", "8", 8, 10.}));
  EXPECT_EQ(r1.flex_.size(), 1);
  EXPECT_TRUE((r1.flex_[0] == DfsOpt::Player{"e", "5", 5, 5.}));
  EXPECT_EQ(r1.dsts_.size(), 1);
  EXPECT_TRUE((r1.dsts_[0] == DfsOpt::Dst{"k", 11, 10.}));

  auto& r2 = outData.rosters_[1];
  EXPECT_EQ(r2.cost_, 36);
  EXPECT_NEAR(r2.expectedPts_, 47., 1.0e-14);
  EXPECT_EQ(r2.qbs_.size(), 1);
  EXPECT_TRUE((r2.qbs_[0] == DfsOpt::Player{"a", "1", 1, 2.}));
  EXPECT_EQ(r2.rbs_.size(), 1);
  EXPECT_TRUE((r2.rbs_[0] == DfsOpt::Player{"d", "4", 4, 10.}));
  EXPECT_EQ(r2.wrs_.size(), 1);
  EXPECT_TRUE((r2.wrs_[0] == DfsOpt::Player{"g", "7", 7, 10.}));
  EXPECT_EQ(r2.tes_.size(), 1);
  EXPECT_TRUE((r2.tes_[0] == DfsOpt::Player{"h", "8", 8, 10.}));
  EXPECT_EQ(r2.flex_.size(), 1);
  EXPECT_TRUE((r2.flex_[0] == DfsOpt::Player{"e", "5", 5, 5.}));
  EXPECT_EQ(r2.dsts_.size(), 1);
  EXPECT_TRUE((r2.dsts_[0] == DfsOpt::Dst{"k", 11, 10.}));

  EXPECT_TRUE(true);
}

TEST(SolverTests, ExpectedOutputsWithRequiredPlayers) {
  auto getInData = []() {
    static constexpr int budget = 10;
    static constexpr int numQbs = 1;
    static constexpr int numRbs = 1;
    static constexpr int numWrs = 1;
    static constexpr int numTes = 1;
    static constexpr int numFlex = 1;
    static constexpr int numDsts = 1;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 1.}, {"a0", "0", 2, 2.}};
    std::vector<DfsOpt::Player> rbs{{"b", "2", 1, 1.}, {"c", "3", 2, 1.}};
    std::vector<DfsOpt::Player> wrs{{"d", "4", 1, 1.}};
    std::vector<DfsOpt::Player> tes{{"e", "5", 1, 1.}, {"e0", "6", 1, 1.}};
    std::vector<DfsOpt::Player> ks{};
    std::vector<DfsOpt::Dst> dsts{{"g", 1, 1.}, {"h", 2, 2.}};
    std::vector<DfsOpt::Player> reqQbs{{"a", "1", 1, 1.}};
    std::vector<DfsOpt::Player> reqTes{{"e", "5", 1, 1.}};
    std::vector<DfsOpt::Dst> reqDsts{{"g", 1, 1.}};
    return DfsOpt::InputData{budget, numQbs, numRbs, numWrs, numTes, numFlex, numDsts,
      qbs, rbs, wrs, tes, ks, dsts, reqQbs, {}, {}, reqTes, reqDsts};
  };

  auto inData = getInData();
  DfsOpt::OutputData outData = DfsOpt::Solver::solve(1, inData);
  EXPECT_EQ(outData.rosters_.size(), 1);

  auto& r = outData.rosters_[0];
  EXPECT_EQ(r.cost_, 7);
  EXPECT_NEAR(r.expectedPts_, 6., 1.0e-14);
  EXPECT_EQ(r.qbs_.size(), 1);
  EXPECT_TRUE((r.qbs_[0] == DfsOpt::Player{"a", "1", 1, 1.}));
  EXPECT_EQ(r.rbs_.size(), 1);
  EXPECT_TRUE((r.rbs_[0] == DfsOpt::Player{"b", "2", 1, 1.}));
  EXPECT_EQ(r.wrs_.size(), 1);
  EXPECT_TRUE((r.wrs_[0] == DfsOpt::Player{"d", "4", 1, 1.}));
  EXPECT_EQ(r.tes_.size(), 1);
  EXPECT_TRUE((r.tes_[0] == DfsOpt::Player{"e", "5", 1, 1.}));
  EXPECT_EQ(r.flex_.size(), 1);
  EXPECT_TRUE((r.flex_[0] == DfsOpt::Player{"c", "3", 2, 1.}));
  EXPECT_EQ(r.dsts_.size(), 1);
  EXPECT_TRUE((r.dsts_[0] == DfsOpt::Dst{"g", 1, 1.}));
}

TEST(SolverTests, RequiredPlayersTooExpensive) {
  auto getInData = []() {
    static constexpr int budget = 5;
    static constexpr int numQbs = 1;
    static constexpr int numRbs = 1;
    static constexpr int numWrs = 1;
    static constexpr int numTes = 1;
    static constexpr int numFlex = 1;
    static constexpr int numDsts = 1;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 1.}, {"a0", "0", 6, 1.}};
    std::vector<DfsOpt::Player> rbs{{"b", "2", 1, 1.}, {"c", "3", 1, 1.}};
    std::vector<DfsOpt::Player> wrs{{"d", "4", 1, 1.}};
    std::vector<DfsOpt::Player> tes{{"e", "5", 1, 1.}};
    std::vector<DfsOpt::Player> ks{};
    std::vector<DfsOpt::Dst> dsts{{"g", 1, 1.}};
    std::vector<DfsOpt::Player> reqQbs{{"a0", "0", 6, 1.}};
    return DfsOpt::InputData{budget, numQbs, numRbs, numWrs, numTes, numFlex, numDsts,
      qbs, rbs, wrs, tes, ks, dsts, reqQbs};
  };

  auto inData = getInData();
  DfsOpt::OutputData outData{};
  EXPECT_THROW(outData = DfsOpt::Solver::solve(1, inData), std::exception);
}

TEST(SolverTests, NoSolutions) {
  auto getInData = []() {
    static constexpr int budget = 5;
    static constexpr int numQbs = 1;
    static constexpr int numRbs = 1;
    static constexpr int numWrs = 1;
    static constexpr int numTes = 1;
    static constexpr int numFlex = 1;
    static constexpr int numDsts = 1;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 1.}};
    std::vector<DfsOpt::Player> rbs{{"b", "2", 1, 1.}, {"c", "3", 1, 1.}};
    std::vector<DfsOpt::Player> wrs{{"d", "4", 1, 1.}};
    std::vector<DfsOpt::Player> tes{{"e", "5", 1, 1.}};
    std::vector<DfsOpt::Player> ks{};
    std::vector<DfsOpt::Dst> dsts{{"g", 1, 1.}};
    return DfsOpt::InputData{budget, numQbs, numRbs, numWrs, numTes, numFlex, numDsts, qbs, rbs, wrs, tes, ks, dsts};
  };

  auto inData = getInData();
  auto outData = DfsOpt::Solver::solve(1, inData);
  EXPECT_EQ(outData.rosters_.size(), 0);
}

TEST(SolverTests, CaptainMode) {
  auto getInData = []() {
    static constexpr int budget = 100;
    static constexpr int numFlex = 6;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 1.}};
    std::vector<DfsOpt::Player> rbs{{"b", "2", 2, 2.}, {"c", "3", 3, 3.}};
    std::vector<DfsOpt::Player> wrs{{"d", "4", 4, 4.}};
    std::vector<DfsOpt::Player> tes{{"e", "5", 5, 5.}};
    std::vector<DfsOpt::Player> ks{{"g", "6", 6, 6.}};
    std::vector<DfsOpt::Dst> dsts{{"h", 0, 0.}};
    return DfsOpt::InputData{budget, 0, 0, 0, 0, numFlex, 0, qbs, rbs, wrs, tes, ks, dsts, {}, {}, {}, {}, {}, true};
  };

  auto inData = getInData();
  auto outData = DfsOpt::Solver::solve(1, inData);
  EXPECT_EQ(outData.rosters_.size(), 1);

  auto& r = outData.rosters_[0];
  EXPECT_EQ(r.cost_, 24);
  EXPECT_NEAR(r.expectedPts_, 24., 1.0e-14);
  EXPECT_EQ(r.qbs_.size(), 0);
  EXPECT_EQ(r.rbs_.size(), 0);
  EXPECT_EQ(r.wrs_.size(), 0);
  EXPECT_EQ(r.tes_.size(), 0);
  EXPECT_EQ(r.flex_.size(), 6);
  EXPECT_FALSE(r.dstIsCaptain_);
  // First player in flex array is captain
  EXPECT_TRUE((r.flex_[0] == DfsOpt::Player{"g", "6", 6, 6.}));
  EXPECT_TRUE((r.flex_[1] == DfsOpt::Player{"a", "1", 1, 1.}));
  EXPECT_TRUE((r.flex_[2] == DfsOpt::Player{"b", "2", 2, 2.}));
  EXPECT_TRUE((r.flex_[3] == DfsOpt::Player{"c", "3", 3, 3.}));
  EXPECT_TRUE((r.flex_[4] == DfsOpt::Player{"d", "4", 4, 4.}));
  EXPECT_TRUE((r.flex_[5] == DfsOpt::Player{"e", "5", 5, 5.}));
  EXPECT_EQ(r.dsts_.size(), 0);
}

TEST(SolverTests, CaptainModeDstCaptain) {
  auto getInData = []() {
    static constexpr int budget = 100;
    static constexpr int numFlex = 6;
    std::vector<DfsOpt::Player> qbs{{"a", "1", 1, 1.}};
    std::vector<DfsOpt::Player> rbs{{"b", "2", 2, 2.}, {"c", "3", 3, 3.}};
    std::vector<DfsOpt::Player> wrs{{"d", "4", 4, 4.}};
    std::vector<DfsOpt::Player> tes{{"e", "5", 5, 5.}};
    std::vector<DfsOpt::Player> ks{{"g", "6", 0, 0.}};
    std::vector<DfsOpt::Dst> dsts{{"h", 6, 6.}};
    return DfsOpt::InputData{budget, 0, 0, 0, 0, numFlex, 0, qbs, rbs, wrs, tes, ks, dsts, {}, {}, {}, {}, {}, true};
  };

  auto inData = getInData();
  auto outData = DfsOpt::Solver::solve(1, inData);
  EXPECT_EQ(outData.rosters_.size(), 1);

  auto& r = outData.rosters_[0];
  EXPECT_EQ(r.cost_, 24);
  EXPECT_NEAR(r.expectedPts_, 24., 1.0e-14);
  EXPECT_EQ(r.qbs_.size(), 0);
  EXPECT_EQ(r.rbs_.size(), 0);
  EXPECT_EQ(r.wrs_.size(), 0);
  EXPECT_EQ(r.tes_.size(), 0);
  EXPECT_EQ(r.flex_.size(), 5);
  EXPECT_TRUE(r.dstIsCaptain_);
  EXPECT_TRUE((r.flex_[0] == DfsOpt::Player{"a", "1", 1, 1.}));
  EXPECT_TRUE((r.flex_[1] == DfsOpt::Player{"b", "2", 2, 2.}));
  EXPECT_TRUE((r.flex_[2] == DfsOpt::Player{"c", "3", 3, 3.}));
  EXPECT_TRUE((r.flex_[3] == DfsOpt::Player{"d", "4", 4, 4.}));
  EXPECT_TRUE((r.flex_[4] == DfsOpt::Player{"e", "5", 5, 5.}));
  EXPECT_EQ(r.dsts_.size(), 1);
  EXPECT_TRUE((r.dsts_[0] == DfsOpt::Dst{"h", 6, 6.}));
}
