// ======================================================================
// \title  svGPS/test/ut/Tester.hpp
// \author emil
// \brief  hpp file for svGPS test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef svGPSTESTER_HPP
#define svGPSTESTER_HPP

#include "GTestBase.hpp"
#include "DepA2/svGPS/svGPS.hpp"

namespace DepA2 {

  class Tester :
    public svGPSGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:

      //! Construct object Tester
      //!
      Tester();

      //! Destroy object Tester
      //!
      ~Tester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! To do
      //!
      void toDo();

      void runSingle(void); 

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_dataOut
      //!
      void from_dataOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 gpsTimeWeeks, 
          U32 gpsTimeSeconds, 
          F32 gpsPosX, 
          F32 gpsPoxY, 
          F32 gpsPosZ 
      );

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      svGPS component;

  };

} // end namespace DepA2

#endif
