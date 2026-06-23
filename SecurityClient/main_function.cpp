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

#include "Log.hpp"
#include "Main.hpp"

#ifdef USE_BACKWARD
#include "backward.hpp"
#endif

int main(int argc, const char* const* argv, const char* const* env)
{
#ifdef USE_BACKWARD
    backward::SignalHandling backwardTlsTest;
#endif
    // spdlog::set_level(spdlog::level::trace);
    // spdlog::set_pattern("%^[%H:%M:%S.%e %L %@ %!]%$ %v");

    static AC::ddsIds::Main main;

    AC::ddsIds::g_main = &main;

    return main.startMain(argc, argv, env);
}
