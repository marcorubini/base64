#pragma once
#include <string_view>

namespace ml::internal::base64
{
  constexpr inline auto default_charset =
    std::string_view ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}