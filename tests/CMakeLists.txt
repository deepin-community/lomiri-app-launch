set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Google Test

find_package(GMock)

# Helper test

add_executable (helper-test helper-test.cc)
add_definitions ( -DCMAKE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )
add_definitions ( -DCMAKE_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_link_libraries (helper-test launcher-static gtest_main ${GTEST_MAIN_LIBRARIES} ${DBUSTEST_LIBRARIES})

add_test (helper-test helper-test)

# Helper test

add_executable (helper-handshake-test helper-handshake-test.cc)
target_link_libraries (helper-handshake-test launcher-static gtest_main ${GTEST_MAIN_LIBRARIES})

add_test (helper-handshake-test helper-handshake-test)

# libUAL Test

include_directories("${CMAKE_SOURCE_DIR}/liblomiri-app-launch")
add_definitions ( -DSPEW_UTILITY="${CMAKE_CURRENT_BINARY_DIR}/data-spew" )
add_definitions ( -DSESSION_TEMP_FILE="${CMAKE_CURRENT_BINARY_DIR}/libual-test-session-start-temp" )
add_definitions ( -DSOCKET_DEMANGLER="${CMAKE_BINARY_DIR}/utils/socket-demangler" )
add_definitions ( -DSOCKET_DEMANGLER_INSTALL="${pkglibexecdir}/socket-demangler" )
add_definitions ( -DSOCKET_TOOL="${CMAKE_CURRENT_BINARY_DIR}/socket-tool" )
add_definitions ( -DSNAP_BASEDIR="${CMAKE_CURRENT_SOURCE_DIR}/snap-basedir" )

add_executable (libual-test
	libual-test.cc
	mir-mock.cpp)
target_link_libraries (libual-test gtest_main ${GTEST_MAIN_LIBRARIES} ${LIBUPSTART_LIBRARIES} ${DBUSTEST_LIBRARIES} launcher-static)

add_executable (libual-cpp-test
	libual-cpp-test.cc
	mir-mock.cpp)
target_link_libraries (libual-cpp-test gtest_main ${GMOCK_LIBRARIES} ${LIBUPSTART_LIBRARIES} ${DBUSTEST_LIBRARIES} launcher-static)

add_executable (data-spew
	data-spew.c)
target_link_libraries (data-spew ${GLIB2_LIBRARIES})

add_executable (socket-tool
	socket-tool.c)

add_test (NAME libual-test COMMAND libual-test)
add_test (NAME libual-cpp-test COMMAND libual-cpp-test)

# App Store Legacy

add_executable (app-store-legacy
	app-store-legacy.cpp)
target_link_libraries (app-store-legacy ${GMOCK_LIBRARIES} ${GTEST_MAIN_LIBRARIES} launcher-static ${DBUSTEST_LIBRARIES})

add_test(NAME app-store-legacy COMMAND "${CMAKE_CURRENT_BINARY_DIR}/app-store-legacy" --gtest_list_tests "|" grep "\"^  \"" "|" xargs -n 1 printf "\"--gtest_filter=*.%s\\n\"" "|" xargs -n 1 "${CMAKE_CURRENT_BINARY_DIR}/app-store-legacy")


# Jobs Base Test

add_executable (jobs-base-test
	jobs-base-test.cpp)
target_link_libraries (jobs-base-test ${GMOCK_LIBRARIES} ${GTEST_MAIN_LIBRARIES} launcher-static ${DBUSTEST_LIBRARIES})

add_test(NAME jobs-base-test COMMAND jobs-base-test)

# Jobs Systemd Test

add_executable (jobs-systemd
	jobs-systemd.cpp)
target_link_libraries (jobs-systemd ${GMOCK_LIBRARIES} launcher-static ${DBUSTEST_LIBRARIES})

add_test(NAME jobs-systemd COMMAND jobs-systemd)

# Info Watcher ZG

add_executable (info-watcher-zg
	info-watcher-zg.cpp)
target_link_libraries (info-watcher-zg ${GMOCK_LIBRARIES} launcher-static ${DBUSTEST_LIBRARIES})

add_test(NAME info-watcher-zg COMMAND info-watcher-zg)

# Snapd Info Test

if(CURL_FOUND)
add_definitions ( -DSNAPD_TEST_SOCKET="/tmp/snapd-test-socket" )
add_executable (snapd-info-test
	snapd-info-test.cpp)
target_link_libraries (snapd-info-test gtest_main ${GTEST_MAIN_LIBRARIES} launcher-static)
add_test (NAME snapd-info-test COMMAND snapd-info-test)
endif()

# List Apps

add_executable (list-apps
	list-apps.cpp)
target_link_libraries (list-apps gtest_main ${GTEST_MAIN_LIBRARIES} ${DBUSTEST_LIBRARIES} launcher-static)
add_test (NAME list-apps COMMAND ${CMAKE_CURRENT_BINARY_DIR}/list-apps)

# Application Info Desktop

add_executable (application-info-desktop-test
  application-info-desktop.cpp
)
target_link_libraries (application-info-desktop-test gtest_main ${GMOCK_LIBRARIES} ${DBUSTEST_LIBRARIES} launcher-static)

add_test (NAME application-info-desktop-test COMMAND application-info-desktop-test)

# Application Icon Finder

add_executable (application-icon-finder-test
  # test
  application-icon-finder.cpp

  #sources
  ${CMAKE_SOURCE_DIR}/liblomiri-app-launch/application-icon-finder.cpp)
target_link_libraries (application-icon-finder-test gtest_main ${GTEST_MAIN_LIBRARIES} lomiri-launcher)

add_test (NAME application-icon-finder-test COMMAND application-icon-finder-test)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Formatted code

add_custom_target(format-tests
	COMMAND clang-format -i -style=file
	application-info-desktop.cpp
	app-store-legacy.cpp
	libual-cpp-test.cc
	libual-test.cc
	list-apps.cpp
	eventually-fixture.h
	info-watcher-zg.cpp
	jobs-base-test.cpp
	jobs-systemd.cpp
	libertine-service.h
	registry-mock.h
	snapd-info-test.cpp
	snapd-mock.h
	spew-master.h
	systemd-mock.h
	zg-test.cc
	zg-mock.h
)
