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

#include "NoopResponseHandler.hpp"

#include "Log.hpp"

namespace AC {
namespace ddsIds {
namespace securityClient {

bool NoopResponseHandler::configure(const ResponseHandlerConfig& config)
{
    config_ = config;
    LOG_TRA("NoopResponseHandler configured (id={})", config.id);
    return true;
}

bool NoopResponseHandler::initialize()
{
    LOG_INF("NoopResponseHandler initialized");
    return true;
}

bool NoopResponseHandler::handle(const DetectionFinding& finding)
{
    LOG_DBG("NoopResponseHandler accepted finding {}", finding.detectorId);
    (void)finding;
    return true;
}

void NoopResponseHandler::stop()
{
}

void NoopResponseHandler::finalize()
{
}

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC
