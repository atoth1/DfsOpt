#include "DfsOpt_ProblemData.hpp"
#include "DfsOpt_Solver.hpp"

#include <algorithm>
#include <stdexcept>
#include <set>
#include <utility>
#include <variant>

namespace {
  enum class PosStatus {
    QB,
    RB,
    WR,
    TE,
    FLEX,
    DST
  };

  // Comparasion functor for min heap and descending sort by expected points of roster.
  struct RosterComp {
    bool operator()(const DfsOpt::Roster& l, const DfsOpt::Roster& r) const noexcept {
      return l.expectedPts_ > r.expectedPts_;
    };
  };

  void implStandard(
    const DfsOpt::InputData& data,
    const int nRosters,
    const int startId,
    const PosStatus pos,
    DfsOpt::Roster& current,
    std::vector<int>& qbIds,
    std::vector<int>& rbIds,
    std::vector<int>& wrIds,
    std::vector<int>& teIds,
    std::vector<int>& dstIds,
    std::vector<DfsOpt::Roster>& candidates
  ) {
    switch (pos) {
      case PosStatus::QB: {
        if (data.numQbs_ == 0) {
          implStandard(data, nRosters, 0, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.qbs_.size(); ++id) {
            if (current.cost_ + data.qbs_[id].salary_ > data.budget_) break;
            current.qbs_.push_back(data.qbs_[id]);
            current.cost_ += data.qbs_[id].salary_;
            current.expectedPts_ += data.qbs_[id].expectedPts_;
            qbIds.push_back(id);
            if (qbIds.size() == data.numQbs_) {
              implStandard(data, nRosters, 0, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              implStandard(data, nRosters, id+1, PosStatus::QB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.qbs_.pop_back();
            current.cost_ -= data.qbs_[id].salary_;
            current.expectedPts_ -= data.qbs_[id].expectedPts_;
            qbIds.pop_back();
          }
        }
        break;
      } case PosStatus::RB: {
        if (data.numRbs_ == 0) {
          implStandard(data, nRosters, 0, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.rbs_.size(); ++id) {
            if (current.cost_ + data.rbs_[id].salary_ > data.budget_) break;
            current.rbs_.push_back(data.rbs_[id]);
            current.cost_ += data.rbs_[id].salary_;
            current.expectedPts_ += data.rbs_[id].expectedPts_;
            rbIds.push_back(id);
            if (rbIds.size() == data.numRbs_) {
              implStandard(data, nRosters, 0, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              implStandard(data, nRosters, id+1, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.rbs_.pop_back();
            current.cost_ -= data.rbs_[id].salary_;
            current.expectedPts_ -= data.rbs_[id].expectedPts_;
            rbIds.pop_back();
          }
        }
        break;
      } case PosStatus::WR: {
        if (data.numWrs_ == 0) {
          implStandard(data, nRosters, 0, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.wrs_.size(); ++id) {
            if (current.cost_ + data.wrs_[id].salary_ > data.budget_) break;
            current.wrs_.push_back(data.wrs_[id]);
            current.cost_ += data.wrs_[id].salary_;
            current.expectedPts_ += data.wrs_[id].expectedPts_;
            wrIds.push_back(id);
            if (wrIds.size() == data.numWrs_) {
              implStandard(data, nRosters, 0, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              implStandard(data, nRosters, id+1, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.wrs_.pop_back();
            current.cost_ -= data.wrs_[id].salary_;
            current.expectedPts_ -= data.wrs_[id].expectedPts_;
            wrIds.pop_back();
          }
        }
        break;
      } case PosStatus::TE: {
        if (data.numTes_ == 0) {
          implStandard(data, nRosters, 0, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.tes_.size(); ++id) {
            if (current.cost_ + data.tes_[id].salary_ > data.budget_) break;
            current.tes_.push_back(data.tes_[id]);
            current.cost_ += data.tes_[id].salary_;
            current.expectedPts_ += data.tes_[id].expectedPts_;
            teIds.push_back(id);
            if (teIds.size() == data.numTes_) {
              implStandard(data, nRosters, 0, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              implStandard(data, nRosters, id+1, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.tes_.pop_back();
            current.cost_ -= data.tes_[id].salary_;
            current.expectedPts_ -= data.tes_[id].expectedPts_;
            teIds.pop_back();
          }
        }
        break;
      } case PosStatus::DST: {
        if (data.numDsts_ == 0) {
          implStandard(data, nRosters, 0, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.dsts_.size(); ++id) {
            if (current.cost_ + data.dsts_[id].salary_ > data.budget_) break;
            current.dsts_.push_back(data.dsts_[id]);
            current.cost_ += data.dsts_[id].salary_;
            current.expectedPts_ += data.dsts_[id].expectedPts_;
            dstIds.push_back(id);
            if (dstIds.size() == data.numDsts_) {
              implStandard(data, nRosters, 0, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              implStandard(data, nRosters, id+1, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.dsts_.pop_back();
            current.cost_ -= data.dsts_[id].salary_;
            current.expectedPts_ -= data.dsts_[id].expectedPts_;
            dstIds.pop_back();
          }
        }
        break;
      } case PosStatus::FLEX: {
        auto updateCandidates = [&]() {
          RosterComp comp{};
          if (candidates.size() < nRosters) {
            candidates.push_back(current);
            std::make_heap(candidates.begin(), candidates.end(), comp);
          } else if (comp(current, candidates[0])) {
            std::pop_heap(candidates.begin(), candidates.end(), comp);
            candidates.back() = current;
            std::make_heap(candidates.begin(), candidates.end(), comp);
          }
        };

        if (data.numFlex_ == 0) {
          updateCandidates();
        } else {
          // Look for RB to flex
          int id = startId;
          if (!rbIds.empty() && id <= rbIds.back()) {
            id = rbIds.back() + 1;
          }
          for (; id < data.rbs_.size(); ++id) {
            if (current.cost_ + data.rbs_[id].salary_ > data.budget_) {
              id = data.rbs_.size();
              break;
            }
            current.flex_.push_back(data.rbs_[id]);
            current.cost_ += data.rbs_[id].salary_;
            current.expectedPts_ += data.rbs_[id].expectedPts_;
            rbIds.push_back(id);
            if (current.flex_.size() == data.numFlex_) {
              updateCandidates();
            } else {
              implStandard(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.flex_.pop_back();
            current.cost_ -= data.rbs_[id].salary_;
            current.expectedPts_ -= data.rbs_[id].expectedPts_;
            rbIds.pop_back();
          }

          // Look for WR to flex
          if (!wrIds.empty() && id <= data.rbs_.size() + wrIds.back()) {
            id = data.rbs_.size() + wrIds.back() + 1;
          }
          for (; id < data.rbs_.size() + data.wrs_.size(); ++id) {
            int posId = id - data.rbs_.size();
            if (current.cost_ + data.wrs_[posId].salary_ > data.budget_) {
              id = data.rbs_.size() + data.wrs_.size();
              break;
            }
            current.flex_.push_back(data.wrs_[posId]);
            current.cost_ += data.wrs_[posId].salary_;
            current.expectedPts_ += data.wrs_[posId].expectedPts_;
            wrIds.push_back(posId);
            if (current.flex_.size() == data.numFlex_) {
              updateCandidates();
            } else {
              implStandard(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.flex_.pop_back();
            current.cost_ -= data.wrs_[posId].salary_;
            current.expectedPts_ -= data.wrs_[posId].expectedPts_;
            wrIds.pop_back();
          }

          // Look for TE to flex
          if (!teIds.empty() && id <= data.rbs_.size() + data.wrs_.size() + teIds.back()) {
            id = data.rbs_.size() + data.wrs_.size() + teIds.back() + 1;
          }
          for (; id < data.rbs_.size() + data.wrs_.size() + data.tes_.size(); ++id) {
            int posId = id - data.rbs_.size() - data.wrs_.size();
            if (current.cost_ + data.tes_[posId].salary_ > data.budget_) {
              break;
            }
            current.flex_.push_back(data.tes_[posId]);
            current.cost_ += data.tes_[posId].salary_;
            current.expectedPts_ += data.tes_[posId].expectedPts_;
            teIds.push_back(posId);
            if (current.flex_.size() == data.numFlex_) {
              updateCandidates();
            } else {
              implStandard(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            }
            current.flex_.pop_back();
            current.cost_ -= data.tes_[posId].salary_;
            current.expectedPts_ -= data.tes_[posId].expectedPts_;
            teIds.pop_back();
          }
        }
        break;
      }
    }
  }

  void implCaptain(
    const int rosterSize,
    const int rosterSlot,
    const int captainId,
    const int startId,
    const int budget,
    const int nRosters,
    const std::vector<std::variant<DfsOpt::Player, DfsOpt::Dst>> all,
    DfsOpt::Roster& current,
    std::vector<DfsOpt::Roster>& candidates
  ) {
    if (rosterSlot == rosterSize) {
      RosterComp comp{};
      if (candidates.size() < nRosters) {
        candidates.push_back(current);
        std::make_heap(candidates.begin(), candidates.end(), comp);
      } else if (comp(current, candidates[0])) {
        std::pop_heap(candidates.begin(), candidates.end(), comp);
        candidates.back() = current;
        std::make_heap(candidates.begin(), candidates.end(), comp);
      }
    } else {
      for (int id = startId; id < all.size(); ++id) {
        if (std::holds_alternative<DfsOpt::Player>(all[id])) {
          const auto& player = std::get<DfsOpt::Player>(all[id]);
          if ((rosterSlot == 0 && 1.5 * player.salary_ > budget) || current.cost_ + player.salary_ > budget) break;
          current.flex_.push_back(player);
          if (rosterSlot == 0) {
            current.dstIsCaptain_ = false;
            current.cost_ += 1.5 * player.salary_;
            current.expectedPts_ += 1.5 * player.expectedPts_;
            implCaptain(rosterSize, 1, id, 0, budget, nRosters, all, current, candidates);
            current.cost_ -= 1.5 * player.salary_;
            current.expectedPts_ -= 1.5 * player.expectedPts_;
          } else if (id != captainId) {
            current.cost_ += player.salary_;
            current.expectedPts_ += player.expectedPts_;
            implCaptain(rosterSize, rosterSlot+1, captainId, id+1, budget, nRosters, all, current, candidates);
            current.cost_ -= player.salary_;
            current.expectedPts_ -= player.expectedPts_;
          }
          current.flex_.pop_back();
        } else {
          const auto& dst = std::get<DfsOpt::Dst>(all[id]);
          if ((rosterSlot == 0 && 1.5 * dst.salary_ > budget) || current.cost_ + dst.salary_ > budget) break;
          current.dsts_.push_back(dst);
          if (rosterSlot == 0) {
            current.dstIsCaptain_ = true;
            current.cost_ += 1.5 * dst.salary_;
            current.expectedPts_ += 1.5 * dst.expectedPts_;
            implCaptain(rosterSize, 1, id, 0, budget, nRosters, all, current, candidates);
            current.cost_ -= 1.5 * dst.salary_;
            current.expectedPts_ -= 1.5 * dst.expectedPts_;
          } else if (id != captainId) {
            current.cost_ += dst.salary_;
            current.expectedPts_ += dst.expectedPts_;
            implCaptain(rosterSize, rosterSlot+1, captainId, id+1, budget, nRosters, all, current, candidates);
            current.cost_ -= dst.salary_;
            current.expectedPts_ -= dst.expectedPts_;
          }
          current.dsts_.pop_back();
        }
      }
    }
  }
}

DfsOpt::OutputData DfsOpt::Solver::solve(
  const int nLineups,
  DfsOpt::InputData& data
) {
  int reqCost = 0;
  double reqPts = 0.;
  std::vector<DfsOpt::Player> reqFlex{};
  auto updateForRequired = [&](std::vector<DfsOpt::Player>& container, int& startingCount) {
    int flexed = 0;
    for (const auto& player : container) {
      if (startingCount > 0) {
        --startingCount;
      } else {
        // Parser checks that required qb count < starting qb count, so this can only be rb/wr/te.
        ++flexed;
        --data.numFlex_;
        reqFlex.push_back(player);
      }
      data.budget_ -= player.salary_;
      reqCost += player.salary_;
      reqPts += player.expectedPts_;
    }
    for (int i = 0; i < flexed; ++i) container.pop_back();
  };
  auto updateForRequiredDst = [&]() {
    for (const auto& dst : data.reqDsts_) {
      --data.numDsts_;
      data.budget_ -= dst.salary_;
      reqCost += dst.salary_;
      reqPts += dst.expectedPts_;
    }
  };
  updateForRequired(data.reqQbs_, data.numQbs_);
  updateForRequired(data.reqRbs_, data.numRbs_);
  updateForRequired(data.reqWrs_, data.numWrs_);
  updateForRequired(data.reqTes_, data.numTes_);
  updateForRequiredDst();
  if (data.budget_ < 0) {
    throw std::runtime_error("ERROR: Salaries for required players already exceeds allowed budget.");
  }

  DfsOpt::OutputData ret{{}, data.captainMode_};
  ret.rosters_.reserve(nLineups);
  Roster candidate{};
  if (data.captainMode_) {
    auto consolidate = [&]() {
      // Could probably do this more efficiently, but not performance critical
      using entry_type = std::variant<DfsOpt::Player, DfsOpt::Dst>;
      int total = data.qbs_.size() + data.rbs_.size() + data.wrs_.size()
        + data.tes_.size() + data.ks_.size() + data.dsts_.size();
      std::vector<entry_type> all{};
      all.reserve(total);
      std::for_each(data.qbs_.cbegin(), data.qbs_.cend(), [&](const DfsOpt::Player& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Player>, entry));
      });
      std::for_each(data.rbs_.cbegin(), data.rbs_.cend(), [&](const DfsOpt::Player& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Player>, entry));
      });
      std::for_each(data.wrs_.cbegin(), data.wrs_.cend(), [&](const DfsOpt::Player& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Player>, entry));
      });
      std::for_each(data.tes_.cbegin(), data.tes_.cend(), [&](const DfsOpt::Player& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Player>, entry));
      });
      std::for_each(data.ks_.cbegin(), data.ks_.cend(), [&](const DfsOpt::Player& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Player>, entry));
      });
      std::for_each(data.dsts_.cbegin(), data.dsts_.cend(), [&](const DfsOpt::Dst& entry) {
        all.push_back(entry_type(std::in_place_type<DfsOpt::Dst>, entry));
      });
      std::sort(all.begin(), all.end(), [](const entry_type& l, const entry_type& r) {
        if (std::holds_alternative<DfsOpt::Player>(l)) {
          if (std::holds_alternative<DfsOpt::Player>(r)) {
            return std::get<DfsOpt::Player>(l).salary_ < std::get<DfsOpt::Player>(r).salary_;
          } else {
            return std::get<DfsOpt::Player>(l).salary_ < std::get<DfsOpt::Dst>(r).salary_;
          }
        } else {
          if (std::holds_alternative<DfsOpt::Player>(r)) {
            return std::get<DfsOpt::Dst>(l).salary_ < std::get<DfsOpt::Player>(r).salary_;
          } else {
            return std::get<DfsOpt::Dst>(l).salary_ < std::get<DfsOpt::Dst>(r).salary_;
          }
        }
      });
      return all;
    };
    implCaptain(data.numFlex_, 0, -1, 0, data.budget_, nLineups, consolidate(), candidate, ret.rosters_);
  } else {
    std::vector<int> qbIds{}, rbIds{}, wrIds{}, teIds{}, dstIds{};
    implStandard(data, nLineups, 0, PosStatus::QB, candidate, qbIds, rbIds, wrIds, teIds, dstIds, ret.rosters_);
  }

  auto addRequired = [&](
    DfsOpt::Roster& roster,
    const DfsOpt::InputData& data,
    const std::vector<DfsOpt::Player>& reqFlex
  ) {
    roster.cost_ += reqCost;
    roster.expectedPts_ += reqPts;
    roster.qbs_.insert(roster.qbs_.cend(), data.reqQbs_.cbegin(), data.reqQbs_.cend());
    roster.rbs_.insert(roster.rbs_.cend(), data.reqRbs_.cbegin(), data.reqRbs_.cend());
    roster.wrs_.insert(roster.wrs_.cend(), data.reqWrs_.cbegin(), data.reqWrs_.cend());
    roster.tes_.insert(roster.tes_.cend(), data.reqTes_.cbegin(), data.reqTes_.cend());
    roster.flex_.insert(roster.flex_.cend(), reqFlex.cbegin(), reqFlex.cend());
    roster.dsts_.insert(roster.dsts_.cend(), data.reqDsts_.cbegin(), data.reqDsts_.cend());
  };
  for (auto& roster : ret.rosters_) {
    addRequired(roster, data, reqFlex);
  }

  std::sort(ret.rosters_.begin(), ret.rosters_.end(), RosterComp{});
  return ret;
}