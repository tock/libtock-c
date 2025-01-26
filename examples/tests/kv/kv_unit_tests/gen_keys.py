#!/usr/bin/env python3


import binascii
import siphash
import string


key = "HKANfdlksiqiovcHIFoihASIU"


NUM_REGIONS = 32


keys = {}


for r in range(0, NUM_REGIONS):
    for c1 in string.ascii_letters + string.digits:
        for c2 in string.ascii_letters + string.digits:
            test_key = key + c1 + c2

            key_buffer = test_key.encode("utf-8")
            h = siphash.SipHash_2_4(bytearray(16), key_buffer).digest()

            lower = (h[6] << 8) | h[7]

            if lower % NUM_REGIONS == r:
                keys[r] = test_key
                break

        if r in keys:
            break

    else:
        print("UNABLE TO FIND KEY FOR REGION {}".format(r))

# print(binascii.hexlify(h).decode("utf-8"))


print(keys)

for k, v in keys.items():
    print('"{}",'.format(v))
