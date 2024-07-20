///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#include "prv/Prv.hh"

#include <iostream>

#include "odb/db.h"
#include "utl/Logger.h"

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

}  // namespace prv
