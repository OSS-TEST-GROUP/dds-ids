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

#include "CollectionStage.hpp"

#include "DdsCollector.hpp"
#include "Log.hpp"

#include <algorithm>

namespace {

AC::ddsIds::securityClient::SourceConfig resolve_source_config(
    const AC::ddsIds::securityClient::SourceConfig& source,
    const AC::ddsIds::securityClient::CollectionConfig& collectionConfig)
{
    auto resolved = source;

    const auto transportIt = std::find_if(collectionConfig.transports.begin(), collectionConfig.transports.end(),
                                          [&](const AC::ddsIds::securityClient::TransportConfig& transport) {
                                              return transport.id == source.transportId;
                                          });
    if (transportIt != collectionConfig.transports.end())
    {
        resolved.domainId = transportIt->domainId;
    }

    return resolved;
}

}  // namespace

namespace AC {
namespace ddsIds {
namespace securityClient {

bool CollectionStage::configure(const CollectionConfig& config)
{
    config_ = config;
    LOG_TRA("CollectionStage configured with {} source(s) and {} transport(s)", config.sources.size(),
            config.transports.size());
    return true;
}

bool CollectionStage::initialize()
{
    if (collectors_.empty())
    {
        for (const auto& source : config_.sources)
        {
            collectors_.push_back(std::make_unique<DdsCollector>());
        }
    }

    for (std::size_t index = 0; index < collectors_.size(); ++index)
    {
        const auto source =
            index < config_.sources.size() ? resolve_source_config(config_.sources[index], config_) : SourceConfig {};
        LOG_TRA("CollectionStage initializing collector {} for source id={} topic={} transport={} domain={}", index,
                source.id, source.topic, source.transportId, source.domainId);

        if (!collectors_[index]->configure(source))
        {
            LOG_ERR("CollectionStage collector {} configure failed", index);
            return false;
        }

        if (!collectors_[index]->initialize())
        {
            LOG_ERR("CollectionStage collector {} initialize failed", index);
            return false;
        }

        LOG_DBG("CollectionStage collector {} is ready for topic={} domain={}", index, source.topic, source.domainId);
    }

    running_ = true;
    LOG_INF("CollectionStage initialized with {} collector(s)", collectors_.size());
    return true;
}

bool CollectionStage::start()
{
    running_ = true;
    LOG_INF("CollectionStage started");
    return true;
}

bool CollectionStage::pollOnce(std::vector<CollectedSample>& outSamples)
{
    outSamples.clear();

    if (!running_)
    {
        LOG_WRN("CollectionStage pollOnce called while not running");
        return false;
    }

    LOG_TRA("CollectionStage pollOnce started with {} collector(s)", collectors_.size());
    for (std::size_t index = 0; index < collectors_.size(); ++index)
    {
        auto& collector = collectors_[index];
        std::vector<CollectedSample> samples;
        if (!collector->poll(samples))
        {
            LOG_ERR("CollectionStage collector {} poll failed", index);
            return false;
        }

        LOG_DBG("CollectionStage collector {} returned {} sample(s)", index, samples.size());
        for (const auto& sample : samples)
        {
            observationStore_.upsert(sample);
            outSamples.push_back(sample);
        }
    }

    LOG_TRA("CollectionStage pollOnce produced {} sample(s)", outSamples.size());
    return true;
}

void CollectionStage::stop()
{
    for (auto& collector : collectors_)
    {
        collector->stop();
    }
    running_ = false;
}

void CollectionStage::finalize()
{
    for (auto& collector : collectors_)
    {
        collector->finalize();
    }

    running_ = false;
    observationStore_.clear();
    collectors_.clear();
}

void CollectionStage::addCollector(std::unique_ptr<IDataCollector> collector)
{
    if (collector == nullptr)
    {
        return;
    }

    collectors_.push_back(std::move(collector));
}

ObservationStore& CollectionStage::observationStore()
{
    return observationStore_;
}

const ObservationStore& CollectionStage::observationStore() const
{
    return observationStore_;
}

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC
