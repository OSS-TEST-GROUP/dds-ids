# Copyright (c) 2026 AUTOCRYPT Co., Ltd. (아우토크립트 주식회사)
# All rights reserved.
#
# [Terms of Use]
# 1. Modifications: Any modifications to this source code must be reported
# and shared with AUTOCRYPT Co., Ltd.
# (소스 코드 수정 시 아우토크립트 주식회사에 해당 내용을 반드시 고지해야 함)
# 2. Commercial Use: Prior written consultation and agreement with the
# Company are required for any commercial purposes.
# (상업적 이용 시 반드시 회사와 사전 협의 및 서면 합의 필요)
#
# For full details, please refer to the LICENSE.md file.

set(DDS_IDS_IDL_FILES
    DoorSwitch.idl
    DoorUnlockLockIndicator.idl
    detectionReport.idl
)

set(DDS_IDS_FASTDDSGEN_STAMP_DIR "${CMAKE_BINARY_DIR}/idl_codegen")
file(MAKE_DIRECTORY "${DDS_IDS_FASTDDSGEN_STAMP_DIR}")

if(DEFINED FASTDDSGEN_EXECUTABLE AND FASTDDSGEN_EXECUTABLE)
    set(FASTDDSGEN_EXE "${FASTDDSGEN_EXECUTABLE}" CACHE FILEPATH "Path to fastddsgen executable" FORCE)
endif()

if(DEFINED FASTDDSGEN_EXE AND FASTDDSGEN_EXE MATCHES "-NOTFOUND$")
    unset(FASTDDSGEN_EXE CACHE)
endif()

if(NOT DEFINED FASTDDSGEN_EXE)
    find_program(FASTDDSGEN_EXE NAMES fastddsgen HINTS ${CMAKE_PROGRAM_PATH} PATHS ENV PATH)

    if(FASTDDSGEN_EXE)
        set(FASTDDSGEN_EXE "${FASTDDSGEN_EXE}" CACHE FILEPATH "Path to fastddsgen executable" FORCE)
    endif()
endif()

message(STATUS "FASTDDSGEN_EXE: ${FASTDDSGEN_EXE}")

if(NOT FASTDDSGEN_EXE)
    message(STATUS "fastddsgen not found; target 'generate_idl' is unavailable")
else()
    set(DDS_IDS_FASTDDSGEN_STAMPS)

    foreach(_idl ${DDS_IDS_IDL_FILES})
        get_filename_component(_base "${_idl}" NAME_WE)
        set(_stamp "${DDS_IDS_FASTDDSGEN_STAMP_DIR}/${_base}.stamp")
        add_custom_command(
            OUTPUT "${_stamp}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DDS_IDS_TYPE_DIR}"
            COMMAND ${FASTDDSGEN_EXE} -d "${DDS_IDS_TYPE_DIR}" "${_idl}"
            COMMAND ${CMAKE_COMMAND} -E touch "${_stamp}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_idl}" "${FASTDDSGEN_EXE}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "fastddsgen: Generating types from ${_idl}"
            VERBATIM
        )
        message(STATUS "add custom command: ${FASTDDSGEN_EXE} -d ${DDS_IDS_TYPE_DIR} ${_idl}")
        list(APPEND DDS_IDS_FASTDDSGEN_STAMPS "${_stamp}")
    endforeach()

    # Make IDL generation part of the normal build so types are produced when building the project
    add_custom_target(generate_idl ALL DEPENDS ${DDS_IDS_FASTDDSGEN_STAMPS})
endif()

set(DDS_IDS_VEHICLE_TYPE_SOURCES
    "${DDS_IDS_TYPE_DIR}/DoorSwitchPubSubTypes.cxx"
    "${DDS_IDS_TYPE_DIR}/DoorSwitchTypeObjectSupport.cxx"
    "${DDS_IDS_TYPE_DIR}/DoorUnlockLockIndicatorPubSubTypes.cxx"
    "${DDS_IDS_TYPE_DIR}/DoorUnlockLockIndicatorTypeObjectSupport.cxx"
)

set(DDS_IDS_DETECTION_TYPE_SOURCES
    "${DDS_IDS_TYPE_DIR}/detectionReportPubSubTypes.cxx"
    "${DDS_IDS_TYPE_DIR}/detectionReportTypeObjectSupport.cxx"
)

# Mark generated sources so CMake won't expect them to exist at configure time
if(NOT "${DDS_IDS_VEHICLE_TYPE_SOURCES}" STREQUAL "")
    set_source_files_properties(${DDS_IDS_VEHICLE_TYPE_SOURCES} PROPERTIES GENERATED TRUE)
endif()

if(NOT "${DDS_IDS_DETECTION_TYPE_SOURCES}" STREQUAL "")
    set_source_files_properties(${DDS_IDS_DETECTION_TYPE_SOURCES} PROPERTIES GENERATED TRUE)
endif()

add_library(dds_ids_vehicle_types STATIC ${DDS_IDS_VEHICLE_TYPE_SOURCES})
target_include_directories(dds_ids_vehicle_types PUBLIC "${DDS_IDS_TYPE_DIR}")
target_link_libraries(dds_ids_vehicle_types PUBLIC ${DDS_IDS_RUNTIME_LIBS})
set_target_properties(dds_ids_vehicle_types PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

add_library(dds_ids_detection_types STATIC ${DDS_IDS_DETECTION_TYPE_SOURCES})
target_include_directories(dds_ids_detection_types PUBLIC "${DDS_IDS_TYPE_DIR}")
target_link_libraries(dds_ids_detection_types PUBLIC ${DDS_IDS_RUNTIME_LIBS})
set_target_properties(dds_ids_detection_types PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

if(TARGET generate_idl)
    add_dependencies(dds_ids_vehicle_types generate_idl)
    add_dependencies(dds_ids_detection_types generate_idl)
endif()