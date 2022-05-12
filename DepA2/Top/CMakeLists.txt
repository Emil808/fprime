####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
####

set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/instances.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/topology.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/DepA2TopologyDefs.cpp"
)
set(MOD_DEPS
  Fw/Logger
  Svc/LinuxTime
  # Communication Implementations
  Drv/Udp
  Drv/TcpClient
  Drv/TcpServer
  Drv/MultiTcpServer
  Svc/BufferManager
)

register_fprime_module()