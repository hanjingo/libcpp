#pragma once

#ifdef USE_BOOSTLOG
#include <libcpp/log/boost_log.hpp>
#endif

#ifdef USE_LOG4CPLUS
#include <libcpp/log/log4cplus.hpp>
#endif

#ifdef USE_SPDLOG
#include <libcpp/log/spd_log.hpp>
#endif