#ifndef MERIDIAN_LOG_H
#define MERIDIAN_LOG_H

#include <spdlog/spdlog.h>
#include <fmt/core.h>

template<typename... Args>
void Meridian_info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    spdlog::info(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Meridian_warning(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    spdlog::warn(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Meridian_error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    spdlog::error(fmt, std::forward<Args>(args)...);
}

#endif//MERIDIAN_LOG_H
