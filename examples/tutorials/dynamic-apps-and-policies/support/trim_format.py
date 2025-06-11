#!/usr/bin/env python3
import sys

def trim_trailing_zeroes(b):
    for i in range(len(b) - 1, 0, -1):
        if b[i] != 0x0:
            return b[:i+1]
    return b

def format_data_strings(b):
    def chunked(source, size):
        for i in range(0, len(source), size):
            yield source[i : i+size]

    rows = []
    for chunk in chunked(b, 10):
        row = ["0x{:02x}".format(c) for c in chunk]
        rows.append(", ".join(row))
    return "{{\n  {}\n}}".format(",\n  ".join(rows))

def main(input_path, output_path, array_name):
    data = open(input_path, "rb").read()
    trimmed = trim_trailing_zeroes(data)
    c_data = format_data_strings(trimmed)
    with open(output_path, "w") as f:
        f.write(f"const unsigned char {array_name}[] = {c_data};\n")
        f.write(f"const size_t {array_name}_size = {len(data)};\n")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        sys.exit(1)
    main(sys.argv[1], sys.argv[2], sys.argv[3])