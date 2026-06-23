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

#ifndef __MAIN_H__
#define __MAIN_H__

#include "MainBase.hpp"
#include "app/SecurityClientApp.hpp"

namespace AC {
namespace ddsIds {

class Main : public MainBase
{
public:
    Main();

    bool daemonizeMode = false;

    virtual bool defineArgs(cxxopts::Options& options) override;
    virtual bool configure(const cxxopts::ParseResult& optRes) override;
    int start() override;
    int stop() override;

private:
    AC::ddsIds::securityClient::SecurityClientApp app_;
};

// Use `g_main` from MainBase (extern MainBase* g_main)

}  // namespace ddsIds
}  // namespace AC

#endif  //__MAIN_H__
