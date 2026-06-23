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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_COLLECTION_COLLECTIONSTAGE_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_COLLECTION_COLLECTIONSTAGE_HPP

#include "IDataCollector.hpp"
#include "ObservationStore.hpp"

#include <memory>
#include <vector>

namespace AC {
namespace ddsIds {
namespace securityClient {

class CollectionStage
{
public:
    bool configure(const CollectionConfig& config);
    bool initialize();
    bool start();
    bool pollOnce(std::vector<CollectedSample>& outSamples);
    void stop();
    void finalize();

    void addCollector(std::unique_ptr<IDataCollector> collector);
    ObservationStore& observationStore();
    const ObservationStore& observationStore() const;

private:
    CollectionConfig config_;
    ObservationStore observationStore_;
    std::vector<std::unique_ptr<IDataCollector>> collectors_;
    bool running_ {false};
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
