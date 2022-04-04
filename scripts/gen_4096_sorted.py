from binascii import hexlify

'''
    static constexpr Dictionary<std::uint16_t, std::uint16_t> auto dec = [] () {
      auto table = DecodeDict ();
      for (unsigned i = 0; i < 64 * 64; ++i)
        table.insert (reverse_endian (enc.get (i)), i);
      table.finalize ();
      return table;
    }();
'''


def reverse_endian(num) -> int:
    a = (num & (0xFF << 8)) >> 8
    b = (num & (0xFF << 0)) >> 0
    return (a << 0) | (b << 8)


charset = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
mask_hi = 0b00001111_11000000
mask_lo = 0b00000000_00111111
enc = [0] * (64*64)

dec_keys = [0] * (64*64)
dec_values = [0] * (64*64)

for i in range(64 * 64):
    hi = ord(charset[(i & mask_hi) >> 6])
    lo = ord(charset[(i & mask_lo) >> 0])
    enc[i] = (lo << 8) | hi

for i in range(64 * 64):
    key = reverse_endian(enc[i])
    dec_keys[i] = key
    dec_values[i] = i


indices = list(range(0, 64*64))
indices.sort(key=lambda i: dec_keys[i])

sorted_dec_keys = [dec_keys[i] for i in indices]
sorted_dec_values = [dec_values[i] for i in indices]

with open('4096_dec_sorted.txt', 'w') as f:
    result = '{'
    for i in range(len(sorted_dec_keys)):
        x = sorted_dec_keys[i]
        result += '0x' + str(hexlify(x.to_bytes(2, 'big')), 'utf-8')
        if i != len(sorted_dec_keys)-1:
            result += ','
    result += ', '
    for i in range(len(sorted_dec_values)):
        x = sorted_dec_values[i]
        result += '0x' + str(hexlify(x.to_bytes(2, 'big')), 'utf-8')
        if i != len(sorted_dec_values)-1:
            result += ','
    result += '}'
    f.write(result)
