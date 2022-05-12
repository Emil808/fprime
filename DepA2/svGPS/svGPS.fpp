module DepA2{

    port gpsDataPort(

        gpsTimeWeeks: U32
        gpsTimeSeconds: U32

        gpsPosX: F32
        gpsPoxY: F32
        gpsPosZ: F32
    )
    active component svGPS{

        # ----------------
        # General Ports
        # ----------------
        output port dataOut: gpsDataPort
        
        # ----------------
        # Special Ports 
        # ----------------
        telemetry port tlmOut
        time get port timeGetOut

        async input port Run: Svc.Sched

        # todo:
        # ports to handle serial communications with the physical gps device
        # connected by on the serial bus
        # buffer to allocate the message buffer

        # input port serialRecv
        # output port serialBuffer
        
        # ----------------
        # Telemetry 
        # ----------------
        
        telemetry gpsTimeWeek: U32  
        telemetry gpsTimeSecond: U32
        telemetry gpsPositionX: F32  
        telemetry gpsPositionY: F32 
        telemetry gpsPositionZ: F32 
        
    }



}