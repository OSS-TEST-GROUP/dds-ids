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

#include "../../alert/DdsDetectionReportSink.hpp"

#include <chrono>
#include <fastdds/dds/core/ReturnCode.hpp>
#include <fastdds/dds/core/status/StatusMask.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <thread>

#include <gtest/gtest.h>

namespace {

using AC::ddsIds::securityClient::AlertSinkConfig;
using AC::ddsIds::securityClient::AlertSinkType;
using AC::ddsIds::securityClient::DdsDetectionReportSink;
using AC::ddsIds::securityClient::DetectionFinding;
using AC::ddsIds::securityClient::Severity;
using AC::ddsIds::securityClient::SystemConfig;
using eprosima::fastdds::dds::DataReader;
using eprosima::fastdds::dds::DataReaderQos;
using eprosima::fastdds::dds::DomainParticipant;
using eprosima::fastdds::dds::DomainParticipantFactory;
using eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT;
using eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
using eprosima::fastdds::dds::StatusMask;
using eprosima::fastdds::dds::Subscriber;
using eprosima::fastdds::dds::Topic;
using eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;
using eprosima::fastdds::dds::TypeSupport;

class DetectionReportSubscriber
{
public:
    bool initialize(int domainId, const std::string& topicName)
    {
        participant_ =
            DomainParticipantFactory::get_shared_instance()->create_participant(domainId, PARTICIPANT_QOS_DEFAULT);
        if (participant_ == nullptr)
        {
            return false;
        }

        typeSupport_ = TypeSupport(new security::DetectionReportPubSubType());
        typeSupport_.register_type(participant_);

        subscriber_ = participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr,
                                                      StatusMask::none());
        if (subscriber_ == nullptr)
        {
            finalize();
            return false;
        }

        topic_ = participant_->create_topic(topicName.c_str(), typeSupport_.get_type_name(), TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr)
        {
            finalize();
            return false;
        }

        DataReaderQos readerQos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT;
        readerQos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        readerQos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
        readerQos.history().depth = 1;
        reader_ = subscriber_->create_datareader(topic_, readerQos, nullptr, StatusMask::all());
        if (reader_ == nullptr)
        {
            finalize();
            return false;
        }

        return true;
    }

    bool waitForSample(security::DetectionReport& outReport, int timeoutMs)
    {
        if (reader_ == nullptr)
        {
            return false;
        }

        const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
        while (std::chrono::steady_clock::now() < deadline)
        {
            eprosima::fastdds::dds::SampleInfo info;
            while (reader_->take_next_sample(&outReport, &info) == eprosima::fastdds::dds::RETCODE_OK)
            {
                if (info.valid_data)
                {
                    return true;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        return false;
    }

    void finalize()
    {
        if (participant_ != nullptr)
        {
            participant_->delete_contained_entities();
            DomainParticipantFactory::get_shared_instance()->delete_participant(participant_);
        }

        participant_ = nullptr;
        subscriber_ = nullptr;
        topic_ = nullptr;
        reader_ = nullptr;
    }

    ~DetectionReportSubscriber()
    {
        finalize();
    }

private:
    DomainParticipant* participant_ {nullptr};
    Subscriber* subscriber_ {nullptr};
    Topic* topic_ {nullptr};
    DataReader* reader_ {nullptr};
    TypeSupport typeSupport_;
};

DetectionFinding makeFinding()
{
    return DetectionFinding {"detector-1",    101u, Severity::high, "rt/DoorUnlockLockIndicator",
                             "rule mismatch", {},   12345};
}

TEST(DdsDetectionReportSinkTest, PublishFailsWhenNoSubscriberMatches)
{
    DdsDetectionReportSink sink;
    SystemConfig systemConfig;
    systemConfig.clientId = "sink-no-match";

    ASSERT_TRUE(sink.configure(
        AlertSinkConfig {"sink", AlertSinkType::dds_detection_report, true, 31, "DetectionReportNoMatch"},
        systemConfig));
    ASSERT_TRUE(sink.initialize());

    std::vector<DetectionFinding> findings {makeFinding()};
    EXPECT_FALSE(sink.publish(findings));

    sink.stop();
    sink.finalize();
}

TEST(DdsDetectionReportSinkTest, PublishSendsDetectionReportWhenSubscriberMatches)
{
    DetectionReportSubscriber subscriber;
    ASSERT_TRUE(subscriber.initialize(32, "DetectionReportMatched"));

    DdsDetectionReportSink sink;
    SystemConfig systemConfig;
    systemConfig.clientId = "sink-match";

    ASSERT_TRUE(sink.configure(
        AlertSinkConfig {"sink", AlertSinkType::dds_detection_report, true, 32, "DetectionReportMatched"},
        systemConfig));
    ASSERT_TRUE(sink.initialize());

    std::vector<DetectionFinding> findings {makeFinding()};
    ASSERT_TRUE(sink.publish(findings));

    security::DetectionReport report;
    ASSERT_TRUE(subscriber.waitForSample(report, 2000));
    EXPECT_EQ("sink-match", report.client_id());
    EXPECT_EQ("rt/DoorUnlockLockIndicator", report.dds_topic());
    EXPECT_EQ("rule mismatch", report.rule());

    sink.stop();
    sink.finalize();
}

}  // namespace