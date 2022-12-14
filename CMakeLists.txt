project(lomiri-app-launch C CXX)
cmake_minimum_required(VERSION 2.8.9)

##########################
# Version Info
##########################

set(API_VERSION 0)
set(ABI_VERSION 0)

##########################
# Options
##########################

option (enable_tests "Build tests" ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

set(PACKAGE ${CMAKE_PROJECT_NAME})

##########################
# Dependent Packages
##########################

find_package(PkgConfig REQUIRED)
find_package(GObjectIntrospection REQUIRED)
include(GNUInstallDirs)
include(CheckIncludeFile)
include(CheckFunctionExists)
include(UseGlibGeneration)
include(UseGdbusCodegen)
include(UseConstantBuilder)
include(UseLttngGenTp)

# Workaround for libexecdir on debian
if (EXISTS "/etc/debian_version") 
  set(CMAKE_INSTALL_LIBEXECDIR ${CMAKE_INSTALL_LIBDIR})
  set(CMAKE_INSTALL_FULL_LIBEXECDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}")
endif()

set(pkglibexecdir "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
set(CMAKE_INSTALL_PKGLIBEXECDIR "${CMAKE_INSTALL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
set(CMAKE_INSTALL_FULL_PKGLIBEXECDIR "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${CMAKE_PROJECT_NAME}")
set(CMAKE_INSTALL_FULL_PKGDATADIR "${CMAKE_INSTALL_FULL_DATADIR}/${CMAKE_PROJECT_NAME}")

execute_process(COMMAND dpkg-architecture -qDEB_BUILD_MULTIARCH
	OUTPUT_VARIABLE LOMIRI_APP_LAUNCH_ARCH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(lomiri_app_launch_arch "${LOMIRI_APP_LAUNCH_ARCH}")

add_compile_options(
# Because we believe in quality
	-Wall -Werror
	-g
	-pthread
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

enable_testing()

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})

pkg_check_modules(GOBJECT2 REQUIRED gobject-2.0)
include_directories(${GOBJECT2_INCLUDE_DIRS})

pkg_check_modules(GIO2 REQUIRED gio-2.0 gio-unix-2.0)
include_directories(${GIO2_INCLUDE_DIRS})

pkg_check_modules(JSONGLIB REQUIRED json-glib-1.0>=1.1.2)
include_directories(${JSONGLIB_INCLUDE_DIRS})

pkg_check_modules(ZEITGEIST REQUIRED zeitgeist-2.0)
include_directories(${ZEITGEIST_INCLUDE_DIRS})

pkg_check_modules(DBUS REQUIRED dbus-1)
include_directories(${DBUS_INCLUDE_DIRS})

pkg_check_modules(DBUSTEST REQUIRED dbustest-1>=14.04.0)
include_directories(${DBUSTEST_INCLUDE_DIRS})

pkg_check_modules(LTTNG REQUIRED lttng-ust)
include_directories(${LTTNG_INCLUDE_DIRS})

pkg_check_modules(MIR REQUIRED mirclient)
include_directories(${MIR_INCLUDE_DIRS})

pkg_check_modules(LIBERTINE libertine)
include_directories(${LIBERTINE_INCLUDE_DIRS})
if(${LIBERTINE_FOUND})
  add_definitions(-DHAVE_LIBERTINE=1)
endif(${LIBERTINE_FOUND})

pkg_check_modules(CURL libcurl>=7.47)
include_directories(${CURL_INCLUDE_DIRS})

pkg_check_modules(LOMIRI_API REQUIRED liblomiri-api>=0.1.0)
include_directories(${LOMIRI_API_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

add_subdirectory(liblomiri-app-launch)
add_subdirectory(tools)
add_subdirectory(lomiri-app-test)
add_subdirectory(utils)

# testing & coverage
if (${enable_tests})
  enable_testing ()
  add_subdirectory(tests)
endif ()

find_package(CoverageReport)
set(filter-list)
list(APPEND filter-list "/usr/include")
list(APPEND filter-list "${CMAKE_SOURCE_DIR}/tests/*")
if (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  list(APPEND filter-list "${CMAKE_BINARY_DIR}/*")
endif()
ENABLE_COVERAGE_REPORT(
  TARGETS
	lomiri-launcher
	launcher-static
  TESTS
	app-store-legacy
	application-icon-finder-test
	application-info-desktop-test
	helper-test
	helper-handshake-test
	info-watcher-zg
	jobs-base-test
	jobs-systemd
	libual-test
	libual-cpp-test
	list-apps
	snapd-info-test
  FILTER
  	${filter-list}
)
