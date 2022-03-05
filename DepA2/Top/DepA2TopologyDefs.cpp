#include "DepA2/Top/DepA2TopologyDefs.hpp"

namespace DepA2 {

    namespace Allocation {
        Fw::MallocAllocator mallocator; 
    }

    Svc::LinuxTimer linuxTimer(FW_OPTIONAL_NAME("linuxTimer")); 
}