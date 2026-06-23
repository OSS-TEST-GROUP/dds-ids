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

#include "../../collection/CollectionStage.hpp"

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace {

using AC::ddsIds::securityClient::CollectedSample;
using AC::ddsIds::securityClient::CollectionConfig;
using AC::ddsIds::securityClient::CollectionStage;
using AC::ddsIds::securityClient::IDataCollector;
using AC::ddsIds::securityClient::SourceConfig;

class FakeCollector : public IDataCollector
{
public:
    bool configure(const SourceConfig& config) override
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

    bool poll(std::vector<CollectedSample>& outSamples) override
    {
        outSamples = samples_;
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
    std::vector<CollectedSample> samples_;

private:
    SourceConfig config_ {};
};

class FailingCollector : public IDataCollector
{
public:
    bool configure(const SourceConfig& config) override
    {
        (void)config;
        return true;
    }

    bool initialize() override
    {
        return true;
    }

    bool poll(std::vector<CollectedSample>& outSamples) override
    {
        (void)outSamples;
        return false;
    }

    void stop() override
    {
    }
    void finalize() override
    {
    }
};

TEST(CollectionStageTest, PollOnceUsesCollectorsAndStoresLatestSamples)
{
    CollectionStage stage;

    CollectionConfig config;
    config.sources.push_back(SourceConfig {"door_switch", "dds-default", "rt/DoorSwitch", "DoorSwitch_", {}});
    ASSERT_TRUE(stage.configure(config));

    auto collector = std::make_unique<FakeCollector>();
    collector->samples_.push_back(CollectedSample {"door_switch", "dds", "rt/DoorSwitch", "DoorSwitch_", 1234, {{}}});
    auto* collectorPtr = collector.get();

    stage.addCollector(std::move(collector));

    ASSERT_TRUE(stage.initialize());
    EXPECT_TRUE(collectorPtr->initialized_);

    std::vector<CollectedSample> polled;
    ASSERT_TRUE(stage.pollOnce(polled));
    EXPECT_EQ(1u, polled.size());
    EXPECT_TRUE(stage.observationStore().latestByTopic("rt/DoorSwitch").has_value());
    EXPECT_EQ("door_switch", stage.observationStore().latestByTopic("rt/DoorSwitch")->sourceId);
}

TEST(CollectionStageTest, PollOnceReturnsFalseWhenCollectorFails)
{
    CollectionStage stage;

    CollectionConfig config;
    config.sources.push_back(SourceConfig {"door_switch", "dds-default", "rt/DoorSwitch", "DoorSwitch_", {}});
    ASSERT_TRUE(stage.configure(config));

    stage.addCollector(std::make_unique<FailingCollector>());

    ASSERT_TRUE(stage.initialize());

    std::vector<CollectedSample> polled;
    EXPECT_FALSE(stage.pollOnce(polled));
}

}  // namespace
