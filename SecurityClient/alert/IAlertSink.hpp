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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_ALERT_IALERTSINK_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_ALERT_IALERTSINK_HPP

#include "../config/SecurityClientConfig.hpp"
#include "DetectionFinding.hpp"

#include <vector>

namespace AC {
namespace ddsIds {
namespace securityClient {

class IAlertSink
{
public:
    virtual ~IAlertSink() = default;

    virtual bool configure(const AlertSinkConfig& config, const SystemConfig& systemConfig) = 0;
    virtual bool initialize() = 0;
    virtual bool publish(const std::vector<DetectionFinding>& findings) = 0;
    virtual void stop() = 0;
    virtual void finalize() = 0;
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
