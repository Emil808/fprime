// ======================================================================
// \title  DeviceIDComponentImpl.hpp
// \author emil
// \brief  hpp file for DeviceID component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef DeviceID_HPP
#define DeviceID_HPP

#include "RPI/DeviceID/DeviceIDComponentAc.hpp"

namespace Rpi {

  class DeviceIDComponentImpl :
    public DeviceIDComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object DeviceID
      //!
      DeviceIDComponentImpl(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object DeviceID
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object DeviceID
      //!
      ~DeviceIDComponentImpl(void);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for RD_GET_ID command handler
      //! Reply with unique device ID
      void RD_GET_ID_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );


    };

} // end namespace Rpi

#endif
