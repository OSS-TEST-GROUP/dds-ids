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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_ALERT_IRESPONSEHANDLER_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_ALERT_IRESPONSEHANDLER_HPP

#include "../config/SecurityClientConfig.hpp"
#include "DetectionFinding.hpp"

namespace AC {
namespace ddsIds {
namespace securityClient {

class IResponseHandler
{
public:
    virtual ~IResponseHandler() = default;

    virtual bool configure(const ResponseHandlerConfig& config) = 0;
    virtual bool initialize() = 0;
    virtual bool handle(const DetectionFinding& finding) = 0;
    virtual void stop() = 0;
    virtual void finalize() = 0;
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
