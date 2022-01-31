// ======================================================================
// \title  SimpleProducer.hpp
// \author emil
// \brief  cpp file for SimpleProducer test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "Tester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
#if FW_OBJECT_NAMES == 1
      SimpleProducerGTestBase("Tester", MAX_HISTORY_SIZE),
      component("SimpleProducer")
#else
      SimpleProducerGTestBase(MAX_HISTORY_SIZE),
      component()
#endif
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
    toDo()
  {
    // TODO
  }

  void Tester::testInvokeSET_NUM(){
    this->invokeSET_NUM(1, 0); 
    this->verifySET_NUM(1, 0, 0, 1); 

    this->invokeSET_NUM(1123, 1); 
    this->verifySET_NUM(1123, 1, 1, 2); 
    
  }
 
  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_valOut_handler(
        const NATIVE_INT_TYPE portNum,
        F32 val1
    )
  {
    this->pushFromPortEntry_valOut(val1);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // cmdIn
    this->connect_to_cmdIn(
        0,
        this->component.get_cmdIn_InputPort(0)
    );

    // cmdRegOut
    this->component.set_cmdRegOut_OutputPort(
        0,
        this->get_from_cmdRegOut(0)
    );

    // cmdResponseOut
    this->component.set_cmdResponseOut_OutputPort(
        0,
        this->get_from_cmdResponseOut(0)
    );

    // eventOut
    this->component.set_eventOut_OutputPort(
        0,
        this->get_from_eventOut(0)
    );

    // textEventOut
    this->component.set_textEventOut_OutputPort(
        0,
        this->get_from_textEventOut(0)
    );

    // timeGetOut
    this->component.set_timeGetOut_OutputPort(
        0,
        this->get_from_timeGetOut(0)
    );

    // tlmOut
    this->component.set_tlmOut_OutputPort(
        0,
        this->get_from_tlmOut(0)
    );

    // valOut
    this->component.set_valOut_OutputPort(
        0,
        this->get_from_valOut(0)
    );

  


  }

  void Tester::invokeSET_NUM(F32 input, U32 cmdSeq){
    this->sendCmd_SET_NUM(0, cmdSeq, input); 
    this->component.doDispatch();
  } 

  void Tester::verifySET_NUM(F32 input, U32 cmdSeq, U32 index, U32 expected_size){

    //Verify Command Response
    ASSERT_CMD_RESPONSE_SIZE(expected_size); 
    ASSERT_CMD_RESPONSE(index, SimpleProducer::OPCODE_SET_NUM, cmdSeq, Fw::CmdResponse::OK); 

    //Verify port operation
    ASSERT_FROM_PORT_HISTORY_SIZE(expected_size); 
    ASSERT_from_valOut(index, input); 
    ASSERT_from_valOut_SIZE(expected_size); 

    //Verify Telemetry Operation
    ASSERT_TLM_SIZE(expected_size);
    ASSERT_TLM_VAL_SIZE(expected_size); 
    ASSERT_TLM_VAL(index, input); 

    //Verify Event Operation
    ASSERT_EVENTS_SIZE(expected_size); 
    ASSERT_EVENTS_COMMAND_RECV_SIZE(expected_size); 
    ASSERT_EVENTS_COMMAND_RECV(index, input); 
  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

} // end namespace Ref
