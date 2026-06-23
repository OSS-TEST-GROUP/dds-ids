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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_APP_STAGERUNTIME_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_APP_STAGERUNTIME_HPP

#include "SecurityClientApp.hpp"

namespace AC {
namespace ddsIds {
namespace securityClient {

class StageRuntime
{
public:
    bool configure(const AppLaunchOptions& options);
    bool configure(const SecurityClientConfig& config, const AppLaunchOptions& options = {});
    bool initialize();
    int run();
    void stop();
    void finalize();

private:
    AppLaunchOptions launchOptions_ {};
    SecurityClientConfig config_ {};
    SecurityClientApp app_ {};
    bool configured_ {false};
    bool initialized_ {false};
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
