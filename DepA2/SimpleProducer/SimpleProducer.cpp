// ======================================================================
// \title  SimpleProducer.cpp
// \author emil
// \brief  cpp file for SimpleProducer component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA2/SimpleProducer/SimpleProducer.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace DepA2 {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SimpleProducer ::
    SimpleProducer(
        const char *const compName
    ) : SimpleProducerComponentBase(compName)
  {

  }

  void SimpleProducer ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    SimpleProducerComponentBase::init(queueDepth, instance);
  }

  SimpleProducer ::
    ~SimpleProducer()
  {

  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void SimpleProducer ::
    SET_NUM_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        F32 val1
    )
  {
    // TODO
    this->tlmWrite_VAL(val1); 
    this->log_ACTIVITY_LO_COMMAND_RECV(val1); 
    this->valOut_out(0, val1); 
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace DepA
