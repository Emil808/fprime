module DepA{

    enum Ports_RateGroups{
        rateGroup1
        rateGroup2
        rateGroup3
    }

    enum Ports_StaticMemory{
        downlink 
        uplink 
        hub
        downlinkP
        commP
    }

    topology DepA{

        # ------------------------------
        # Instances used in the topology
        # ------------------------------

        instance $health
        instance cmdDisp
        instance cmdSeq
        instance eventLogger
        instance chanTlm
        instance prmDb
        instance rateGroup1Comp
        instance rateGroup2Comp
        instance rateGroup3Comp
        instance rateGroupDriverComp
        instance comm
        instance downlink
        instance fatalAdapter
        instance fatalHandler
        instance fileDownlink
        instance fileManager
        instance fileUplink
        instance fileUplinkBufferManager
        instance linuxTime
        instance linuxTimer
        instance staticMemory
        instance textLogger
        instance uplink
        instance systemResources

        instance hub
        instance downlinkP
        instance commP
        instance SimpleProducer
        instance fileDownlinkP

        # ------------------------------
        # pattern graph specifiers
        # ------------------------------

        command connections instance cmdDisp
        
        event connections instance eventLogger
        param connections instance prmDb
        telemetry connections instance chanTlm
        text event connections instance textLogger
        time connections instance linuxTime
        health connections instance $health

        # ----------------------------------------------------------------------
        # Direct graph specifiers
        # ----------------------------------------------------------------------

        connections Downlink {

            chanTlm.PktSend -> downlink.comIn
            eventLogger.PktSend -> downlink.comIn
            fileDownlink.bufferSendOut -> downlink.bufferIn

            downlink.framedAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.downlink]
            downlink.framedOut -> comm.send
            downlink.bufferDeallocate -> fileDownlink.bufferReturn

            comm.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.downlink]

        }

        connections FaultProtection {
            eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
        }
        
        connections Sequencer {
        cmdSeq.comCmdOut -> cmdDisp.seqCmdBuff
        cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
        }

        connections Uplink {

        comm.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.uplink]
        comm.$recv -> uplink.framedIn
        uplink.framedDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.uplink]

        uplink.comOut -> cmdDisp.seqCmdBuff
        cmdDisp.seqCmdStatus -> uplink.cmdResponseIn

        uplink.bufferAllocate -> fileUplinkBufferManager.bufferGetCallee
        uplink.bufferOut -> fileUplink.bufferSendIn
        uplink.bufferDeallocate -> fileUplinkBufferManager.bufferSendIn
        fileUplink.bufferSendOut -> fileUplinkBufferManager.bufferSendIn

        }

        connections RateGroups {

            linuxTimer.CycleOut -> rateGroupDriverComp.CycleIn
            # Rate group 1
            rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1Comp.CycleIn
            rateGroup1Comp.RateGroupMemberOut[1] -> fileDownlinkP.Run
            rateGroup1Comp.RateGroupMemberOut[2] -> chanTlm.Run
            rateGroup1Comp.RateGroupMemberOut[3] -> fileDownlink.Run
            rateGroup1Comp.RateGroupMemberOut[4] -> systemResources.run
            

            # Rate group 2
            rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2Comp.CycleIn
            rateGroup2Comp.RateGroupMemberOut[0] -> cmdSeq.schedIn

            # Rate group 3
            rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3Comp.CycleIn
            rateGroup3Comp.RateGroupMemberOut[0] -> $health.Run
            rateGroup3Comp.RateGroupMemberOut[3] -> fileUplinkBufferManager.schedIn

        }

        connections hub{
            SimpleProducer.valOut -> hub.portIn[0]
            
            hub.bufferDeallocate -> fileDownlinkP.bufferReturn
            fileDownlinkP.bufferSendOut -> hub.buffersIn 

            hub.dataOutAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.hub]
            hub.dataOut -> downlinkP.bufferIn
            downlinkP.bufferDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.hub]

            downlinkP.framedAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.downlinkP]
            downlinkP.framedOut -> commP.send
            commP.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.downlinkP]
            

        }
    }
}