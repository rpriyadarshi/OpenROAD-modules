///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace odb {
class dbDatabase;
class dbBTerm;
class dbBlock;
};  // namespace odb

namespace sta {
class dbSta;
};

namespace utl {
class Logger;
};

namespace prv {

class Prv
{
 private:  // Data members
  // Global state
  odb::dbDatabase* db_;
  sta::dbSta* sta_;
  utl::Logger* logger_;

 public:  // Constructors/destructors/initializations
  Prv();

  void init(odb::dbDatabase* db, sta::dbSta* sta, utl::Logger* logger);

 public:  // Algorithms
  // Path tracing based on odb
  void tracePathToAllSinks(odb::dbBTerm* inputPort, odb::dbBlock* block);
  void tracePathToAllSinks(const std::string& inputPort);
};

}  // namespace prv
