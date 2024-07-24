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

#include "sta/Sta.hh"
#include "sta/Network.hh"
#include "sta/NetworkClass.hh"
#include "sta/PortDirection.hh"
#include "db_sta/dbNetwork.hh"

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

  void applyNet(sta::Network *network, sta::Net *net)
  {
    std::cout << "Net: " << network->pathName(net) << std::endl;
  }

  void applyInst(odb::dbInst* inst)
  {
      std::cout << "  Instance: " << inst->getName() << std::endl;
  }

  void applyInst(sta::Network *network, sta::Instance *inst)
  {
      std::cout << "  Instance: " << network->pathName(inst) << std::endl;
  }

  void applyPort(odb::dbBTerm* bterm)
  {
      std::cout << "  Output Port: " << bterm->getName() << std::endl;
  }

  void applyPort(sta::Network *network, const sta::Pin *pin)
  {
      std::cout << "  Output Port: " << network->pathName(pin) << std::endl;
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

void Prv::tracePathToAllSinksDb(odb::dbBTerm* inputPort, odb::dbBlock* block, TracerCallbacks& cb)
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
      cb.applyPort(bterm);
    }
  }
}

void Prv::tracePathToAllSinksSta(sta::Net* startNet, sta::Network* network, TracerCallbacks& cb) {
    std::unordered_set<sta::Net*> visitedNets;
    std::unordered_set<sta::Instance*> visitedInsts;

    std::queue<sta::Net*> netQueue;
    if (startNet) {
        netQueue.push(startNet);
    }

    while (!netQueue.empty()) {
        auto net = netQueue.front();
        netQueue.pop();

        if (visitedNets.find(net) != visitedNets.end()) {
            continue;
        }
        visitedNets.insert(net);

        cb.applyNet(network, net);

        // Traverse pins connected to the net
        auto pinIter = network->pinIterator(net);
        while (pinIter->hasNext()) {
            auto pin = pinIter->next();
            auto inst = network->instance(pin);

            if (network->isTopLevelPort(pin)) {
                // Check if the pin is an output port
                if (network->direction(pin) == sta::PortDirection::output()) {
                    cb.applyPort(network, pin);
                }
            } else {
                if (visitedInsts.find(inst) != visitedInsts.end()) {
                    continue;
                }
                visitedInsts.insert(inst);

                cb.applyInst(network, inst);

                // Add all output nets of this instance to the queue
                auto instPinIter = network->pinIterator(inst);
                while (instPinIter->hasNext()) {
                    auto instPin = instPinIter->next();
                    auto instNet = network->net(instPin);
                    if (instNet && visitedNets.find(instNet) == visitedNets.end()) {
                        netQueue.push(instNet);
                    }
                }
                delete instPinIter;
            }
        }
        delete pinIter;
    }
}


void Prv::tracePathToAllSinksDb(const std::string& inputPortName)
{
  auto chip = db_->getChip();
  auto block = chip->getBlock();

  // Get input inputPort
  auto inputPort = block->findBTerm(inputPortName.c_str());

  if (inputPort) {
    TracerCallbacks cb;
    tracePathToAllSinksDb(inputPort, block, cb);
  } else {
    std::cerr << "Input port \"" << inputPortName << "\" not found."
              << std::endl;
  }
}

void Prv::tracePathToAllSinksSta(const std::string& inputPortName)
{
  auto network = sta_->network();
  auto dbnetwork = sta_->getDbNetwork();

  auto chip = db_->getChip();
  auto block = chip->getBlock();

  // Get input inputPort. Interestingly, this has no net in sta, hence we get stuff from odb
  auto inputPort = block->findBTerm(inputPortName.c_str());
  auto startNet = inputPort->getNet();

  if (startNet) {
    TracerCallbacks cb;
    tracePathToAllSinksSta(dbnetwork->dbToSta(startNet), network, cb);
  } else {
    std::cerr << "Input port \"" << inputPortName << "\" not found."
              << std::endl;
  }
}

}  // namespace prv
