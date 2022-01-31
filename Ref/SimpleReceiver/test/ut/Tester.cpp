// ======================================================================
// \title  SimpleReceiver.hpp
// \author emil
// \brief  cpp file for SimpleReceiver test harness implementation class
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
      SimpleReceiverGTestBase("Tester", MAX_HISTORY_SIZE),
      component("SimpleReceiver")
#else
      SimpleReceiverGTestBase(MAX_HISTORY_SIZE),
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

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {
     // valIn
    this->connect_to_valIn(
        0,
        this->component.get_valIn_InputPort(0)
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




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

  void Tester :: testSingle(){
    F32 test_value; 

    test_value = 1234; 

    this->invoke_to_valIn(0, test_value); 
    
    ASSERT_EVENTS_SIZE(1); 
    ASSERT_EVENTS_SR_RECEIVED_val(0, test_value); 
    ASSERT_EVENTS_SR_RECEIVED_val_SIZE(1); 

    ASSERT_TLM_SIZE(1); 
    ASSERT_TLM_currentVal(0, test_value); 
  }
} // end namespace Ref
