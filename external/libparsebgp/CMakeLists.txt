cmake_minimum_required(VERSION 3.13)

set(LIBPARSEBGP_VERSION   "1.0.0")
set(LIBPARSEBGP_SOVERSION "2.0.0")

project(libparsebgp VERSION ${LIBPARSEBGP_VERSION})

message(STATUS "Project '${PROJECT_NAME}' version: '${PROJECT_VERSION}'")

include(GNUInstallDirs)

set(LIBPARSEBGP_ROOT_DIR  "upstream")
set(LIBPARSEBGP_LIB_DIR   "${LIBPARSEBGP_ROOT_DIR}/lib")
set(LIBPARSEBGP_TOOLS_DIR "${LIBPARSEBGP_ROOT_DIR}/lib")

set(LIBPARSEBGP_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set(LIBPARSEBGP_MID_VERSION   ${PROJECT_VERSION_MINOR})
set(LIBPARSEBGP_MINOR_VERSION ${PROJECT_VERSION_PATCH})

option(LIBPARSEBGP_PARSER_DEBUG "Enable parser debug info" OFF)

add_library(parsebgp)

if(LIBPARSEBGP_PARSER_DEBUG)
    target_compile_definitions(parsebgp PRIVATE PARSER_DEBUG)
endif()

configure_file(
    "${LIBPARSEBGP_LIB_DIR}/parsebgp.h.in"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp.h"
    @ONLY
)

set_target_properties(parsebgp PROPERTIES
    C_STANDARD 99
    SOVERSION ${LIBPARSEBGP_SOVERSION}
    PUBLIC_HEADER
        "${LIBPARSEBGP_LIB_DIR}/parsebgp.h"
        "${LIBPARSEBGP_LIB_DIR}/parsebgp_error.h"
        "${LIBPARSEBGP_LIB_DIR}/parsebgp_opts.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_common.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_notification.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_open.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_opts.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_route_refresh.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_ext_communities.h"
        "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_mp_reach.h"
        "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp.h"
        "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp_opts.h"
        "${LIBPARSEBGP_LIB_DIR}/mrt/parsebgp_mrt.h"
)

target_include_directories(parsebgp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBPARSEBGP_LIB_DIR}>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBPARSEBGP_LIB_DIR}/bgp>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBPARSEBGP_LIB_DIR}/bmp>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBPARSEBGP_LIB_DIR}/mrt>
)

target_sources(parsebgp PRIVATE
    "${LIBPARSEBGP_LIB_DIR}/parsebgp.c"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp.h"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_error.c"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_error.h"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_opts.c"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_opts.h"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_utils.c"
    "${LIBPARSEBGP_LIB_DIR}/parsebgp_utils.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_common.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_common.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_notification.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_notification.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_open.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_open.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_opts.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_opts.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_route_refresh.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_route_refresh.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_ext_communities.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_ext_communities.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_mp_reach.c"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_mp_reach.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_common_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_notification_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_open_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_route_refresh_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_ext_communities_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bgp/parsebgp_bgp_update_mp_reach_impl.h"
    "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp.c"
    "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp.h"
    "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp_opts.c"
    "${LIBPARSEBGP_LIB_DIR}/bmp/parsebgp_bmp_opts.h"
    "${LIBPARSEBGP_LIB_DIR}/mrt/parsebgp_mrt.c"
    "${LIBPARSEBGP_LIB_DIR}/mrt/parsebgp_mrt.h"
)

add_executable(parsebgp_tools_parsebgp "${LIBPARSEBGP_TOOLS_DIR}/parsebgp.c")

set_target_properties(parsebgp_tools_parsebgp PROPERTIES
  C_STANDARD 99
  OUTPUT_NAME parsebgp
)

target_link_libraries(parsebgp_tools_parsebgp parsebgp)

install(
  TARGETS parsebgp parsebgp_tools_parsebgp
  EXPORT parsebgp-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT parsebgp-config
  NAMESPACE parsebgp::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/parsebgp"
)
