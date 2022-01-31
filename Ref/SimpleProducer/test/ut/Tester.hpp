// ======================================================================
// \title  SimpleProducer/test/ut/Tester.hpp
// \author emil
// \brief  hpp file for SimpleProducer test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Ref/SimpleProducer/SimpleProducer.hpp"

namespace Ref {

  class Tester :
    public SimpleProducerGTestBase
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

      void testInvokeSET_NUM(); 
    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_valOut
      //!
      void from_valOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 val1 
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

      //! Invoke Simple Producer's SET_NUM Command
      //!
      void testSET_NUM(F32 input);

      void invokeSET_NUM(F32 input, U32 cmdSeq); 

      void verifySET_NUM(F32 input, U32 cmdSeq, U32 index, U32 expected_size);

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      SimpleProducer component;

  };

} // end namespace Ref

#endif
