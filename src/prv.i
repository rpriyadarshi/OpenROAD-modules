///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

%module prv

%{

#include "ord/OpenRoad.hh"
#include "prv/Prv.hh"

prv::Prv * getPrv()
{
  return ord::OpenRoad::openRoad()->getPrv();
}

%}

%inline
%{
  void trace_path_to_all_sinks_db(const char* input_port)
  {
    if (input_port != nullptr) {
      getPrv()->tracePathToAllSinksDb(input_port);
    }
  }

  void trace_path_to_all_sinks_sta(const char* input_port)
  {
    if (input_port != nullptr) {
      getPrv()->tracePathToAllSinksSta(input_port);
    }
  }
%}  // inline
