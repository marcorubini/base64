#pragma once
#include <algorithm>
#include <bit>
#include <concepts>
#include <functional>
#include <span>
#include <string_view>

#include <ml/base64/container/sorted_array.hpp>
#include <ml/base64/table/table4096.hpp>
#include <ml/base64/table/table64.hpp>

namespace ml::internal::base64
{
  struct to_byte
  {
    template<class T>
      requires (std::is_trivial_v<T> && sizeof (T) == 1)
    [[nodiscard]] constexpr std::byte operator() (T const& arg) const noexcept
    {
      return std::bit_cast<std::byte> (arg);
    }
  };

  constexpr inline auto table = [] () {
    return Dense4096 ();
    // return Sorted4096 ();
    // return Dense64 ();
    // return Sorted64 ();
  }();

  // ---

  template<unsigned Expected>
  constexpr inline auto read_n = [] (std::uint32_t& result, auto source, auto good) {
    for (unsigned i = 0; i < Expected; ++i) {
      if (!good ())
        return i;
      result |= static_cast<std::uint32_t> (source ()) << ((Expected - i - 1) * 8);
    }
    return Expected;
  };

  template<unsigned N>
  constexpr inline auto write_n = [] (std::uint32_t value, auto sink) {
    for (unsigned i = 0; i < N; ++i) {
      sink (static_cast<std::byte> (value & 0xFF));
      value >>= 8;
    }
  };

  constexpr inline auto encode_ = [] (auto source, auto good, auto sink) {
    std::uint32_t curr = 0;
    auto read = read_n<3> (curr, source, good);
    while (read == 3)
      [[likely]]
      {
        write_n<4> (table.encode (curr, Padding::ZERO), sink);
        curr = 0;
        read = read_n<3> (curr, source, good);
      }

    if (read != 0)
      write_n<4> (table.encode (curr, read == 1 ? Padding::TWO : Padding::ONE), sink);
  };

  constexpr inline auto decode_ = [] (auto source, auto good, auto sink) {
    std::uint32_t curr = 0;
    auto read = read_n<4> (curr, source, good);
    while (read == 4)
      [[likely]]
      {
        write_n<3> (table.decode (curr, Padding::ZERO), sink);
        curr = 0;
        read = read_n<4> (curr, source, good);
      }

    switch (read) {
      [[likely]] case 0 : break;

    case 1:
      [[fallthrough]];
    case 2: {
      auto decoded = table.decode (curr, Padding::TWO);
      write_n<1> (decoded, sink);
      break;
    }
    case 3: {
      auto decoded = table.decode (curr, Padding::ONE);
      write_n<2> (decoded, sink);
    }
    default:
      HEDLEY_UNREACHABLE ();
      break;
    }
  };

  // ---

  template<class It>
  concept ByteIterator = std::output_iterator<It, std::byte> //
    || std::output_iterator<It, std::uint8_t>                //
    || std::output_iterator<It, unsigned char>               //
    || std::output_iterator<It, char>;

  constexpr inline auto write_byte = []<ByteIterator It> (It& out, std::byte byte) {
    if constexpr (std::output_iterator<It, std::byte>) {
      *out++ = byte;
    } else if constexpr (std::output_iterator<It, std::uint8_t>) {
      *out++ = static_cast<std::uint8_t> (byte);
    } else if constexpr (std::output_iterator<It, unsigned char>) {
      *out++ = static_cast<unsigned char> (byte);
    } else {
      *out++ = static_cast<char> (byte);
    }
  };

  constexpr inline struct encode_bytes_fn
  {
    template<std::input_iterator I1,
      std::sentinel_for<I1> I2,                                   //
      std::indirectly_regular_unary_invocable<I1> Proj = to_byte, //
      ByteIterator O>
    constexpr auto operator() (I1 first, I2 last, O out, Proj proj = {}) const
    {
      auto const source = [&] () -> std::byte {
        return proj (*first++);
      };

      auto const good = [&] () -> bool {
        return first != last;
      };

      auto const sink = [&] (std::byte value) {
        write_byte (out, value);
      };

      encode_ (source, good, sink);
      return out;
    }

    template<std::ranges::input_range R,                                                  //
      std::indirectly_regular_unary_invocable<std::ranges::iterator_t<R>> Proj = to_byte, //
      ByteIterator O>
    constexpr auto operator() (R&& range, O out, Proj proj = {}) const
    {
      return (*this) (std::ranges::begin (range), std::ranges::end (range), std::move (out), std::move (proj));
    }
  } encode_bytes {};

  constexpr inline struct encode_bytes_n_fn
  {
    template<std::input_iterator I1,
      std::indirectly_regular_unary_invocable<I1> Proj = to_byte, //
      ByteIterator O>
    constexpr auto operator() (I1 first, std::iter_difference_t<I1> n, O out, Proj proj = {}) const
    {
      auto const source = [&] () -> std::byte {
        --n;
        return proj (*first++);
      };

      auto const good = [&] () -> bool {
        return n != 0;
      };

      auto const sink = [&] (std::byte value) {
        write_byte (out, value);
      };

      encode_ (source, good, sink);
      return out;
    }
  } encode_bytes_n {};

  struct to_char
  {
    template<class T>
      requires (std::integral<T>)
    [[nodiscard]] constexpr char operator() (T const& arg) const noexcept
    {
      return std::bit_cast<char> (arg);
    }
  };

  constexpr inline struct decode_fn
  {
    template<std::input_iterator I1,
      std::sentinel_for<I1> I2,                                   //
      std::indirectly_regular_unary_invocable<I1> Proj = to_char, //
      ByteIterator O>
    constexpr auto operator() (I1 first, I2 last, O out, Proj proj = {}) const
    {
      auto const source = [&] () -> char {
        return proj (*first++);
      };

      auto const good = [&] () -> bool {
        return first != last && proj (*first) != '=';
      };

      auto const sink = [&] (std::byte value) {
        write_byte (out, value);
      };

      decode_ (source, good, sink);
      return out;
    }

    template<std::ranges::input_range R,                                                  //
      std::indirectly_regular_unary_invocable<std::ranges::iterator_t<R>> Proj = to_char, //
      ByteIterator O>
    constexpr auto operator() (R&& range, O out, Proj proj = {}) const
    {
      return (*this) (std::ranges::begin (range), std::ranges::end (range), std::move (out), std::move (proj));
    }
  } decode {};

  constexpr inline struct decode_n_fn
  {
    template<std::input_iterator I1,
      std::indirectly_regular_unary_invocable<I1> Proj = to_char, //
      ByteIterator O>
    constexpr auto operator() (I1 first, std::iter_difference_t<I1> n, O out, Proj proj = {}) const
    {
      auto const source = [&] () -> char {
        --n;
        return proj (*first++);
      };

      auto const good = [&] () -> bool {
        return n != 0 && proj (*first) != '=';
      };

      auto const sink = [&] (std::byte value) {
        write_byte (out, value);
      };

      decode_ (source, good, sink);
      return out;
    }
  } decode_n {};

} // namespace ml::internal::base64

/*! @namespace ml::base64
 *  @brief Top level namespace
 */
namespace ml::base64
{
  /*! \addtogroup FunctionObject
   * @{
   *    @defgroup Encoding Encoding
   *    @brief    Function objects that encode ranges of bytes
   *
   *    Encoding function objects accept a range of byte-like types.
   *    A byte-like type is a trivial type of size 1.
   *
   *    You can optionally pass a projection that converts the
   *    range value type to std::byte. The default projection
   *    uses std::bit_cast.
   *
   *    Multiple overloads are available, you can use pairs of iterators
   *    or input ranges.
   *
   *    @todo Allow configuring the encoding algorithm.
   *
   *    @defgroup Decoding Decoding
   *    @brief    Function objects that encode ranges of bytes
   *
   *    Decoding function objects accept a range of integral types.
   *    Input characters are narrowed to the range [0, 255).
   *    You can optionally pass a projection that converts the range
   *    value type to 'char'. The default projection uses static_cast.
   *
   *    Multiple overloads are available, you can use pairs of iterators
   *    or input ranges or iterator + size.
   *
   *    @todo Allow configuring the decoding algorithm.
   * @}
   */

  /*! @addtogroup Decoding
   * @{
   */

  /*! @var decode
   * @hideinitializer
   * @brief Decode a range with string semantics.
   *
   * The range value type must be integral. Characters are assumed to be valid
   * Base64 and encoding stops when the range is exhausted or a padding or null terminator is found.
   *
   * Example
   * -------
   *
   *     std::string_view encoded = "TWFu";
   *     std::string result;
   *     ml::base64::decode(encoded, back_inserter(result));
   */
  constexpr inline auto decode = internal::base64::decode;

  /*! @var decode_n
   * @hideinitializer
   * @brief Like @ref decode but invocable with a start iterator and a size.
   */
  constexpr inline auto decode_n = internal::base64::decode_n;

  /*! @} */

  /*! @addtogroup Encoding
   * @{
   */

  /*! @var encode_bytes
   * @hideinitializer
   * @brief Encode a range of bytes
   *
   * @remark The input range does not have string semantics, it is not expected to be null terminated.
   * Any null terminator will be encoded.
   * This is a problem if you try to encode character arrays with string semantics, like:
   *
   *     ml::base64::encode("Man", output);
   *
   * To fix this, use std::string_view instead of character arrays.
   *
   *
   * @remark Likewise, the output is not null terminated. When the output is a character array
   * with string semantics, you need to manually add the null terminator.
   *
   * Example - Encoding a string
   * ---------------------------
   *
   *     std::string_view text = "ABC";
   *     std::string result;
   *     ml::base64::encode_bytes(text, back_inserter(result));
   *
   * Example - Iterator interface
   * ----------------------------
   *
   *     std::string_view text = "ABC";
   *     std::string result;
   *     ml::base64::encode_bytes(text.begin(), text.end(), back_inserter(result));
   *
   * Example - Encoding a stream
   * ---------------------------
   *
   *     auto in = std::ranges::istream_view<char>(std::cin);
   *     std::string result;
   *     ml::base64::encode_bytes(in, back_inserter(result));
   *
   * Example - Adding null terminator
   * --------------------------------
   *
   *     std::string_view text = "ABC";
   *     char result[100];
   *     char* last = ml::base64::encode_bytes(text, result);
   *     *last = '\0';
   */
  constexpr inline auto encode_bytes = internal::base64::encode_bytes;

  /*! @var encode_bytes_n
   * @hideinitializer
   * @brief Like @ref encode_bytes but invocable with a start iterator and a size.
   */
  constexpr inline auto encode_bytes_n = internal::base64::encode_bytes_n;

  /*! @} */

} // namespace ml::base64
