///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "db_sta/dbSta.hh"
#include "odb/db.h"
#include "utl/Logger.h"

namespace prv {

class Prv
{
 public:
  Prv();

  void init(odb::dbDatabase* db, sta::dbSta* sta, utl::Logger* logger);

 private:
  // Global state
  odb::dbDatabase* db_;
  sta::dbSta* sta_;
  utl::Logger* logger_;
};

}  // namespace prv
