#include "DfsOpt_Outputter.hpp"
#include "DfsOpt_ProblemData.hpp"

#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <string_view>

void DfsOpt::Outputter::output(
  const std::string_view outFileName,
  const DfsOpt::OutputData& outData,
  const int nLineupsExpected,
  const double solveTime
) {
  std::unique_ptr<std::ofstream> fout;
  if (!outFileName.empty()) {
    fout = std::make_unique<std::ofstream>(outFileName.data());
  }
  std::ostream* out = fout ? fout.get() : &std::cout;
  
  *out << "dfs-opt produced the following " << outData.rosters_.size() << " lineups after "
    << solveTime << " seconds of solution time.\n";
  if (outData.rosters_.size() < nLineupsExpected) {
    *out << "NOTE: This is fewer than the number requested. Either not enough input player data was "
      << "provided (at least not enough cheaper players), or an reasonably large number  was requested.\n";
  }
  *out << '\n';

  using namespace std::string_view_literals;
  static constexpr auto divider = 
    "----------------------------------------------------------------------------------------------------\n\n"sv;

  int lineupCount = 0;
  for (const auto roster : outData.rosters_) {
    ++lineupCount;
    *out << divider;
    {
      std::ostringstream lnum, pts, cost, header;
      lnum << "Lineup #" << lineupCount << ":";
      pts << "Expected Points = " << std::fixed << std::setprecision(1) << roster.expectedPts_;
      cost << "Lineup Cost = " << roster.cost_;
      header << std::left << std::setw(20)  << lnum.str() << std::setw(40) << pts.str() << std::setw(30) << cost.str();
      *out << header.str() << '\n';
    }

    auto posHeader = [](std::ostream* out, const std::string_view posStr, bool firstPrint) {
      std::ostringstream header;
      header << std::left << std::setw(32) << posStr;
      if (firstPrint) {
        header << std::setw(6) << "Team" << std::right << std::setw(30)
          << "Expected Points" << std::setw(20) << "Cost";
      }
      *out << header.str() << '\n';
    };

    auto playerOut = [](std::ostream* out, const DfsOpt::Player& player, const bool isCaptain = false) {
      std::ostringstream name;
      name << player.name_;
      int salary = player.salary_;
      double pts = player.expectedPts_;
      if (isCaptain) {
        name << " (C)";
        salary *= 1.5;
        pts *= 1.5;
      }
      std::ostringstream ostr;
      ostr << std::left << "  " << std::setw(30) << name.str() << std::setw(6) << player.team_ << std::right
        << std::setw(30) << std::fixed << std::setprecision(1) << pts << std::setw(20) << salary;
      *out << ostr.str() << '\n';
    };

    auto dstOut = [](std::ostream* out, const DfsOpt::Dst& dst, const bool isCaptain = false) {
      std::ostringstream team;
      team << dst.team_;
      int salary = dst.salary_;
      double pts = dst.expectedPts_;
      if (isCaptain) {
        team << " (C)";
        salary *= 1.5;
        pts *= 1.5;
      }
      std::stringstream ostr;
      ostr << std::left << "  " << std::setw(36) << team.str() << std::right << std::setw(30)
        << std::fixed << std::setprecision(1) << pts << std::setw(20) << salary;
      *out << ostr.str() << '\n';
    };

    bool firstPrint = true;
    if (!roster.qbs_.empty()) {
      posHeader(out, "QBs:", firstPrint);
      firstPrint = false;
      for (const auto& player : roster.qbs_) {
        playerOut(out, player);
      }
    }
    if (!roster.rbs_.empty()) {
      posHeader(out, "RBs:", firstPrint);
      firstPrint = false;
      for (const auto& player : roster.rbs_) {
        playerOut(out, player);
      }
    }
    if (!roster.wrs_.empty()) {
      posHeader(out, "WRs:", firstPrint);
      firstPrint = false;
      for (const auto& player : roster.wrs_) {
        playerOut(out, player);
      }
    }
    if (!roster.tes_.empty()) {
      posHeader(out, "TEs:", firstPrint);
      firstPrint = false;
      for (const auto& player : roster.tes_) {
        playerOut(out, player);
      }
    }
    if (!roster.flex_.empty()) {
      posHeader(out, "FLEX:", firstPrint);
      int id = 0;
      for (const auto& player : roster.flex_) {
        playerOut(out, player, outData.captainMode_ && !roster.dstIsCaptain_ && id++ == 0);
      }
    }
    if (!roster.dsts_.empty()) {
      *out << "DST:\n";
      int id = 0;
      for (const auto& player : roster.dsts_) {
        dstOut(out, player,  outData.captainMode_ && roster.dstIsCaptain_ && id++ == 0);
      }
    }
    *out << '\n';
  }
}

