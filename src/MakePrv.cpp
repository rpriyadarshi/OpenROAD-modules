///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2024, Daedel Inc.
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#include "prv/MakePrv.hh"

#include "ord/OpenRoad.hh"
#include "prv/Prv.hh"

namespace sta {
extern const char* prv_tcl_inits[];
extern void evalTclInit(Tcl_Interp*, const char*[]);
}  // namespace sta

namespace prv {

extern "C" {
extern int Prv_Init(Tcl_Interp* interp);
}

prv::Prv* makePrv()
{
  return new prv::Prv();
}

void initPrv(ord::OpenRoad* openroad)
{
  Tcl_Interp* interp = openroad->tclInterp();
  Prv_Init(interp);
  sta::evalTclInit(interp, sta::prv_tcl_inits);
  openroad->getPrv()->init(
      openroad->getDb(), openroad->getSta(), openroad->getLogger());
}

void deletePrv(prv::Prv* prv)
{
  delete prv;
}

}  // namespace prv
