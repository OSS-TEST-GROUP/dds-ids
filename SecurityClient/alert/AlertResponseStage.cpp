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

#include "AlertResponseStage.hpp"

#include "Log.hpp"

#include <fmt/core.h>

namespace AC {
namespace ddsIds {
namespace securityClient {

bool AlertResponseStage::configure(const SystemConfig& systemConfig, const AlertResponseConfig& config)
{
    systemConfig_ = systemConfig;
    config_ = config;
    LOG_TRA("AlertResponseStage configured with {} sink(s) and {} handler(s)", config.alertSinks.size(),
            config.responseHandlers.size());
    return true;
}

bool AlertResponseStage::initialize()
{
    running_ = true;
    LOG_INF("AlertResponseStage initialized with {} sink(s) and {} handler(s)", alertSinks_.size(),
            responseHandlers_.size());

    for (auto& sink : alertSinks_)
    {
        if (!sink->initialize())
        {
            return false;
        }
    }

    for (auto& handler : responseHandlers_)
    {
        if (!handler->initialize())
        {
            return false;
        }
    }

    return true;
}

bool AlertResponseStage::publish(const std::vector<DetectionFinding>& findings)
{
    LOG_TRA("AlertResponseStage publish requested for {} finding(s) with {} sink(s) and {} handler(s)", findings.size(),
            alertSinks_.size(), responseHandlers_.size());
    if (!running_)
    {
        return false;
    }

    bool ok = true;

    for (auto& sink : alertSinks_)
    {
        if (!sink->publish(findings))
        {
            LOG_ERR("AlertResponseStage sink publish failed");
            ok = false;
        }
    }

    for (const auto& finding : findings)
    {
        LOG_DBG("AlertResponseStage forwarding finding detector={} rule={} topic={} summary={}", finding.detectorId,
                finding.ruleId, finding.topicName, finding.summary);
        for (auto& handler : responseHandlers_)
        {
            if (!handler->handle(finding))
            {
                LOG_ERR("AlertResponseStage handler failed for detector {}", finding.detectorId);
                ok = false;
            }
        }
    }

    return ok;
}

void AlertResponseStage::stop()
{
    LOG_INF("AlertResponseStage stopping {} sink(s) and {} handler(s)", alertSinks_.size(), responseHandlers_.size());
    for (auto& sink : alertSinks_)
    {
        sink->stop();
    }

    for (auto& handler : responseHandlers_)
    {
        handler->stop();
    }

    running_ = false;
}

void AlertResponseStage::finalize()
{
    LOG_TRA("AlertResponseStage finalizing alert pipeline");
    for (auto& sink : alertSinks_)
    {
        sink->finalize();
    }

    for (auto& handler : responseHandlers_)
    {
        handler->finalize();
    }

    alertSinks_.clear();
    responseHandlers_.clear();
    running_ = false;
}

void AlertResponseStage::addAlertSink(std::unique_ptr<IAlertSink> alertSink)
{
    if (alertSink == nullptr)
    {
        return;
    }

    const auto index = alertSinks_.size();
    const AlertSinkConfig sinkConfig =
        index < config_.alertSinks.size() ? config_.alertSinks[index] : AlertSinkConfig {};
    (void)alertSink->configure(sinkConfig, systemConfig_);
    alertSinks_.push_back(std::move(alertSink));
}

void AlertResponseStage::addResponseHandler(std::unique_ptr<IResponseHandler> responseHandler)
{
    if (responseHandler == nullptr)
    {
        return;
    }

    const auto index = responseHandlers_.size();
    const ResponseHandlerConfig handlerConfig =
        index < config_.responseHandlers.size() ? config_.responseHandlers[index] : ResponseHandlerConfig {};
    (void)responseHandler->configure(handlerConfig);
    responseHandlers_.push_back(std::move(responseHandler));
}

}  // namespace securityClient
}  // namespace ddsIds
}  // namespace AC
