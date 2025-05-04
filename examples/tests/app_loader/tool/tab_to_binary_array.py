#!/usr/bin/env python3

###################################
# Author: Viswajith Govinda Rajan
# Date:5/3/2025
# This script takes a .tab file and
# generates a c-binary array for 
# cortex-m4 architecture boards.
###################################

import tarfile
import os
import sys

def extract_app_name_from_filename(path):
    base_name = os.path.basename(path) 
    app_name, _ = os.path.splitext(base_name)  
    return app_name

def read_tbf_header(tbf):

    header = tbf[:16]
    version = int.from_bytes(header[0:2], "little")
    header_len = int.from_bytes(header[2:4], "little")
    total_size = int.from_bytes(header[4:8], "little")
    flags = int.from_bytes(header[8:12], "little")

    print(f"Version: {version}")
    print(f"Header Length: {header_len} bytes")
    print(f"Total App Size: {total_size} bytes")
    print(f"Flags: {hex(flags)}")

    return total_size
    

def extract_cortex_m4_tbf(tab_path):
    """Extracts the cortex-m4.tbf binary from a .tab archive."""
    try:
        with tarfile.open(tab_path, "r") as tar:
            if "cortex-m4.tbf" in tar.getnames():
                with tar.extractfile("cortex-m4.tbf") as tbf_file:
                    tbf_data = tbf_file.read()

                return tbf_data
            else:
                print("Error: cortex-m4.tbf not found in the .tab file.")
    except Exception as e:
        print(f"Extraction failed: {e}")

def binary_to_array(tbf):
    hex_array = [hex(byte) for byte in tbf]
    return hex_array

def print_hex_array(hex_array):
    print("Hexadecimal Array:")
    count = 0
    print("{")
    for hex_byte in hex_array:
        print(f"0x{hex_byte[2:].zfill(2)}", end=", ")  # Add prefix '0x' and leading zero padding
        count += 1
        if count % 14 == 0:
            print()  # Add a new line after every 14 elements
    if count % 14 != 0:
        print()  # Add a new line if the last line is not complete
    print("}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 tab_to_binary_array.py <path_to_tab_file>")
        sys.exit(1)

    file_path = sys.argv[1]

    if not os.path.isfile(file_path):
        print(f"Error: File '{file_path}' not found.")
        sys.exit(1)

    tbf = extract_cortex_m4_tbf(file_path)
    total_size = read_tbf_header(tbf)
    app_name = extract_app_name_from_filename(file_path)
    hex_array = binary_to_array(tbf)

print_hex_array(hex_array)
