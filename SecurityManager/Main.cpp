/*
 * Copyright (c) 2026 AUTOCRYPT Co., Ltd. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * AUTOCRYPT Co., Ltd. ("Confidential Information").
 *
 * You shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with AUTOCRYPT Co., Ltd.
 *
 * For more information, please refer to the LICENSE.md file in the
 * root directory or contact contact@autocrypt.io.
 */

#include "Main.hpp"

#include "Log.hpp"
#include "Util.hpp"
#include "config_dds_ids.h"
#include "cxxopts.hpp"

#include <csignal>
#include <fstream>
#include <iostream>
#include <string>

namespace AC {
namespace ddsIds {

Main::Main()
{
    programName = "security_manager";
    helpString = "Bridge DetectionReport topics across DDS domains";
}

bool Main::defineArgs(cxxopts::Options& options)
{
    // clang-format off
        options.add_options()
        ("s,source-domain", "Source DDS domain", cxxopts::value<int>()->default_value(std::to_string(DEFAULT_INPUT_DOMAIN_ID)))
        ("d,dest-domain", "Destination DDS domain", cxxopts::value<int>()->default_value(std::to_string(DEFAULT_OUTPUT_DOMAIN_ID)))
        ("t,topic", "Override source DetectionReport topic", cxxopts::value<std::string>()->default_value(""));
    // clang-format on
    return true;
}

bool Main::configure(const cxxopts::ParseResult& optRes)
{
    managerState.config(optRes["source-domain"].as<int>(), optRes["dest-domain"].as<int>(),
                        optRes["topic"].as<std::string>());
    return true;
}

int Main::start()
{
    LOG_INF("[SecurityManager] started");

    return managerState.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}

int Main::stop()
{
    LOG_INF("Stopping");
    managerState.stop();
    return 0;
}

}  // namespace ddsIds
}  // namespace AC
