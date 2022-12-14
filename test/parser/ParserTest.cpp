#include "DfsOpt.hpp"
#include "gtest/gtest.h"
#include "libxml++/libxml++.h"

#include <cmath>
#include <stdexcept>
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
  EXPECT_FALSE(inData.captainMode_);

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
  EXPECT_FALSE(inData.captainMode_);

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
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::parse_error);
}

TEST(ParserTests, SchemaValidationMissingRequirements) {
  std::string_view inFile = "missing-reqs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::validity_error);
}

TEST(ParserTests, SchemaValidationMissingPlayerAttr) {
  std::string_view inFile = "missing-player-attr.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::validity_error);
}

TEST(ParserTests, SchemaValidationMissingDstAttr) {
  std::string_view inFile = "missing-dst-attr.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::validity_error);
}

TEST(ParserTests, SchemaValidationNegativeCost) {
  std::string_view inFile = "negative-cost.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::validity_error);
}

TEST(ParserTests, SchemaValidationNegativePoints) {
  std::string_view inFile = "negative-points.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), xmlpp::validity_error);
}

TEST(ParserTests, SanityCheckQbCountFailure) {
  std::string_view inFile = "sanity-check-qbs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}

TEST(ParserTests, SanityCheckFlexCountFailure) {
  std::string_view inFile = "sanity-check-flex.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}

TEST(ParserTests, SanityCheckRequiredQbsCountFailure) {
  std::string_view inFile = "sanity-check-req-qbs.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}

TEST(ParserTests, SanityCheckRequiredFlexCountFailure) {
  std::string_view inFile = "sanity-check-req-flex.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}

TEST(ParsreTests, CaptainMode) {
  std::string_view inFile = "captain-mode.xml";
  DfsOpt::InputData inData{};
  EXPECT_NO_THROW(inData = DfsOpt::XmlParser::parse(inFile));
  EXPECT_EQ(inData.budget_, 50000);
  EXPECT_EQ(inData.numQbs_, 0);
  EXPECT_EQ(inData.numRbs_, 0);
  EXPECT_EQ(inData.numWrs_, 0);
  EXPECT_EQ(inData.numTes_, 0);
  EXPECT_EQ(inData.numFlex_, 6);
  EXPECT_EQ(inData.numDsts_, 0);
  EXPECT_TRUE(inData.captainMode_);

  // Only check kickers here, other poisition parsing is unchanged
  EXPECT_EQ(inData.ks_.size(), 2);
  EXPECT_TRUE((inData.ks_[0] == DfsOpt::Player{"Ryan Succop", "TB", 3800, 7.3}));
  EXPECT_TRUE((inData.ks_[1] == DfsOpt::Player{"Justin Tucker", "BAL", 4000, 9.0}));
}

TEST(ParserTests, CaptainModeNonFlex) {
  std::string_view inFile = "captain-mode-nonflex.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}

TEST(ParserTests, CaptainModeRequiredPlayers) {
  std::string_view inFile = "captain-mode-req-players.xml";
  DfsOpt::InputData inData{};
  EXPECT_THROW(inData = DfsOpt::XmlParser::parse(inFile), std::runtime_error);
}
