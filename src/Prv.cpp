///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <queue>
#include <unordered_set>

#include "db_sta/dbSta.hh"
#include "odb/db.h"
#include "utl/Logger.h"

#include "prv/Prv.hh"

namespace prv {
  
class TracerCallbacks 
{
private:

public: // Constructors/destructors.
  TracerCallbacks() {}

public: // Callbacks
  void applyNet(odb::dbNet* net)
  {
    std::cout << "Net: " << net->getName() << std::endl;
  }

  void applyInst(odb::dbInst* inst)
  {
      std::cout << "  Instance: " << inst->getName() << std::endl;
  }

  void applyBTerm(odb::dbBTerm* bterm)
  {
      std::cout << "  Output Port: " << bterm->getName() << std::endl;
  }
};

Prv::Prv()
{
}

void Prv::init(odb::dbDatabase* db, sta::dbSta* sta, utl::Logger* logger)
{
  db_ = db;
  logger_ = logger;
  sta_ = sta;
}

void Prv::tracePathToAllSinks(odb::dbBTerm* inputPort, odb::dbBlock* block, TracerCallbacks& cb)
{
  std::unordered_set<odb::dbNet*> visitedNets;
  std::unordered_set<odb::dbInst*> visitedInsts;

  std::queue<odb::dbNet*> netQueue;
  netQueue.push(inputPort->getNet());

  while (!netQueue.empty()) {
    auto net = netQueue.front();
    netQueue.pop();

    if (visitedNets.find(net) != visitedNets.end()) {
      continue;
    }
    visitedNets.insert(net);

    cb.applyNet(net);

    auto iterms = net->getITerms();
    for (auto iterm : iterms) {
      auto inst = iterm->getInst();
      if (visitedInsts.find(inst) != visitedInsts.end()) {
        continue;
      }
      visitedInsts.insert(inst);

      cb.applyInst(inst);

      auto instIterms = inst->getITerms();
      for (auto inst_iterm : instIterms) {
        auto instNet = inst_iterm->getNet();
        if (instNet && visitedNets.find(instNet) == visitedNets.end()) {
          netQueue.push(instNet);
        }
      }
    }

    auto bterms = net->getBTerms();
    for (auto bterm : bterms) {
      cb.applyBTerm(bterm);
    }
  }
}

void Prv::tracePathToAllSinks(const std::string& inputPortName)
{
  auto chip = db_->getChip();
  auto block = chip->getBlock();

  // Get input inputPort
  auto inputPort = block->findBTerm(inputPortName.c_str());

  if (inputPort) {
    TracerCallbacks cb;
    tracePathToAllSinks(inputPort, block, cb);
  } else {
    std::cerr << "Input port \"" << inputPortName << "\" not found."
              << std::endl;
  }
}

}  // namespace prv
