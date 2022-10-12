
include_directories("${CMAKE_SOURCE_DIR}/liblomiri-app-launch")
add_executable(lomiri-app-test lomiri-app-test.cpp)
set_target_properties(lomiri-app-test PROPERTIES OUTPUT_NAME "lomiri-app-test")
target_link_libraries(lomiri-app-test lomiri-launcher ${MIR_LIBRARIES})
install(TARGETS lomiri-app-test RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}/app-test")

