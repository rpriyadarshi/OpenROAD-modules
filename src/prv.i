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
  void trace_path_to_all_sinks(const char* input_port)
  {
    if (input_port != nullptr) {
      getPrv()->tracePathToAllSinks(input_port);
    }
  }
%}  // inline
