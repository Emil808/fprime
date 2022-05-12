module DepA {

  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {

    constant queueSize = 10

    constant stackSize = 16 * 1024

  }

    # ----------------------------------------------------------------------
    # Active component instances
    # ----------------------------------------------------------------------

    instance rateGroup1Comp: Svc.ActiveRateGroup base id 0x0200 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 120 \
    {

    phase Fpp.ToCpp.Phases.configObjects """
    NATIVE_UINT_TYPE context[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    """

    phase Fpp.ToCpp.Phases.instances """
    Svc::ActiveRateGroup rateGroup1Comp(
        FW_OPTIONAL_NAME("rateGroup1Comp"),
        ConfigObjects::rateGroup1Comp::context,
        FW_NUM_ARRAY_ELEMENTS(ConfigObjects::rateGroup1Comp::context)
    );
    """

    }

    instance rateGroup2Comp: Svc.ActiveRateGroup base id 0x0300 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 119 \
    {

    phase Fpp.ToCpp.Phases.configObjects """
    NATIVE_UINT_TYPE context[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    """

    phase Fpp.ToCpp.Phases.instances """
    Svc::ActiveRateGroup rateGroup2Comp(
        FW_OPTIONAL_NAME("rateGroup2Comp"),
        ConfigObjects::rateGroup2Comp::context,
        FW_NUM_ARRAY_ELEMENTS(ConfigObjects::rateGroup2Comp::context)
    );
    """

    }

    instance rateGroup3Comp: Svc.ActiveRateGroup base id 0x0400 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 118 \
    {

    phase Fpp.ToCpp.Phases.configObjects """
    NATIVE_UINT_TYPE context[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    """

    phase Fpp.ToCpp.Phases.instances """
    Svc::ActiveRateGroup rateGroup3Comp(
        FW_OPTIONAL_NAME("rateGroup3Comp"),
        ConfigObjects::rateGroup3Comp::context,
        FW_NUM_ARRAY_ELEMENTS(ConfigObjects::rateGroup3Comp::context)
    );
    """

    }

    instance cmdDisp: Svc.CommandDispatcher base id 0x0500 \
    queue size 20 \
    stack size Default.stackSize \
    priority 101

    instance cmdSeq: Svc.CmdSequencer base id 0x0600 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 100 \
    {

        phase Fpp.ToCpp.Phases.configConstants """
        enum {
            BUFFER_SIZE = 5*1024
    };
    """

    phase Fpp.ToCpp.Phases.configComponents """
        cmdSeq.allocateBuffer(
        0,
        Allocation::mallocator,
        ConfigConstants::cmdSeq::BUFFER_SIZE
    );
    """

    phase Fpp.ToCpp.Phases.tearDownComponents """
        cmdSeq.deallocateBuffer(Allocation::mallocator);
    """

    }

    instance fileDownlink: Svc.FileDownlink base id 0x0700 \
    queue size 30 \
    stack size Default.stackSize \
    priority 100 \
    {

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
        TIMEOUT = 1000,
        COOLDOWN = 1000,
        CYCLE_TIME = 1000,
        FILE_QUEUE_DEPTH = 10
    };
    """

    phase Fpp.ToCpp.Phases.configComponents """
    fileDownlink.configure(
        ConfigConstants::fileDownlink::TIMEOUT,
        ConfigConstants::fileDownlink::COOLDOWN,
        ConfigConstants::fileDownlink::CYCLE_TIME,
        ConfigConstants::fileDownlink::FILE_QUEUE_DEPTH
    );
    """

    }

    instance fileManager: Svc.FileManager base id 0x0800 \
    queue size 30 \
    stack size Default.stackSize \
    priority 100

    instance fileUplink: Svc.FileUplink base id 0x0900 \
    queue size 30 \
    stack size Default.stackSize \
    priority 100

    instance eventLogger: Svc.ActiveLogger base id 0x0B00 \
        queue size Default.queueSize \
        stack size Default.stackSize \
        priority 98

    instance chanTlm: Svc.TlmChan base id 0x0C00 \
        queue size Default.queueSize \
        stack size Default.stackSize \
        priority 97

    instance prmDb: Svc.PrmDb base id 0x0D00 \
        queue size Default.queueSize \
        stack size Default.stackSize \
        priority 96 \
    {

        phase Fpp.ToCpp.Phases.instances """
        Svc::PrmDb prmDb(FW_OPTIONAL_NAME("prmDb"), "PrmDb.dat");
        """

        phase Fpp.ToCpp.Phases.readParameters """
        prmDb.readParamFile();
        """

    }

    instance SimpleProducer: DepA.SimpleProducer base id 0x0E00 \
        queue size Default.queueSize \
        stack size Default.stackSize \ 
        priority 100


    # ----------------------------------------------------------------------
    # Queued component instances
    # ----------------------------------------------------------------------

      instance $health: Svc.Health base id 0x2000 \
    queue size 25 \
  {

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
      WATCHDOG_CODE = 0x123
    };
    """

    phase Fpp.ToCpp.Phases.configComponents """
    health.setPingEntries(
        ConfigObjects::health::pingEntries,
        FW_NUM_ARRAY_ELEMENTS(ConfigObjects::health::pingEntries),
        ConfigConstants::health::WATCHDOG_CODE
    );
    """

  }

    # ----------------------------------------------------------------------
    # Passive component instances
    # ----------------------------------------------------------------------

    @ Communications driver. May be swapped with other comm drivers like UART
    @ Note: Here we have TCP reliable uplink and UDP (low latency) downlink
    instance comm: Drv.ByteStreamDriverModel base id 0x4000 \
    at "../../Drv/TcpClient/TcpClient.hpp" \
    {

    phase Fpp.ToCpp.Phases.instances """
    Drv::TcpClient comm(FW_OPTIONAL_NAME("comm"));
    """

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
        PRIORITY = 100,
        STACK_SIZE = Default::stackSize
    };
    """

    phase Fpp.ToCpp.Phases.startTasks """
    // Initialize socket server if and only if there is a valid specification
    if (state.hostName != nullptr && state.portNumber != 0) {
        Os::TaskString name("ReceiveTask");
        // Uplink is configured for receive so a socket task is started
        comm.configure(state.hostName, state.portNumber);
        (void) printf("FHELLO\n");
        comm.startSocketTask(
            name,
            true,
            ConfigConstants::comm::PRIORITY,
            ConfigConstants::comm::STACK_SIZE
        );
    }
    """

    phase Fpp.ToCpp.Phases.freeThreads """
    comm.stopSocketTask();
    (void) comm.joinSocketTask(nullptr);
    """

    }

    instance downlink: Svc.Framer base id 0x4100 {

    phase Fpp.ToCpp.Phases.configObjects """
    Svc::FprimeFraming framing;
    """

    phase Fpp.ToCpp.Phases.configComponents """
    downlink.setup(ConfigObjects::downlink::framing);
    """

    }

    instance fatalAdapter: Svc.AssertFatalAdapter base id 0x4200

    instance fatalHandler: Svc.FatalHandler base id 0x4300

    instance fileUplinkBufferManager: Svc.BufferManager base id 0x4400 {

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
      STORE_SIZE = 3000,
      QUEUE_SIZE = 30,
      MGR_ID = 200
    };
    """

    phase Fpp.ToCpp.Phases.configComponents """
    Svc::BufferManager::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins, 0, sizeof(upBuffMgrBins));
    {
      using namespace ConfigConstants::fileUplinkBufferManager;
      upBuffMgrBins.bins[0].bufferSize = STORE_SIZE;
      upBuffMgrBins.bins[0].numBuffers = QUEUE_SIZE;
      fileUplinkBufferManager.setup(
          MGR_ID,
          0,
          Allocation::mallocator,
          upBuffMgrBins
      );
    }
    """

    phase Fpp.ToCpp.Phases.tearDownComponents """
    fileUplinkBufferManager.cleanup();
    """

    }


    instance linuxTime: Svc.Time base id 0x4500 \
    at "../../Svc/LinuxTime/LinuxTime.hpp" \
    {

    phase Fpp.ToCpp.Phases.instances """
    Svc::LinuxTime linuxTime(FW_OPTIONAL_NAME("linuxTime"));
    """

    }

    instance linuxTimer: Svc.LinuxTimer base id 1600 \
  {

    phase Fpp.ToCpp.Phases.instances """
    // Declared in DepATopologyDefs.cpp
    """

    phase Fpp.ToCpp.Phases.stopTasks """
    linuxTimer.quit();
    """

  }

    instance rateGroupDriverComp: Svc.RateGroupDriver base id 0x4600 {

    phase Fpp.ToCpp.Phases.configObjects """
    NATIVE_INT_TYPE rgDivs[Svc::RateGroupDriver::DIVIDER_SIZE] = { 1, 2, 4 };
    """

    phase Fpp.ToCpp.Phases.instances """
    Svc::RateGroupDriver rateGroupDriverComp(
        FW_OPTIONAL_NAME("rateGroupDriverComp"),
        ConfigObjects::rateGroupDriverComp::rgDivs,
        FW_NUM_ARRAY_ELEMENTS(ConfigObjects::rateGroupDriverComp::rgDivs)
    );
    """

    }


    instance staticMemory: Svc.StaticMemory base id 0x4800

    instance textLogger: Svc.PassiveTextLogger base id 0x4900

    instance uplink: Svc.Deframer base id 0x4A00 {

        phase Fpp.ToCpp.Phases.configObjects """
        Svc::FprimeDeframing deframing;
        """

        phase Fpp.ToCpp.Phases.configComponents """
        uplink.setup(ConfigObjects::uplink::deframing);
        """

    }

    instance systemResources: Svc.SystemResources base id 0x4B00

    instance hub: Svc.GenericHub base id 0x4C00

    instance commP: Drv.ByteStreamDriverModel base id 0x5000 \
    at "../../Drv/MultiTcpServer/MultiTcpServer.hpp" \
    {

    phase Fpp.ToCpp.Phases.instances """
    Drv::MultiTcpServer commP(FW_OPTIONAL_NAME("commP"));
    """

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
        PRIORITY = 100,
        STACK_SIZE = Default::stackSize
    };
    """

    phase Fpp.ToCpp.Phases.startTasks """
    // Initialize socket server if and only if there is a valid specification
    const char * hostNameP = "127.0.1.1"; 
    U32 portNumberP = 50100; 
    char hostfile_name[] = "node_hostfile_0.txt"; 
    if (hostNameP != nullptr && portNumberP != 0) {
        Os::TaskString name("commP.Accept");
        commP.configure(hostNameP, portNumberP); 
        commP.setDeviceID(0x20202020); 
        commP.setHostFile(hostfile_name, sizeof(hostfile_name));
        commP.startup(); //todo check that startup starts correctly? 

        commP.startAcceptTask(name, true, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT); 
        
        name = "commP.Open"; 
        commP.startSocketOpenTask(name, true, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT, Os::Task::TASK_DEFAULT);
        
    }
    """

    phase Fpp.ToCpp.Phases.freeThreads """
    commP.shutdown(); 
    commP.stopAcceptTask();
    commP.stopSocketOpenTask(); 
    (void) commP.joinAcceptTask(nullptr);
    (void) commP.joinSocketOpenTask(nullptr); 
    """

    }

    instance downlinkP: Svc.Framer base id 0x5100 {

        phase Fpp.ToCpp.Phases.configObjects """
        Svc::FprimeFraming framing;
        """

        phase Fpp.ToCpp.Phases.configComponents """
        downlinkP.setup(ConfigObjects::downlinkP::framing);
        """

    }

    instance fileDownlinkP: Svc.FileDownlink base id 0x5200 \
    queue size 30 \
    stack size Default.stackSize \
    priority 100 \
    {

    phase Fpp.ToCpp.Phases.configConstants """
    enum {
        TIMEOUT = 1000,
        COOLDOWN = 1000,
        CYCLE_TIME = 1000,
        FILE_QUEUE_DEPTH = 10
    };
    """

    phase Fpp.ToCpp.Phases.configComponents """
    fileDownlinkP.configure(
        ConfigConstants::fileDownlinkP::TIMEOUT,
        ConfigConstants::fileDownlinkP::COOLDOWN,
        ConfigConstants::fileDownlinkP::CYCLE_TIME,
        ConfigConstants::fileDownlinkP::FILE_QUEUE_DEPTH
    );
    """

    }

    instance dynamicMemory: Svc.BufferManager base id 0x5300 \
    {

    phase Fpp.ToCpp.Phases.instances"""
        Svc::BufferManager dynamicMemory(FW_OPTIONAL_NAME("dynamicMemory"));
    """
    phase Fpp.ToCpp.Phases.configComponents"""
       
        Svc::BufferManagerComponentImpl::BufferBins bins;
        memset(&bins,0,sizeof(bins));
        bins.bins[0].bufferSize = 1024; 
        bins.bins[0].numBuffers = 5; 
        
        static Fw::MallocAllocator allocator; 
        dynamicMemory.setup(0x5300, 0x50, allocator ,bins);
    """

    phase Fpp.ToCpp.Phases.tearDownComponents """
        dynamicMemory.cleanup(); 
    """
    }

    instance swarmFramer: DepA.SwarmFramer base id 0x5400 \
    {
        phase Fpp.ToCpp.Phases.configComponents """
        swarmFramer.setSourceId(0x20202020); 

        """
    }

}