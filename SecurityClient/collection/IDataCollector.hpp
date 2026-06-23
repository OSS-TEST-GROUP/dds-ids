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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_COLLECTION_IDATACOLLECTOR_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_COLLECTION_IDATACOLLECTOR_HPP

#include "../config/SecurityClientConfig.hpp"
#include "CollectedSample.hpp"

#include <vector>

namespace AC {
namespace ddsIds {
namespace securityClient {

class IDataCollector
{
public:
    virtual ~IDataCollector() = default;

    virtual bool configure(const SourceConfig& config) = 0;
    virtual bool initialize() = 0;
    virtual bool poll(std::vector<CollectedSample>& outSamples) = 0;
    virtual void stop() = 0;
    virtual void finalize() = 0;
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
