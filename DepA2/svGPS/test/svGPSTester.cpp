// ======================================================================
// \title  svGPS.hpp
// \author emil
// \brief  cpp file for svGPS test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "svGPSTester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace DepA2 {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
#if FW_OBJECT_NAMES == 1
      svGPSGTestBase("Tester", MAX_HISTORY_SIZE),
      component("svGPS")
#else
      svGPSGTestBase(MAX_HISTORY_SIZE),
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

  void Tester::runSingle(void){

    this->invoke_to_Run(0,0); 
    ASSERT_TLM_SIZE(1); 

  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_dataOut_handler(
        const NATIVE_INT_TYPE portNum,
        U32 gpsTimeWeeks,
        U32 gpsTimeSeconds,
        F32 gpsPosX,
        F32 gpsPoxY,
        F32 gpsPosZ
    )
  {
    this->pushFromPortEntry_dataOut(gpsTimeWeeks, gpsTimeSeconds, gpsPosX, gpsPoxY, gpsPosZ);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // Run
    this->connect_to_Run(
        0,
        this->component.get_Run_InputPort(0)
    );

    // dataOut
    this->component.set_dataOut_OutputPort(
        0,
        this->get_from_dataOut(0)
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

} // end namespace DepA2
