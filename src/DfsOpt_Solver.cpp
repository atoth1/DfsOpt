#include "DfsOpt_ProblemData.hpp"
#include "DfsOpt_Solver.hpp"

#include <algorithm>
#include <set>
#include <utility>

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

  void impl(
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
          impl(data, nRosters, 0, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.qbs_.size(); ++id) {
            if (current.cost_ + data.qbs_[id].salary_ > data.budget_) break;
            current.qbs_.push_back(data.qbs_[id]);
            current.cost_ += data.qbs_[id].salary_;
            current.expectedPts_ += data.qbs_[id].expectedPts_;
            qbIds.push_back(id);
            if (qbIds.size() == data.numQbs_) {
              impl(data, nRosters, 0, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              impl(data, nRosters, id+1, PosStatus::QB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
          impl(data, nRosters, 0, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.rbs_.size(); ++id) {
            if (current.cost_ + data.rbs_[id].salary_ > data.budget_) break;
            current.rbs_.push_back(data.rbs_[id]);
            current.cost_ += data.rbs_[id].salary_;
            current.expectedPts_ += data.rbs_[id].expectedPts_;
            rbIds.push_back(id);
            if (rbIds.size() == data.numRbs_) {
              impl(data, nRosters, 0, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              impl(data, nRosters, id+1, PosStatus::RB, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
          impl(data, nRosters, 0, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.wrs_.size(); ++id) {
            if (current.cost_ + data.wrs_[id].salary_ > data.budget_) break;
            current.wrs_.push_back(data.wrs_[id]);
            current.cost_ += data.wrs_[id].salary_;
            current.expectedPts_ += data.wrs_[id].expectedPts_;
            wrIds.push_back(id);
            if (wrIds.size() == data.numWrs_) {
              impl(data, nRosters, 0, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              impl(data, nRosters, id+1, PosStatus::WR, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
          impl(data, nRosters, 0, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.tes_.size(); ++id) {
            if (current.cost_ + data.tes_[id].salary_ > data.budget_) break;
            current.tes_.push_back(data.tes_[id]);
            current.cost_ += data.tes_[id].salary_;
            current.expectedPts_ += data.tes_[id].expectedPts_;
            teIds.push_back(id);
            if (teIds.size() == data.numTes_) {
              impl(data, nRosters, 0, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              impl(data, nRosters, id+1, PosStatus::TE, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
          impl(data, nRosters, 0, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
        } else {
          for (int id = startId; id < data.dsts_.size(); ++id) {
            if (current.cost_ + data.dsts_[id].salary_ > data.budget_) break;
            current.dsts_.push_back(data.dsts_[id]);
            current.cost_ += data.dsts_[id].salary_;
            current.expectedPts_ += data.dsts_[id].expectedPts_;
            dstIds.push_back(id);
            if (dstIds.size() == data.numDsts_) {
              impl(data, nRosters, 0, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
            } else {
              impl(data, nRosters, id+1, PosStatus::DST, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
              impl(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
              impl(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
              impl(data, nRosters, id+1, PosStatus::FLEX, current, qbIds, rbIds, wrIds, teIds, dstIds, candidates);
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
}

DfsOpt::OutputData DfsOpt::Solver::solve(
  const int nLineups,
  const DfsOpt::InputData& data
) {
  DfsOpt::OutputData ret{};
  ret.rosters_.reserve(nLineups);
  Roster candidate{};
  std::vector<int> qbIds{}, rbIds{}, wrIds{}, teIds{}, dstIds{};
  impl(data, nLineups, 0, PosStatus::QB, candidate, qbIds, rbIds, wrIds, teIds, dstIds, ret.rosters_);
  std::sort(ret.rosters_.begin(), ret.rosters_.end(), RosterComp{});
  return ret;
}