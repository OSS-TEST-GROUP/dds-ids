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

project(SecurityManager LANGUAGES CXX)

add_executable(SecurityManager
    main_function.cpp
    Main.cpp
    ManagerState.cpp
)

target_include_directories(SecurityManager
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}"
)

target_link_libraries(SecurityManager
    PRIVATE
    libdds-ids::common
    dds_ids_vehicle_types
    dds_ids_detection_types
    PUBLIC
    fmt::fmt
    spdlog::spdlog
    cxxopts::cxxopts
)

set_target_properties(SecurityManager PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS YES
)

install(TARGETS SecurityManager RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
