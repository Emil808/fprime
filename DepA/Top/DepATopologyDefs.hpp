#ifndef DepATopologyDefs_HPP
#define DepATopologyDefs_HPP

#include "DepA/Top/FppConstantsAc.hpp" 
#include "Svc/FramingProtocol/FprimeProtocol.hpp"
#include "Fw/Types/MallocAllocator.hpp"
#include "Svc/LinuxTimer/LinuxTimer.hpp"

namespace DepA{

    extern Svc::LinuxTimer linuxTimer; 
    
    namespace Allocation{
        extern Fw::MallocAllocator mallocator; 

    }

    struct TopologyState{
        TopologyState():
            hostName(""),
            portNumber(0)
        {

        }

        TopologyState(const char* hostName, U32 portNumber) : 
            hostName(hostName), 
            portNumber(portNumber)
        {

        }
        const char* hostName; 
        U32 portNumber; 
    };

    namespace PingEntries{
        namespace chanTlm { enum { WARN = 3, FATAL = 5 }; }
        namespace cmdDisp { enum { WARN = 3, FATAL = 5 }; }
        namespace cmdSeq { enum { WARN = 3, FATAL = 5 }; }
        namespace eventLogger { enum { WARN = 3, FATAL = 5 }; }
        namespace fileDownlink { enum { WARN = 3, FATAL = 5 }; }
        namespace fileManager { enum { WARN = 3, FATAL = 5 }; }
        namespace fileUplink { enum { WARN = 3, FATAL = 5 }; }
        namespace prmDb { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup1Comp { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup2Comp { enum { WARN = 3, FATAL = 5 }; }
        namespace rateGroup3Comp { enum { WARN = 3, FATAL = 5 }; }
    }

    
}

#endif