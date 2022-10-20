#include "DfsOpt.hpp"
#include "gtest/gtest.h"

#include <cmath>
#include <exception>
#include <string_view>

TEST(ParserTests, Success) {
  std::string_view inFile = "successful-parse.xml";
  DfsOpt::InputData inData{};
  EXPECT_NO_THROW(inData = DfsOpt::XmlParser::parse(inFile));
  EXPECT_EQ(inData.budget_, 50000);
  EXPECT_EQ(inData.numQbs_, 1);
  EXPECT_EQ(inData.numRbs_, 2);
  EXPECT_EQ(inData.numWrs_, 3);
  EXPECT_EQ(inData.numTes_, 1);
  EXPECT_EQ(inData.numFlex_, 1);
  EXPECT_EQ(inData.numDsts_, 1);

  // Keeping in mind, position vectors are sorted by increasing cost
  EXPECT_EQ(inData.qbs_.size(), 2);
  EXPECT_TRUE((inData.qbs_[0] == DfsOpt::Player{"Justin Herbert", "LAC", 7200, 22.9}));
  EXPECT_TRUE((inData.qbs_[1] == DfsOpt::Player{"Lamar Jackson", "BAL", 8000, 23.0}));
  EXPECT_EQ(inData.rbs_.size(), 4);
  EXPECT_TRUE((inData.rbs_[0] == DfsOpt::Player{"Saquon Barkley", "NYG", 7900, 21.7}));
  EXPECT_TRUE((inData.rbs_[1] == DfsOpt::Player{"Derrick Henry", "TEN", 8200, 17.9}));
  EXPECT_TRUE((inData.rbs_[2] == DfsOpt::Player{"Austin Ekeler", "LAC", 8300, 22.2}));
  EXPECT_TRUE((inData.rbs_[3] == DfsOpt::Player{"Christian McCaffrey", "CAR", 8400, 19.2}));
  EXPECT_EQ(inData.wrs_.size(), 4);
  EXPECT_TRUE((inData.wrs_[0] == DfsOpt::Player{"Tee Higgins", "CIN", 6400, 15.7}));
  EXPECT_TRUE((inData.wrs_[1] == DfsOpt::Player{"CeeDee Lamb", "DAL", 6800, 19.8}));
  EXPECT_TRUE((inData.wrs_[2] == DfsOpt::Player{"Deebo Samuel", "SF", 7600, 16.4}));
  EXPECT_TRUE((inData.wrs_[3] == DfsOpt::Player{"Davante Adams", "LV", 8700, 20.7}));
  EXPECT_EQ(inData.tes_.size(), 2);
  EXPECT_TRUE((inData.tes_[0] == DfsOpt::Player{"Mark Andrews", "BAL", 7400, 17.8}));
  EXPECT_TRUE((inData.tes_[1] == DfsOpt::Player{"Travis Kelce", "KC", 8000, 18.0}));
  EXPECT_EQ(inData.dsts_.size(), 2);
  EXPECT_TRUE((inData.dsts_[0] == DfsOpt::Dst{"LV", 3300 , 13.0}));
  EXPECT_TRUE((inData.dsts_[1] == DfsOpt::Dst{"TB", 3900, 13.4}));
}

TEST(ParserTests, SuccessWithRequiredPlayers) {
  std::string_view inFile = "successful-parse-req-players.xml";
  DfsOpt::InputData inData{};
  EXPECT_NO_THROW(inData = DfsOpt::XmlParser::parse(inFile));
  EXPECT_EQ(inData.budget_, 50000);
  EXPECT_EQ(inData.numQbs_, 1);
  EXPECT_EQ(inData.numRbs_, 2);
  EXPECT_EQ(inData.numWrs_, 3);
  EXPECT_EQ(inData.numTes_, 1);
  EXPECT_EQ(inData.numFlex_, 1);
  EXPECT_EQ(inData.numDsts_, 1);

  EXPECT_EQ(inData.reqQbs_.size(), 1);
  EXPECT_TRUE((inData.reqQbs_[0] == DfsOpt::Player{"Justin Herbert", "LAC", 7200, 22.9}));
  EXPECT_EQ(inData.reqRbs_.size(), 1);
  EXPECT_TRUE((inData.reqRbs_[0] == DfsOpt::Player{"Saquon Barkley", "NYG", 7900, 21.7}));
  EXPECT_EQ(inData.reqWrs_.size(), 1);
  EXPECT_TRUE((inData.reqWrs_[0] == DfsOpt::Player{"Deebo Samuel", "SF", 7600, 16.4}));
  EXPECT_EQ(inData.reqTes_.size(), 1);
  EXPECT_TRUE((inData.reqTes_[0] == DfsOpt::Player{"Travis Kelce", "KC", 8000, 18.0}));
  EXPECT_EQ(inData.reqDsts_.size(), 1);
  EXPECT_TRUE((inData.reqDsts_[0] == DfsOpt::Dst{"TB", 3900, 13.4}));

  EXPECT_EQ(inData.qbs_.size(), 1);
  EXPECT_TRUE((inData.qbs_[0] == DfsOpt::Player{"Lamar Jackson", "BAL", 8000, 23.0}));
  EXPECT_EQ(inData.rbs_.size(), 3);
  EXPECT_TRUE((inData.rbs_[0] == DfsOpt::Player{"Derrick Henry", "TEN", 8200, 17.9}));
  EXPECT_TRUE((inData.rbs_[1] == DfsOpt::Player{"Austin Ekeler", "LAC", 8300, 22.2}));
  EXPECT_TRUE((inData.rbs_[2] == DfsOpt::Player{"Christian McCaffrey", "CAR", 8400, 19.2}));
  EXPECT_EQ(inData.wrs_.size(), 3);
  EXPECT_TRUE((inData.wrs_[0] == DfsOpt::Player{"Tee Higgins", "CIN", 6400, 15.7}));
  EXPECT_TRUE((inData.wrs_[1] == DfsOpt::Player{"CeeDee Lamb", "DAL", 6800, 19.8}));
  EXPECT_TRUE((inData.wrs_[2] == DfsOpt::Player{"Davante Adams", "LV", 8700, 20.7}));
  EXPECT_EQ(inData.tes_.size(), 1);
  EXPECT_TRUE((inData.tes_[0] == DfsOpt::Player{"Mark Andrews", "BAL", 7400, 17.8}));
  EXPECT_EQ(inData.dsts_.size(), 1);
  EXPECT_TRUE((inData.dsts_[0] == DfsOpt::Dst{"LV", 3300 , 13.0}));
}

TEST(ParserTests, SchemaValidationIllFormedDocument) {
  std::string_view inFile = "ill-formed.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SchemaValidationMissingRequirements) {
  std::string_view inFile = "missing-reqs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SchemaValidationMissingPlayerAttr) {
  std::string_view inFile = "missing-player-attr.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SchemaValidationMissingDstAttr) {
  std::string_view inFile = "missing-dst-attr.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SchemaValidationNegativeCost) {
  std::string_view inFile = "negative-cost.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SchemaValidationNegativePoints) {
  std::string_view inFile = "negative-points.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SanityCheckQbCountFailure) {
  std::string_view inFile = "sanity-check-qbs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SanityCheckFlexCountFailure) {
  std::string_view inFile = "sanity-check-flex.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SanityCheckRequiredQbsCountFailure) {
  std::string_view inFile = "sanity-check-req-qbs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

TEST(ParserTests, SanityCheckRequiredFlexCountFailure) {
  std::string_view inFile = "sanity-check-req-flex.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::exception);
}

