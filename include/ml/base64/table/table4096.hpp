#pragma once
#include <ml/base64/table/data_4096_dense.hpp>
#include <ml/base64/table/data_4096_sorted.hpp>
#include <ml/base64/table/table64.hpp>
#include <bit>

namespace ml::internal::base64
{
  struct Sorted4096 : Table
  {
    static constexpr auto charset = default_charset;
    static constexpr auto dense64 = Dense64 ();

    using enc_type = dense_table<dense_table_traits<std::uint16_t, std::uint16_t> {.capacity = 64 * 64}>;
    using dec_type = sorted_array<sorted_array_traits<std::uint16_t, std::uint16_t> {.capacity = 64 * 64}>;

    static constexpr auto enc = std::bit_cast<enc_type> (enc_4096_dense);
    static constexpr auto dec = std::bit_cast<dec_type> (dec_4096_sorted);

    [[nodiscard]] constexpr std::uint32_t encode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t mask_hi = 0b00000000'00001111'11111111;
      constexpr std::uint32_t mask_lo = 0b11111111'11110000'00000000;

      std::uint32_t const hi = enc.get ((input & mask_hi));
      std::uint32_t const lo = enc.get ((input & mask_lo) >> 12);

      auto const pad1 = [] (std::uint32_t hi) {
        hi &= (~(0xFF << 24));
        hi |= ('=' << 24);
        return hi;
      };

      auto const pad2 = [pad1] (std::uint32_t hi) {
        hi &= (~(0xFFFF << 16));
        hi |= ('=' << 16) | ('=' << 24);
        return hi;
      };

      // output: |SSSSSSSS|RRRRRRRR|QQQQQQQQ|PPPPPPPP|

      switch (padding) {
        [[likely]] case Padding::ZERO : return (hi << 16) | (lo << 0);
        [[unlikely]] case Padding::ONE : return pad1 (hi << 16) | (lo << 0);
        [[unlikely]] case Padding::TWO : return pad2 (hi << 16) | (lo << 0);
      default:
        HEDLEY_UNREACHABLE ();
        return 0;
      }

      return (hi << 16) | lo;
    }

    [[nodiscard]] constexpr std::uint32_t decode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t mask_hi = 0b11111111'11111111'00000000'00000000;
      constexpr std::uint32_t mask_lo = 0b00000000'00000000'11111111'11111111;

      std::uint32_t const hi_index = ((input & mask_hi) >> 16);
      std::uint32_t const lo_index = ((input & mask_lo) >> 0);

      std::uint32_t const hi = dec.get (hi_index);
      std::uint32_t lo;

      if (padding == Padding::ZERO) {
        lo = dec.get (lo_index);
      } else {
        lo = std::uint32_t (dense64.dec.get (lo_index >> 8)) << 6;
      }

      // patched       : |PPPPPPQQ|QQQQRRRR|RRSSSSSS|00000000|
      // before reverse: |AAAAAAAA|BBBBBBBB|CCCCCCCC|00000000|
      // after reverse : |00000000|CCCCCCCC|BBBBBBBB|AAAAAAAA|

      return reverse_endian ((hi << 20) | (lo << 8));
    }
  };

  struct Dense4096 : Table
  {
    static constexpr auto charset = default_charset;
    static constexpr auto dense64 = Dense64 ();

    using enc_type = dense_table<dense_table_traits<std::uint16_t, std::uint16_t> {.capacity = 64 * 64}>;
    using dec_type = dense_table<dense_table_traits<std::uint16_t, std::uint16_t> {.capacity = 128 * 256}>;

    static constexpr auto enc = std::bit_cast<enc_type> (enc_4096_dense);
    static constexpr auto dec = std::bit_cast<dec_type> (dec_4096_dense);

    [[nodiscard]] constexpr std::uint32_t encode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t mask_hi = 0b00000000'00001111'11111111;
      constexpr std::uint32_t mask_lo = 0b11111111'11110000'00000000;

      std::uint32_t const hi = enc.get ((input & mask_hi));
      std::uint32_t const lo = enc.get ((input & mask_lo) >> 12);

      auto const pad1 = [] (std::uint32_t hi) {
        hi &= (~(0xFF << 24));
        hi |= ('=' << 24);
        return hi;
      };

      auto const pad2 = [pad1] (std::uint32_t hi) {
        hi &= (~(0xFFFF << 16));
        hi |= ('=' << 16) | ('=' << 24);
        return hi;
      };

      // output: |SSSSSSSS|RRRRRRRR|QQQQQQQQ|PPPPPPPP|

      switch (padding) {
        [[likely]] case Padding::ZERO : return (hi << 16) | (lo << 0);
        [[unlikely]] case Padding::ONE : return pad1 (hi << 16) | (lo << 0);
        [[unlikely]] case Padding::TWO : return pad2 (hi << 16) | (lo << 0);
      default:
        HEDLEY_UNREACHABLE ();
        return 0;
      }

      return (hi << 16) | lo;
    }
    [[nodiscard]] constexpr std::uint32_t decode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t mask_hi = 0b11111111'11111111'00000000'00000000;
      constexpr std::uint32_t mask_lo = 0b00000000'00000000'11111111'11111111;

      std::uint32_t const hi_index = ((input & mask_hi) >> 16);
      std::uint32_t const lo_index = ((input & mask_lo) >> 0);

      std::uint32_t const hi = dec.get (hi_index);
      std::uint32_t lo;

      if (padding == Padding::ZERO) {
        lo = dec.get (lo_index);
      } else {
        lo = std::uint32_t (dense64.dec.get (lo_index >> 8)) << 6;
      }

      // patched       : |PPPPPPQQ|QQQQRRRR|RRSSSSSS|00000000|
      // before reverse: |AAAAAAAA|BBBBBBBB|CCCCCCCC|00000000|
      // after reverse : |00000000|CCCCCCCC|BBBBBBBB|AAAAAAAA|

      return reverse_endian ((hi << 20) | (lo << 8));
    }
  };
} // namespace ml::internal::base64