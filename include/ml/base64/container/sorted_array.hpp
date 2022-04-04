#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>

namespace ml::internal::base64
{
  template<std::unsigned_integral KeyType, std::unsigned_integral ValueType>
  struct sorted_array_traits
  {
    using key_type = KeyType;
    using value_type = ValueType;
    std::size_t capacity;
  };

  template<sorted_array_traits Traits>
  struct sorted_array
  {
    using traits_type = decltype (Traits);
    using key_type = typename traits_type::key_type;
    using value_type = typename traits_type::value_type;

    static constexpr std::size_t capacity = Traits.capacity;

    // ---

    std::array<key_type, capacity> _keys {};
    std::array<value_type, capacity> _values {};

    // ---

    sorted_array () = default;

    // ---

    constexpr void sort ()
    {
      auto indices = std::array<unsigned, capacity> {};
      std::iota (indices.begin (), indices.end (), 0u);

      if constexpr (sizeof (key_type) <= 2) {
        constexpr auto max_keys = 1u << (sizeof (key_type) * 8);
        auto radix = std::array<unsigned, max_keys> {};
        for (key_type k : _keys)
          radix[k]++;
        std::partial_sum (radix.begin (), radix.end (), radix.begin ());
        for (unsigned i = 0; i < capacity; ++i)
          indices[i] = --radix[_keys[i]];
      } else {
        std::ranges::sort (indices, std::ranges::less {}, [&] (unsigned i) {
          return _keys[i];
        });
      }

      for (unsigned i = 0; i < capacity; ++i) {
        unsigned next = i;
        while (indices[next] < capacity) {
          std::ranges::swap (_keys[i], _keys[indices[next]]);
          std::ranges::swap (_values[i], _values[indices[next]]);
          next = std::exchange (indices[next], capacity);
        }
      }

      if (!std::ranges::is_sorted (_keys))
        throw "1";
    }

    constexpr void finalize () noexcept
    {
      sort ();
    }

    // ---

    [[nodiscard]] constexpr bool contains (key_type key) const noexcept
    {
      return std::ranges::binary_search (_keys, key);
    }

    [[nodiscard]] constexpr std::size_t index_of (key_type key) const noexcept
    {
      return std::ranges::lower_bound (_keys, key) - std::ranges::begin (_keys);
    }

    [[nodiscard]] constexpr value_type get (key_type key) const noexcept
    {
      return _values[index_of (key)];
    }
  };
} // namespace ml::internal::base64