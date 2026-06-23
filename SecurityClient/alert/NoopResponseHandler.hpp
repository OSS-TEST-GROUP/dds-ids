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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_ALERT_NOOPRESPONSEHANDLER_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_ALERT_NOOPRESPONSEHANDLER_HPP

#include "IResponseHandler.hpp"

namespace AC {
namespace ddsIds {
namespace securityClient {

class NoopResponseHandler : public IResponseHandler
{
public:
    bool configure(const ResponseHandlerConfig& config) override;
    bool initialize() override;
    bool handle(const DetectionFinding& finding) override;
    void stop() override;
    void finalize() override;

private:
    ResponseHandlerConfig config_;
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
