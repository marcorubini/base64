#pragma once
#include <ml/base64/container/dense_table.hpp>

namespace ml::internal::base64
{
  constexpr inline struct
  {
    template<std::same_as<std::uint64_t> T>
    [[nodiscard]] constexpr T operator() (T bytes) const noexcept
    {
      std::uint64_t const a = (bytes & (0xFFull << 56)) >> 56;
      std::uint64_t const b = (bytes & (0xFFull << 48)) >> 48;
      std::uint64_t const c = (bytes & (0xFFull << 40)) >> 40;
      std::uint64_t const d = (bytes & (0xFFull << 32)) >> 32;
      std::uint64_t const e = (bytes & (0xFFull << 24)) >> 24;
      std::uint64_t const f = (bytes & (0xFFull << 16)) >> 16;
      std::uint64_t const g = (bytes & (0xFFull << 8)) >> 8;
      std::uint64_t const h = (bytes & (0xFFull << 0)) >> 0;
      return ((a << 0) | (b << 8) | (c << 16) | (d << 24) | (e << 32) | (f << 40) | (g << 48) | (h << 56));
    }

    template<std::same_as<std::uint32_t> T>
    [[nodiscard]] constexpr T operator() (T bytes) const noexcept
    {
      std::uint32_t const a = (bytes & (0xFF << 24)) >> 24;
      std::uint32_t const b = (bytes & (0xFF << 16)) >> 16;
      std::uint32_t const c = (bytes & (0xFF << 8)) >> 8;
      std::uint32_t const d = (bytes & (0xFF << 0)) >> 0;
      return (a << 0) | (b << 8) | (c << 16) | (d << 24);
    }

    template<std::same_as<std::uint16_t> T>
    [[nodiscard]] constexpr T operator() (T bytes) const noexcept
    {
      std::uint16_t const a = (bytes & (0xFF << 8)) >> 8;
      std::uint16_t const b = (bytes & (0xFF << 0)) >> 0;
      return (a << 0) | (b << 8);
    }

    template<std::same_as<std::uint8_t> T>
    [[nodiscard]] constexpr T operator() (T bytes) const noexcept
    {
      return bytes;
    }
  } reverse_endian {};

  enum class Padding
  {
    ZERO,
    ONE,
    TWO
  };

  struct Table
  {
    [[nodiscard]] virtual constexpr std::uint32_t encode (std::uint32_t input, Padding padding) const noexcept = 0;
    [[nodiscard]] virtual constexpr std::uint32_t decode (std::uint32_t input, Padding padding) const noexcept = 0;
  };

} // namespace ml::internal::base64