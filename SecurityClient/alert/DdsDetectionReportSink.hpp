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

#ifndef DDS_IDS_OSS_SECURITYCLIENT_ALERT_DDSDETECTIONREPORTSINK_HPP
#define DDS_IDS_OSS_SECURITYCLIENT_ALERT_DDSDETECTIONREPORTSINK_HPP

#include "IAlertSink.hpp"
#include "detectionReportPubSubTypes.hpp"
#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/domain/DomainParticipantFactory.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/topic/Topic.hpp"
#include "fastdds/dds/topic/TypeSupport.hpp"

#include <atomic>

namespace AC {
namespace ddsIds {
namespace securityClient {

class DdsDetectionReportSink : public IAlertSink
{
public:
    bool configure(const AlertSinkConfig& config, const SystemConfig& systemConfig) override;
    bool initialize() override;
    bool publish(const std::vector<DetectionFinding>& findings) override;
    void stop() override;
    void finalize() override;

private:
    bool createWriter();
    unsigned int currentWriterMask() const;
    bool waitForMatch();
    void releaseWriter();

    AlertSinkConfig config_ {};
    SystemConfig systemConfig_ {};
    eprosima::fastdds::dds::DomainParticipant* participant_ {nullptr};
    eprosima::fastdds::dds::Publisher* publisher_ {nullptr};
    eprosima::fastdds::dds::Topic* topic_ {nullptr};
    eprosima::fastdds::dds::DataWriter* writer_ {nullptr};
    eprosima::fastdds::dds::TypeSupport typeSupport_;
    unsigned int writerMask_ {0};
    std::atomic_bool initialized_ {false};
};

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC

#endif
