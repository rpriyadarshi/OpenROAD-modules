///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <unordered_set>
#include <queue>

#include "db_sta/dbSta.hh"
#include "odb/db.h"
#include "utl/Logger.h"

#include "prv/Prv.hh"

namespace prv {

Prv::Prv()
{
}

void Prv::init(odb::dbDatabase* db, sta::dbSta* sta, utl::Logger* logger)
{
  db_ = db;
  logger_ = logger;
  sta_ = sta;
}

void Prv::tracePathToAllSinks(odb::dbBTerm* input_port, odb::dbBlock* block)
{
    std::unordered_set<odb::dbNet*> visitedNets;
    std::unordered_set<odb::dbInst*> visitedInsts;

    std::queue<odb::dbNet*> netQueue;
    netQueue.push(input_port->getNet());

    while (!netQueue.empty()) {
        auto net = netQueue.front();
        netQueue.pop();

        if (visitedNets.find(net) != visitedNets.end()) {
            continue;
        }
        visitedNets.insert(net);

        std::cout << "Net: " << net->getName() << std::endl;

        auto iterms = net->getITerms();
        for (auto iterm : iterms) {
            auto inst = iterm->getInst();
            if (visitedInsts.find(inst) != visitedInsts.end()) {
                continue;
            }
            visitedInsts.insert(inst);

            std::cout << "  Instance: " << inst->getName() << std::endl;

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
            std::cout << "  Output Port: " << bterm->getName() << std::endl;
        }
    }
}

  void Prv::tracePathToAllSinks(const std::string& portName)
  {
    auto chip = db_->getChip();
    auto block = chip->getBlock();

    // Get input port
    auto port = block->findBTerm(portName.c_str());

    if (port) {
        tracePathToAllSinks(port, block);
    } else {
        std::cerr << "Input port \"" << portName << "\" not found." << std::endl;
    }
  }

}  // namespace prv
