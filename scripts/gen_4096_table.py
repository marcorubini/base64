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
dec = [0] * (128*256)

for i in range(64 * 64):
    hi = ord(charset[(i & mask_hi) >> 6])
    lo = ord(charset[(i & mask_lo) >> 0])
    enc[i] = (lo << 8) | hi

for i in range(64 * 64):
    key = reverse_endian(enc[i])
    dec[key] = i


with open('4096_enc_table.txt', 'w') as f:
    result = '{'
    for i in range(len(enc)):
        x = enc[i]
        result += '0x' + str(hexlify(x.to_bytes(2, 'big')), 'utf-8')
        if i != len(enc)-1:
            result += ','
    result += '}'
    f.write(result)

with open('4096_dec_table.txt', 'w') as f:
    result = '[](){\n'
    result += 'auto result = std::array<std::uint16_t, {}>();\n'.format(128*256)
    for i in range(len(dec)):
        if dec[i] != 0:
            result += 'result[' + str(i) + '] = ' + '0x' + str(hexlify(dec[i].to_bytes(2, 'big')), 'utf-8') + ';\n'
    result += 'return result;'
    result += '}();'
    f.write(result)
