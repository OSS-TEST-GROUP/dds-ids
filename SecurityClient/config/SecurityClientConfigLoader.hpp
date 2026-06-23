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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_CONFIG_SECURITYCLIENTCONFIGLOADER_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_CONFIG_SECURITYCLIENTCONFIGLOADER_HPP

#include "SecurityClientConfig.hpp"

#include <filesystem>

namespace AC {
namespace ddsIds {
namespace securityClient {

class SecurityClientConfigLoader
{
public:
    bool loadRuntimeConfig(const std::filesystem::path& path, SecurityClientConfig& config) const;
    bool loadPolicyConfig(const std::filesystem::path& path, PolicyConfig& config) const;
    bool load(const std::filesystem::path& runtimePath, const std::filesystem::path& policyPath,
              SecurityClientConfig& config) const;
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
