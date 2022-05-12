// ======================================================================
// \title  svGPS.cpp
// \author emil
// \brief  cpp file for svGPS component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DepA2/svGPS/svGPS.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <string> 

namespace DepA2 {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  svGPS ::
    svGPS(
        const char *const compName
    ) : svGPSComponentBase(compName)
  {

  }

  void svGPS ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    svGPSComponentBase::init(queueDepth, instance);
  }

  svGPS ::
    ~svGPS()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void svGPS ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO


    //Simulate getting GPS data from the gps device connected on the serial bus
    //From I2C or SPI driver

    //yay, we have data
    U32 gpsTimeWeeks = 2202;
    U32 gpsTimeSeconds = 179118; 

    F32 gpsPosX = 144 + rand() % 100; 
    F32 gpsPosY = 124 + rand() % 100; 
    F32 gpsPosZ = 903 + rand() % 100; 

    this->tlmWrite_gpsTimeWeek(gpsTimeWeeks);
    this->tlmWrite_gpsTimeSecond(gpsTimeSeconds);
    this->tlmWrite_gpsPositionX(gpsPosX); 
    this->tlmWrite_gpsPositionY(gpsPosY); 
    this->tlmWrite_gpsPositionZ(gpsPosZ); 

    //this->dataOut_out(0, gpsTimeWeeks, gpsTimeSeconds, gpsPosX, gpsPosY, gpsPosZ); 
  }

} // end namespace DepA2
