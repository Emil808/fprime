set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/svGPS.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/svGPS.cpp"

)

register_fprime_module()

set(UT_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/svGPS.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/svGPS.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/svGPSTester.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/svGPSTesterMain.cpp"
)
register_fprime_ut()