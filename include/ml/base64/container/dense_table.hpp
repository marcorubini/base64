#pragma once
#include <algorithm>
#include <array>
#include <numeric>

namespace ml::internal::base64
{
  template<std::unsigned_integral KeyType, std::unsigned_integral ValueType>
  struct dense_table_traits
  {
    using key_type = KeyType;
    using value_type = ValueType;
    std::size_t capacity;
  };

  template<dense_table_traits Traits>
  struct dense_table
  {
    using traits_type = decltype (Traits);
    using key_type = typename traits_type::key_type;
    using value_type = typename traits_type::value_type;

    static constexpr std::size_t capacity = Traits.capacity;

    // ---

    std::array<value_type, capacity> _values {};

    // ---

    constexpr void insert (key_type key, value_type value) noexcept
    {
      _values[key] = value;
    }

    constexpr void finalize () noexcept
    {}

    // ---

    [[nodiscard]] constexpr value_type get (key_type key) const noexcept
    {
      return _values[key];
    }
  };
} // namespace ml::internal::base64