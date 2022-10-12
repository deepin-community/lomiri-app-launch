
########################
# lomiri-app-list
########################

add_executable(lomiri-app-list lomiri-app-list.cpp)
set_target_properties(lomiri-app-list PROPERTIES OUTPUT_NAME "lomiri-app-list")
target_link_libraries(lomiri-app-list lomiri-launcher)
install(TARGETS lomiri-app-list RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-list-pids
########################

add_executable(lomiri-app-list-pids lomiri-app-list-pids.cpp)
set_target_properties(lomiri-app-list-pids PROPERTIES OUTPUT_NAME "lomiri-app-list-pids")
target_link_libraries(lomiri-app-list-pids lomiri-launcher)
install(TARGETS lomiri-app-list-pids RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-info
########################

add_executable(lomiri-app-info lomiri-app-info.cpp)
set_target_properties(lomiri-app-info PROPERTIES OUTPUT_NAME "lomiri-app-info")
target_link_libraries(lomiri-app-info lomiri-launcher)
install(TARGETS lomiri-app-info RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

###########################
# lomiri-app-launch-appids
###########################

add_executable(lomiri-app-launch-appids lomiri-app-launch-appids.cpp)
set_target_properties(lomiri-app-launch-appids PROPERTIES OUTPUT_NAME "lomiri-app-launch-appids")
target_link_libraries(lomiri-app-launch-appids lomiri-launcher)
install(TARGETS lomiri-app-launch-appids RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-launch
########################

add_executable(lomiri-app-launch lomiri-app-launch.cpp)
set_target_properties(lomiri-app-launch PROPERTIES OUTPUT_NAME "lomiri-app-launch")
target_link_libraries(lomiri-app-launch lomiri-launcher)
install(TARGETS lomiri-app-launch RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-watch
########################

add_executable(lomiri-app-watch lomiri-app-watch.cpp)
set_target_properties(lomiri-app-watch PROPERTIES OUTPUT_NAME "lomiri-app-watch")
target_link_libraries(lomiri-app-watch lomiri-launcher)
install(TARGETS lomiri-app-watch RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-pid
########################

add_executable(lomiri-app-pid lomiri-app-pid.cpp)
set_target_properties(lomiri-app-pid PROPERTIES OUTPUT_NAME "lomiri-app-pid")
target_link_libraries(lomiri-app-pid lomiri-launcher)
install(TARGETS lomiri-app-pid RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-stop
########################

add_executable(lomiri-app-stop lomiri-app-stop.cpp)
set_target_properties(lomiri-app-stop PROPERTIES OUTPUT_NAME "lomiri-app-stop")
target_link_libraries(lomiri-app-stop lomiri-launcher)
install(TARGETS lomiri-app-stop RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-triplet
########################

add_executable(lomiri-app-triplet lomiri-app-triplet.cpp)
set_target_properties(lomiri-app-triplet PROPERTIES OUTPUT_NAME "lomiri-app-triplet")
target_link_libraries(lomiri-app-triplet lomiri-launcher)
install(TARGETS lomiri-app-triplet RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-helper-start
########################

add_executable(lomiri-helper-start lomiri-helper-start.cpp)
set_target_properties(lomiri-helper-start PROPERTIES OUTPUT_NAME "lomiri-helper-start")
target_link_libraries(lomiri-helper-start lomiri-launcher)
install(TARGETS lomiri-helper-start RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-helper-stop
########################

add_executable(lomiri-helper-stop lomiri-helper-stop.cpp)
set_target_properties(lomiri-helper-stop PROPERTIES OUTPUT_NAME "lomiri-helper-stop")
target_link_libraries(lomiri-helper-stop lomiri-launcher)
install(TARGETS lomiri-helper-stop RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-helper-list
########################

add_executable(lomiri-helper-list lomiri-helper-list.cpp)
set_target_properties(lomiri-helper-list PROPERTIES OUTPUT_NAME "lomiri-helper-list")
target_link_libraries(lomiri-helper-list lomiri-launcher)
install(TARGETS lomiri-helper-list RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# lomiri-app-usage
########################

add_executable(lomiri-app-usage lomiri-app-usage.c)
set_target_properties(lomiri-app-usage PROPERTIES OUTPUT_NAME "lomiri-app-usage")
target_link_libraries(lomiri-app-usage lomiri-launcher)
install(TARGETS lomiri-app-usage RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_BINDIR}")

########################
# Formatting
########################

add_custom_target(format-tools
	COMMAND clang-format -i -style=file
    lomiri-app-info.cpp
    lomiri-app-launch-appids.cpp
    lomiri-app-launch.cpp
    lomiri-app-list.cpp
    lomiri-app-list-pids.cpp
    lomiri-app-pid.cpp
    lomiri-app-stop.cpp
    lomiri-app-triplet.cpp
    lomiri-app-watch.cpp
    lomiri-helper-list.cpp
    lomiri-helper-start.cpp
    lomiri-helper-stop.cpp
)
