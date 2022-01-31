set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/SimpleProducer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/SimpleProducer.fpp"
        
)
register_fprime_module()

set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/SimpleProducer.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/test/ut/Testmain.cpp"
)
register_fprime_ut()