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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_ALERT_ALERTRESPONSESTAGE_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_ALERT_ALERTRESPONSESTAGE_HPP

#include "IAlertSink.hpp"
#include "IResponseHandler.hpp"

#include <memory>
#include <vector>

namespace AC {
namespace ddsIds {
namespace securityClient {

class AlertResponseStage
{
public:
    bool configure(const SystemConfig& systemConfig, const AlertResponseConfig& config);
    bool initialize();
    bool publish(const std::vector<DetectionFinding>& findings);
    void stop();
    void finalize();

    void addAlertSink(std::unique_ptr<IAlertSink> alertSink);
    void addResponseHandler(std::unique_ptr<IResponseHandler> responseHandler);

private:
    SystemConfig systemConfig_;
    AlertResponseConfig config_;
    std::vector<std::unique_ptr<IAlertSink>> alertSinks_;
    std::vector<std::unique_ptr<IResponseHandler>> responseHandlers_;
    bool running_ {false};
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
