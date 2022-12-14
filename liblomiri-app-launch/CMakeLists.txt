include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


##########################
# Library
##########################

add_lttng_gen_tp(NAME lomiri-app-launch-trace)

EXECUTE_PROCESS(COMMAND gcc -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if ("${GCC_VERSION}" VERSION_LESS "5.0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=pedantic")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wpedantic")
add_definitions ( -DOOM_HELPER="${pkglibexecdir}/oom-adjust-setuid-helper" )
add_definitions ( -DLIBERTINE_LAUNCH="${CMAKE_INSTALL_FULL_BINDIR}/libertine-launch" )
add_definitions ( -DG_LOG_DOMAIN="lomiri-app-launch" )
add_definitions ( -DLOMIRI_APP_LAUNCH_ARCH="${LOMIRI_APP_LAUNCH_ARCH}" )
add_definitions ( -DHELPER_EXEC_TOOL_DIR="${pkglibexecdir}" )
add_definitions ( -DHELPER_HELPER_TOOL="${pkglibexecdir}/systemd-helper-helper" )

set(LAUNCHER_HEADERS
lomiri-app-launch.h
)

set(LAUNCHER_CPP_HEADERS
appid.h
application.h
helper.h
oom.h
registry.h
type-tagger.h
)

set(LAUNCHER_CPP_SOURCES
application.cpp
app-store-base.h
app-store-base.cpp
app-store-legacy.h
app-store-legacy.cpp
app-store-libertine.h
app-store-libertine.cpp
app-store-snap.h
app-store-snap.cpp
helper.cpp
helper-impl.h
registry.cpp
registry-impl.h
registry-impl.cpp
application-impl-base.h
application-impl-base.cpp
application-impl-legacy.h
application-impl-legacy.cpp
application-impl-libertine.h
application-impl-libertine.cpp
application-impl-snap.h
application-impl-snap.cpp
application-info-desktop.h
application-info-desktop.cpp
application-icon-finder.h
application-icon-finder.cpp
info-watcher.h
info-watcher.cpp
info-watcher-zg.h
info-watcher-zg.cpp
glib-thread.h
glib-thread.cpp
jobs-base.h
jobs-base.cpp
jobs-systemd.h
jobs-systemd.cpp
jobs-posix.h
jobs-posix.cpp
signal-unsubscriber.h
snapd-info.h
snapd-info.cpp
string-util.h
)

set(LAUNCHER_SOURCES
lomiri-app-launch.cpp
second-exec-core.c
lomiri-app-launch-trace.c
utils.c
utils-shared.c
)

add_custom_target(format
	COMMAND clang-format -i -style=file ${LAUNCHER_CPP_HEADERS} ${LAUNCHER_CPP_SOURCES}
)

set(LAUNCHER_GEN_SOURCES
)

add_gdbus_codegen_with_namespace(LAUNCHER_GEN_SOURCES proxy-socket-demangler com.lomiri.LomiriAppLaunch. proxy ${CMAKE_SOURCE_DIR}/data/com.lomiri.LomiriAppLaunch.SocketDemangler.xml)

add_library(launcher-static ${LAUNCHER_SOURCES} ${LAUNCHER_CPP_SOURCES} ${LAUNCHER_GEN_SOURCES})

target_link_libraries(launcher-static
	${GLIB2_LIBARIES}
	${GOBJECT2_LIBRARIES}
	${LIBUPSTART_LIBRARIES}
	${GIO2_LIBRARIES}
	${LTTNG_LIBRARIES}
	${JSONGLIB_LIBRARIES}
	${CLICK_LIBRARIES}
	${ZEITGEIST_LIBRARIES}
	${MIR_LIBRARIES}
	${LIBERTINE_LIBRARIES}
	${CURL_LIBRARIES}
	-lpthread
	-Wl,--no-undefined
)

add_library(lomiri-launcher SHARED ${LAUNCHER_SOURCES} ${LAUNCHER_CPP_SOURCES} ${LAUNCHER_GEN_SOURCES})

set_target_properties(lomiri-launcher PROPERTIES
	VERSION ${ABI_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
	OUTPUT_NAME "lomiri-app-launch"
	LINK_FLAGS "${ldflags} -Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/liblomiri-app-launch.map"
	LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/liblomiri-app-launch.map"
)

target_link_libraries(lomiri-launcher
	${GLIB2_LIBARIES}
	${GOBJECT2_LIBRARIES}
	${LIBUPSTART_LIBRARIES}
	${GIO2_LIBRARIES}
	${LTTNG_LIBRARIES}
	${JSONGLIB_LIBRARIES}
	${CLICK_LIBRARIES}
	${ZEITGEIST_LIBRARIES}
	${MIR_LIBRARIES}
	${LIBERTINE_LIBRARIES}
	${CURL_LIBRARIES}
	-lpthread
	-Wl,--no-undefined
)

install(
	FILES ${LAUNCHER_HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/liblomiri-app-launch-${API_VERSION}"
)

install(
	FILES ${LAUNCHER_CPP_HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/liblomiri-app-launch-${API_VERSION}/lomiri-app-launch"
)

install(
	TARGETS lomiri-launcher
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

##########################
# Pkg Config
##########################

set(apiversion "${API_VERSION}")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
set(VERSION "${API_VERSION}")

configure_file("lomiri-app-launch.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/lomiri-app-launch-${API_VERSION}.pc" @ONLY)
install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/lomiri-app-launch-${API_VERSION}.pc"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

##########################
# Introspection
##########################

include(UseGObjectIntrospection)

set(INTROSPECTION_GIRS)
set(_introspection_files ${LAUNCHER_HEADERS})
set(LomiriAppLaunch_3_gir "lomiri-app-launch")
set(LomiriAppLaunch_3_gir_INCLUDES GObject-2.0)

gir_get_cflags(_cflags)
list_prefix(MIR_C_INCLUDES MIR_INCLUDE_DIRS "-I")
set(LomiriAppLaunch_3_gir_CFLAGS ${c_flags} ${MIR_C_INCLUDES} "-L/usr/local/lib")
set(LomiriAppLaunch_3_gir_LIBS lomiri-app-launch)

list_make_absolute(_abs_introspection_files _introspection_files "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LomiriAppLaunch_3_gir_FILES ${_abs_introspection_files})
set(LomiriAppLaunch_3_gir_SCANNERFLAGS --c-include "lomiri-app-launch.h")
set(LomiriAppLaunch_3_gir_EXPORT_PACKAGES "lomiri-app-launch-${API_VERSION}")

list(APPEND INTROSPECTION_GIRS LomiriAppLaunch-3.gir)
gir_add_introspections(INTROSPECTION_GIRS)

add_subdirectory(abi-check)
