import os

def read_binary_file(file_name):
    parent_directory = os.path.dirname(os.getcwd())
    grandparent_directory = os.path.dirname(parent_directory)
    file_path = os.path.join(grandparent_directory, file_name)
    with open(file_path, 'rb') as file:
        binary_data = file.read()
    return binary_data

def binary_to_array(binary_data):
    hex_array = [hex(byte) for byte in binary_data]
    return hex_array

def print_hex_array(hex_array):
    print("Hexadecimal Array:")
    count = 0
    for hex_byte in hex_array:
        print(f"0x{hex_byte[2:].zfill(2)}", end=", ")  # Add prefix '0x' and leading zero padding
        count += 1
        if count % 14 == 0:
            print()  # Add a new line after every 14 elements
    if count % 14 != 0:
        print()  # Add a new line if the last line is not complete


file_name = 'examples/tests/app_loader/tock-dpl-hello/build/cortex-m4/cortex-m4.tbf'
binary_data = read_binary_file(file_name)
hex_array = binary_to_array(binary_data)
print_hex_array(hex_array)