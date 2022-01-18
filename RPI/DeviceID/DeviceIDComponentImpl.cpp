// ======================================================================
// \title  DeviceIDComponentImpl.cpp
// \author emil
// \brief  cpp file for DeviceID component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <RPI/DeviceID/DeviceIDComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Rpi {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  DeviceIDComponentImpl ::
    DeviceIDComponentImpl(
        const char *const compName
    ) : DeviceIDComponentBase(compName)
  {

  }

  void DeviceIDComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    DeviceIDComponentBase::init(instance);
  }

  DeviceIDComponentImpl ::
    ~DeviceIDComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void DeviceIDComponentImpl ::
    RD_GET_ID_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    // TODO

    DeviceID = "D00" 

    
    this->cmdResponse_out(opCode,cmdSeq,Fw::COMMAND_OK);
  }

} // end namespace Rpi
