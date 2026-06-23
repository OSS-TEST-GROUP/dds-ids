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

#include "StageRuntime.hpp"

#include "Log.hpp"

namespace AC {
namespace ddsIds {
namespace securityClient {

bool StageRuntime::configure(const AppLaunchOptions& options)
{
    launchOptions_ = options;
    configured_ = false;

    if (!app_.configure(options))
    {
        LOG_ERR("StageRuntime configure failed for client {}", options.clientId);
        return false;
    }

    configured_ = true;
    return true;
}

bool StageRuntime::configure(const SecurityClientConfig& config, const AppLaunchOptions& options)
{
    config_ = config;
    launchOptions_ = options;

    if (!options.clientId.empty())
    {
        config_.system.clientId = options.clientId;
    }

    if (!app_.configure(options, config_))
    {
        LOG_ERR("StageRuntime configure override failed for client {}", options.clientId);
        return false;
    }

    configured_ = true;
    return true;
}

bool StageRuntime::initialize()
{
    if (!configured_)
    {
        LOG_ERR("StageRuntime initialize called before configure");
        return false;
    }

    if (!app_.initialize())
    {
        LOG_ERR("StageRuntime app initialization failed");
        return false;
    }

    initialized_ = true;
    return true;
}

int StageRuntime::run()
{
    if (!initialized_)
    {
        return EXIT_FAILURE;
    }

    return app_.run();
}

void StageRuntime::stop()
{
    if (initialized_)
    {
        app_.stop();
    }
}

void StageRuntime::finalize()
{
    if (initialized_)
    {
        app_.finalize();
    }

    initialized_ = false;
    configured_ = false;
}

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC
