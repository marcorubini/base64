#pragma once
#include <ml/base64/base64.hpp>
#include <doctest/doctest.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

static const char* moby_dick_plain =
  "Call me Ishmael. Some years ago--never mind how long precisely--having\n"
  "little or no money in my purse, and nothing particular to interest me on\n"
  "shore, I thought I would sail about a little and see the watery part of\n"
  "the world. It is a way I have of driving off the spleen and regulating\n"
  "the circulation. Whenever I find myself growing grim about the mouth;\n"
  "whenever it is a damp, drizzly November in my soul; whenever I find\n"
  "myself involuntarily pausing before coffin warehouses, and bringing up\n"
  "the rear of every funeral I meet; and especially whenever my hypos get\n"
  "such an upper hand of me, that it requires a strong moral principle to\n"
  "prevent me from deliberately stepping into the street, and methodically\n"
  "knocking people's hats off--then, I account it high time to get to sea\n"
  "as soon as I can. This is my substitute for pistol and ball. With a\n"
  "philosophical flourish Cato throws himself upon his sword; I quietly\n"
  "take to the ship. There is nothing surprising in this. If they but knew\n"
  "it, almost all men in their degree, some time or other, cherish very\n"
  "nearly the same feelings towards the ocean with me.\n";

static const char* moby_dick_base64 = "Q2FsbCBtZSBJc2htYWVsLiBTb21lIHllYXJzIGFnby0tbmV2ZXIgbWluZCBob3cgbG9uZ"
                                      "yBwcmVjaXNlbHktLWhhdmluZwpsaXR0bGUgb3Igbm8gbW9uZXkgaW4gbXkgcHVyc2UsIG"
                                      "FuZCBub3RoaW5nIHBhcnRpY3VsYXIgdG8gaW50ZXJlc3QgbWUgb24Kc2hvcmUsIEkgdGh"
                                      "vdWdodCBJIHdvdWxkIHNhaWwgYWJvdXQgYSBsaXR0bGUgYW5kIHNlZSB0aGUgd2F0ZXJ5"
                                      "IHBhcnQgb2YKdGhlIHdvcmxkLiBJdCBpcyBhIHdheSBJIGhhdmUgb2YgZHJpdmluZyBvZ"
                                      "mYgdGhlIHNwbGVlbiBhbmQgcmVndWxhdGluZwp0aGUgY2lyY3VsYXRpb24uIFdoZW5ldm"
                                      "VyIEkgZmluZCBteXNlbGYgZ3Jvd2luZyBncmltIGFib3V0IHRoZSBtb3V0aDsKd2hlbmV"
                                      "2ZXIgaXQgaXMgYSBkYW1wLCBkcml6emx5IE5vdmVtYmVyIGluIG15IHNvdWw7IHdoZW5l"
                                      "dmVyIEkgZmluZApteXNlbGYgaW52b2x1bnRhcmlseSBwYXVzaW5nIGJlZm9yZSBjb2Zma"
                                      "W4gd2FyZWhvdXNlcywgYW5kIGJyaW5naW5nIHVwCnRoZSByZWFyIG9mIGV2ZXJ5IGZ1bm"
                                      "VyYWwgSSBtZWV0OyBhbmQgZXNwZWNpYWxseSB3aGVuZXZlciBteSBoeXBvcyBnZXQKc3V"
                                      "jaCBhbiB1cHBlciBoYW5kIG9mIG1lLCB0aGF0IGl0IHJlcXVpcmVzIGEgc3Ryb25nIG1v"
                                      "cmFsIHByaW5jaXBsZSB0bwpwcmV2ZW50IG1lIGZyb20gZGVsaWJlcmF0ZWx5IHN0ZXBwa"
                                      "W5nIGludG8gdGhlIHN0cmVldCwgYW5kIG1ldGhvZGljYWxseQprbm9ja2luZyBwZW9wbG"
                                      "UncyBoYXRzIG9mZi0tdGhlbiwgSSBhY2NvdW50IGl0IGhpZ2ggdGltZSB0byBnZXQgdG8"
                                      "gc2VhCmFzIHNvb24gYXMgSSBjYW4uIFRoaXMgaXMgbXkgc3Vic3RpdHV0ZSBmb3IgcGlz"
                                      "dG9sIGFuZCBiYWxsLiBXaXRoIGEKcGhpbG9zb3BoaWNhbCBmbG91cmlzaCBDYXRvIHRoc"
                                      "m93cyBoaW1zZWxmIHVwb24gaGlzIHN3b3JkOyBJIHF1aWV0bHkKdGFrZSB0byB0aGUgc2"
                                      "hpcC4gVGhlcmUgaXMgbm90aGluZyBzdXJwcmlzaW5nIGluIHRoaXMuIElmIHRoZXkgYnV"
                                      "0IGtuZXcKaXQsIGFsbW9zdCBhbGwgbWVuIGluIHRoZWlyIGRlZ3JlZSwgc29tZSB0aW1l"
                                      "IG9yIG90aGVyLCBjaGVyaXNoIHZlcnkKbmVhcmx5IHRoZSBzYW1lIGZlZWxpbmdzIHRvd"
                                      "2FyZHMgdGhlIG9jZWFuIHdpdGggbWUuCg==";

TEST_CASE ("encoding")
{
  using ml::base64::decode;
  using ml::base64::encode_bytes;

  auto encode_str = [] (std::string_view text) -> std::string {
    std::string result;
    encode_bytes (text, back_inserter (result));
    return result;
  };

  auto decode_str = [] (std::string_view text) -> std::string {
    std::string result;
    decode (text, back_inserter (result));
    return result;
  };

  CHECK_EQ (encode_str ("Man"), "TWFu");
  CHECK_EQ (encode_str (moby_dick_plain), std::string (moby_dick_base64));
  CHECK_EQ (encode_str (""), "");
  CHECK_EQ (encode_str ("f"), "Zg==");
  CHECK_EQ (encode_str ("fo"), "Zm8=");
  CHECK_EQ (encode_str ("foo"), "Zm9v");
  CHECK_EQ (encode_str ("foob"), "Zm9vYg==");
  CHECK_EQ (encode_str ("fooba"), "Zm9vYmE=");
  CHECK_EQ (encode_str ("foobar"), "Zm9vYmFy");
}

TEST_CASE ("decoding")
{
  using ml::base64::decode;
  using ml::base64::encode_bytes;

  auto encode_str = [] (std::string_view text) -> std::string {
    std::string result;
    encode_bytes (text, back_inserter (result));
    return result;
  };

  auto decode_str = [] (std::string_view text) -> std::string {
    std::string result;
    decode (text, back_inserter (result));
    return result;
  };

  CHECK_EQ (decode_str (encode_str ("Man")), "Man");
  CHECK_EQ (decode_str (encode_str (moby_dick_plain)), std::string (moby_dick_plain));
  CHECK_EQ (decode_str ("bGlnaHQgdw=="), "light w");
  CHECK_EQ (decode_str ("bGlnaHQgd28="), "light wo");
  CHECK_EQ (decode_str ("bGlnaHQgd29y"), "light wor");
  CHECK_EQ (decode_str (encode_str ("")), "");
  CHECK_EQ (decode_str (encode_str ("f")), "f");
  CHECK_EQ (decode_str (encode_str ("fo")), "fo");
  CHECK_EQ (decode_str (encode_str ("foo")), "foo");
  CHECK_EQ (decode_str (encode_str ("foob")), "foob");
  CHECK_EQ (decode_str (encode_str ("fooba")), "fooba");
  CHECK_EQ (decode_str (encode_str ("foobar")), "foobar");
}

TEST_CASE ("null terminator")
{
  char result[100] {};
  std::ranges::fill (result, 'a');

  char* last = ml::base64::encode_bytes (std::string_view ("Man"), result);
  *last = '\0';

  CHECK (std::string (result) == "TWFu");
}