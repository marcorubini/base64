#pragma once
#include <concepts>

namespace ml::internal::base64
{
  template<class T, class KeyType, class ValueType>
  concept Dictionary = requires (T dict, KeyType key, ValueType value)
  {
    // clang-format off
    requires std::default_initializable<T>;
    requires std::unsigned_integral<KeyType>;
    requires std::unsigned_integral<ValueType>;
    { dict.finalize() };
    { dict.get(key) } -> std::same_as<ValueType>;
    // clang-format on
  };
} // namespace ml::internal::base64