// ======================================================================
// \title  svGPS.hpp
// \author emil
// \brief  hpp file for svGPS component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef svGPS_HPP
#define svGPS_HPP

#include "DepA2/svGPS/svGPSComponentAc.hpp"

namespace DepA2 {

  class svGPS :
    public svGPSComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object svGPS
      //!
      svGPS(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object svGPS
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object svGPS
      //!
      ~svGPS();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for Run
      //!
      void Run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );


    };

} // end namespace DepA2

#endif
