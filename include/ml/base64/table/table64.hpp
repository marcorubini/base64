#pragma once
#include <ml/base64/charset/charset.hpp>
#include <ml/base64/container/sorted_array.hpp>
#include <ml/base64/table/hedley.hpp>
#include <ml/base64/table/table.hpp>

namespace ml::internal::base64
{
  struct Dense64 : Table
  {
    static constexpr auto charset = default_charset;

    using enc_type = dense_table<dense_table_traits<std::uint8_t, std::uint8_t> {.capacity = 64}>;
    using dec_type = dense_table<dense_table_traits<std::uint8_t, std::uint8_t> {.capacity = 4096}>;

    static constexpr enc_type enc = [] () {
      auto table = enc_type ();
      for (unsigned i = 0; i < charset.size (); ++i)
        table.insert (i, charset[i]);
      table.finalize ();
      return table;
    }();

    static constexpr dec_type dec = [] () {
      auto table = dec_type ();
      for (unsigned i = 0; i < charset.size (); ++i)
        table.insert (enc.get (i), i);
      table.finalize ();
      return table;
    }();

    [[nodiscard]] constexpr std::uint32_t encode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t maskP = 0b00000000'11111100'00000000'00000000;
      constexpr std::uint32_t maskQ = 0b00000000'00000011'11110000'00000000;
      constexpr std::uint32_t maskR = 0b00000000'00000000'00001111'11000000;
      constexpr std::uint32_t maskS = 0b00000000'00000000'00000000'00111111;

      std::uint32_t const Pi = (input & maskP) >> 18;
      std::uint32_t const Qi = (input & maskQ) >> 12;
      std::uint32_t const Ri = (input & maskR) >> 6;
      std::uint32_t const Si = (input & maskS) >> 0;

      std::uint32_t const P = enc.get (Pi);
      std::uint32_t const Q = enc.get (Qi);
      std::uint32_t const R = enc.get (Ri);
      std::uint32_t const S = enc.get (Si);

      switch (padding) {
        [[likely]] case Padding::ZERO : return (S << 24) | (R << 16) | (Q << 8) | (P << 0);
        [[unlikely]] case Padding::ONE : return ('=' << 24) | (R << 16) | (Q << 8) | (P << 0);
        [[unlikely]] case Padding::TWO : return ('=' << 24) | ('=' << 16) | (Q << 8) | (P << 0);
      default:
        HEDLEY_UNREACHABLE ();
        return 0;
      }
    }

    [[nodiscard]] constexpr std::uint32_t decode (std::uint32_t input, [[maybe_unused]] Padding padding) const noexcept override
    {
      constexpr std::uint32_t maskP = 0b11111111'00000000'00000000'00000000;
      constexpr std::uint32_t maskQ = 0b00000000'11111111'00000000'00000000;
      constexpr std::uint32_t maskR = 0b00000000'00000000'11111111'00000000;
      constexpr std::uint32_t maskS = 0b00000000'00000000'00000000'11111111;

      std::uint32_t const P = ((input & maskP) >> 24);
      std::uint32_t const Q = ((input & maskQ) >> 16);
      std::uint32_t const R = ((input & maskR) >> 8);
      std::uint32_t const S = ((input & maskS) >> 0);

      std::uint32_t const a = dec.get (P);
      std::uint32_t const b = dec.get (Q);
      std::uint32_t const c = dec.get (R);
      std::uint32_t const d = dec.get (S);

      // output : |aaaaaabb|bbbbcccc|ccdddddd|00000000|
      // reverse: |00000000|CCCCCCCC|BBBBBBBB|AAAAAAAA|
      return reverse_endian ((a << 26) | (b << 20) | (c << 14) | (d << 8));
    }
  };

  struct Sorted64 : Table
  {
    static constexpr auto charset = default_charset;

    using enc_type = dense_table<dense_table_traits<std::uint8_t, std::uint8_t> {.capacity = 64}>;
    using dec_type = sorted_array<sorted_array_traits<std::uint8_t, std::uint8_t> {.capacity = 64}>;

    static constexpr enc_type enc = [] () {
      auto table = enc_type ();
      for (unsigned i = 0; i < charset.size (); ++i)
        table.insert (i, charset[i]);
      table.finalize ();
      return table;
    }();

    static constexpr dec_type dec = [] () {
      auto table = dec_type ();
      for (unsigned i = 0; i < charset.size (); ++i) {
        table._keys[i] = enc.get (i);
        table._values[i] = i;
      }
      table.finalize ();
      return table;
    }();

    [[nodiscard]] constexpr std::uint32_t encode (std::uint32_t input, Padding padding) const noexcept override
    {
      constexpr std::uint32_t maskP = 0b00000000'11111100'00000000'00000000;
      constexpr std::uint32_t maskQ = 0b00000000'00000011'11110000'00000000;
      constexpr std::uint32_t maskR = 0b00000000'00000000'00001111'11000000;
      constexpr std::uint32_t maskS = 0b00000000'00000000'00000000'00111111;

      std::uint32_t const Pi = (input & maskP) >> 18;
      std::uint32_t const Qi = (input & maskQ) >> 12;
      std::uint32_t const Ri = (input & maskR) >> 6;
      std::uint32_t const Si = (input & maskS) >> 0;

      std::uint32_t const P = enc.get (Pi);
      std::uint32_t const Q = enc.get (Qi);
      std::uint32_t const R = enc.get (Ri);
      std::uint32_t const S = enc.get (Si);

      switch (padding) {
        [[likely]] case Padding::ZERO : return (S << 24) | (R << 16) | (Q << 8) | (P << 0);
        [[unlikely]] case Padding::ONE : return ('=' << 24) | (R << 16) | (Q << 8) | (P << 0);
        [[unlikely]] case Padding::TWO : return ('=' << 24) | ('=' << 16) | (Q << 8) | (P << 0);
      default:
        HEDLEY_UNREACHABLE ();
        return 0;
      }
    }

    [[nodiscard]] constexpr std::uint32_t decode (std::uint32_t input, [[maybe_unused]] Padding padding) const noexcept override
    {
      constexpr std::uint32_t maskP = 0b11111111'00000000'00000000'00000000;
      constexpr std::uint32_t maskQ = 0b00000000'11111111'00000000'00000000;
      constexpr std::uint32_t maskR = 0b00000000'00000000'11111111'00000000;
      constexpr std::uint32_t maskS = 0b00000000'00000000'00000000'11111111;

      std::uint32_t const P = ((input & maskP) >> 24);
      std::uint32_t const Q = ((input & maskQ) >> 16);
      std::uint32_t const R = ((input & maskR) >> 8);
      std::uint32_t const S = ((input & maskS) >> 0);

      std::uint32_t const a = dec.get (P);
      std::uint32_t const b = dec.get (Q);
      std::uint32_t const c = dec.get (R);
      std::uint32_t const d = dec.get (S);

      // output : |aaaaaabb|bbbbcccc|ccdddddd|00000000|
      // reverse: |00000000|CCCCCCCC|BBBBBBBB|AAAAAAAA|
      return reverse_endian ((a << 26) | (b << 20) | (c << 14) | (d << 8));
    }
  };
} // namespace ml::internal::base64