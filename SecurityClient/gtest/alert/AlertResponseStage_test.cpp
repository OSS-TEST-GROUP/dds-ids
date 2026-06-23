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

#include "../../alert/AlertResponseStage.hpp"

#include "Log.hpp"

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace {

using AC::ddsIds::securityClient::AlertResponseConfig;
using AC::ddsIds::securityClient::AlertResponseStage;
using AC::ddsIds::securityClient::AlertSinkConfig;
using AC::ddsIds::securityClient::AlertSinkType;
using AC::ddsIds::securityClient::DetectionFinding;
using AC::ddsIds::securityClient::IAlertSink;
using AC::ddsIds::securityClient::IResponseHandler;
using AC::ddsIds::securityClient::ResponseHandlerConfig;
using AC::ddsIds::securityClient::ResponseHandlerType;
using AC::ddsIds::securityClient::Severity;
using AC::ddsIds::securityClient::SystemConfig;

class FakeAlertSink : public IAlertSink
{
public:
    bool configure(const AlertSinkConfig& config, const SystemConfig& systemConfig) override
    {
        config_ = config;
        systemConfig_ = systemConfig;
        configured_ = true;
        return true;
    }

    bool initialize() override
    {
        initialized_ = true;
        return true;
    }

    bool publish(const std::vector<DetectionFinding>& findings) override
    {
        publishedFindings = findings;
        publishCalled_ = true;
        return true;
    }

    void stop() override
    {
    }
    void finalize() override
    {
    }

    bool configured_ {false};
    bool initialized_ {false};
    bool publishCalled_ {false};
    std::vector<DetectionFinding> publishedFindings;

private:
    AlertSinkConfig config_ {};
    SystemConfig systemConfig_ {};
};

class FailingSink : public IAlertSink
{
public:
    bool configure(const AlertSinkConfig& config, const SystemConfig& systemConfig) override
    {
        (void)config;
        (void)systemConfig;
        return true;
    }

    bool initialize() override
    {
        return true;
    }

    bool publish(const std::vector<DetectionFinding>& findings) override
    {
        (void)findings;
        return false;
    }

    void stop() override
    {
    }
    void finalize() override
    {
    }
};

class FakeResponseHandler : public IResponseHandler
{
public:
    bool configure(const ResponseHandlerConfig& config) override
    {
        config_ = config;
        configured_ = true;
        return true;
    }

    bool initialize() override
    {
        initialized_ = true;
        return true;
    }

    bool handle(const DetectionFinding& finding) override
    {
        handledFindings.push_back(finding);
        handled_ = true;
        return true;
    }

    void stop() override
    {
    }
    void finalize() override
    {
    }

    bool configured_ {false};
    bool initialized_ {false};
    bool handled_ {false};
    std::vector<DetectionFinding> handledFindings;

private:
    ResponseHandlerConfig config_ {};
};

TEST(AlertResponseStageTest, PublishRoutesFindingsToConfiguredSinksAndHandlers)
{
    AlertResponseStage stage;
    SystemConfig systemConfig;
    systemConfig.clientId = "sc-test";

    AlertResponseConfig config;
    config.alertSinks.push_back(
        AlertSinkConfig {"sink-1", AlertSinkType::dds_detection_report, true, 0, "DetectionReport"});
    config.responseHandlers.push_back(ResponseHandlerConfig {"handler-1", ResponseHandlerType::noop, true});

    auto sink = std::make_unique<FakeAlertSink>();
    auto* sinkPtr = sink.get();
    auto handler = std::make_unique<FakeResponseHandler>();
    auto* handlerPtr = handler.get();

    ASSERT_TRUE(stage.configure(systemConfig, config));
    stage.addAlertSink(std::move(sink));
    stage.addResponseHandler(std::move(handler));

    ASSERT_TRUE(stage.initialize());

    std::vector<DetectionFinding> findings;
    findings.push_back({"detector-1", 100u, Severity::high, "rt/DoorUnlockLockIndicator", "rule mismatch", {}, 42});

    ASSERT_TRUE(stage.publish(findings));
    EXPECT_TRUE(sinkPtr->publishCalled_);
    EXPECT_EQ(1u, sinkPtr->publishedFindings.size());
    EXPECT_EQ("detector-1", sinkPtr->publishedFindings.front().detectorId);
    EXPECT_TRUE(handlerPtr->handled_);
    EXPECT_EQ(1u, handlerPtr->handledFindings.size());
}

TEST(AlertResponseStageTest, PublishReturnsFalseWhenSinkOrHandlerFails)
{
    AlertResponseStage stage;
    SystemConfig systemConfig;

    AlertResponseConfig config;
    config.alertSinks.push_back(
        AlertSinkConfig {"sink-fail", AlertSinkType::dds_detection_report, true, 0, "DetectionReport"});
    config.responseHandlers.push_back(ResponseHandlerConfig {"handler-fail", ResponseHandlerType::noop, true});

    auto sink = std::make_unique<FailingSink>();
    auto handler = std::make_unique<FakeResponseHandler>();
    handler->handled_ = false;

    ASSERT_TRUE(stage.configure(systemConfig, config));
    stage.addAlertSink(std::move(sink));
    stage.addResponseHandler(std::move(handler));

    ASSERT_TRUE(stage.initialize());

    std::vector<DetectionFinding> findings;
    findings.push_back({"detector-1", 100u, Severity::high, "rt/DoorUnlockLockIndicator", "rule mismatch", {}, 42});

    EXPECT_FALSE(stage.publish(findings));
}

}  // namespace
