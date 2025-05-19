import tarfile
import sys
import os
import re

app_binaries_h_path = "../app_loader/app_binaries.h" 
app_binaries_c_path = "../app_loader/app_binaries.c"
app_loader_c_path = "../app_loader/main.c"  

binaries = []

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
    # extract cortex-m4.tbf binary from a .tab file. (nrf52840dk has a cortex-m4 chip)
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

# debug function
def print_hex_array(hex_array):
    print("Hexadecimal Array:")
    count = 0
    print("[")
    for hex_byte in hex_array:
        # Add prefix '0x' and leading zero padding
        print(f"0x{hex_byte[2:].zfill(2)}", end=", ")  
        count += 1
        if count % 14 == 0:
            print()  
    if count % 14 != 0:
        print()  
    print("]")

def update_app_binaries_h_file(path, name, size):
    new_line = f'extern uint8_t {name}[{size}];\n'

    with open(app_binaries_h_path, "r") as file:
        lines = file.readlines()

    count = 0
    insert_index = None

    # insert the new line before the cpp wrapper ends
    for i, line in enumerate(lines):
        if line.strip() == "#ifdef __cplusplus":
            count += 1
            if count == 2:
                insert_index = i
                break

    if insert_index is not None:
        lines.insert(insert_index, new_line) 

        # print(new_line)

        with open(path, "w") as file:
            file.writelines(lines)

def update_app_binaries_c_file(path, name, array):

    formatted_hex_array = ""
    
    for i in range(0, len(array), 14):
        formatted_hex_array += "    " + ", ".join(array[i:i+14]) + ",\n"

    new_array = f"uint8_t {name} [] = {{\n{formatted_hex_array}}};\n\n"

    with open(path, "a") as file:
        file.write(new_array)

def update_app_loader_c_file(c_file_path):

    # read the app_loader/app_binaries.h to get list of existing binaries
    with open(app_binaries_h_path, "r") as header_file:
        content = header_file.readlines()
        for line in content:
            match = re.match(r"extern\s+uint8_t\s+(\w+)\[\d+\];", line)
            if match:
                binaries.append(match.group(1))

    # read existing list of binaries in app_loader/main.c
    with open(c_file_path, "r") as c_file:
        c_file_content = c_file.readlines()

    binaries_match = re.search(r'uint8_t\* binaries\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))
    binary_sizes_match = re.search(r'size_t binary_sizes\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))
    binary_names_match = re.search(r'const char\* binary_names\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))

    existing_binaries = binaries_match.group(1).split(', ') if binaries_match else []
    existing_binary_sizes = binary_sizes_match.group(1).split(', ') if binary_sizes_match else []
    existing_binary_names = binary_names_match.group(1).split(', ') if binary_names_match else []

    # identify new binaries not present in app_loader/main.c
    existing_binary_names_cleaned = [name.strip('"') for name in existing_binary_names]
    new_values = [var for var in binaries if var not in existing_binary_names_cleaned]

     # debug
    print("existing_binaries: {}".format(existing_binaries))
    print("existing_binary_sizes: {}".format(existing_binary_sizes))
    print("existing_binary_names: {}".format(existing_binary_names))
    print("new_values: {}".format(new_values))

    if new_values:
        new_binaries = ', '.join(existing_binaries + new_values)
        new_binary_sizes = ', '.join(existing_binary_sizes + [f'sizeof({var})' for var in new_values])
        new_binary_names = ', '.join(existing_binary_names + [f'"{var}"' for var in new_values])
        
        updated_binaries = f"uint8_t* binaries[] = {{{new_binaries}}};\n"
        updated_binary_sizes = f"size_t binary_sizes[] = {{{new_binary_sizes}}};\n"
        updated_binary_names = f"const char* binary_names[] = {{{new_binary_names}}};\n"
        
        # Replace the old arrays with the updated ones in app_loader/main.c
        updated_content = []
        binaries_updated = False
        sizes_updated = False
        names_updated = False

        for line in c_file_content:
            if 'uint8_t* binaries[] =' in line:
                updated_content.append(updated_binaries)
                binaries_updated = True
            elif 'size_t binary_sizes[] =' in line:
                updated_content.append(updated_binary_sizes)
                sizes_updated = True
            elif 'const char* binary_names[] =' in line:
                updated_content.append(updated_binary_names)
                names_updated = True
            else:
                updated_content.append(line)

        if not binaries_updated:
            updated_content.append("\n" + updated_binaries)
        if not sizes_updated:
            updated_content.append(updated_binary_sizes)
        if not names_updated:
            updated_content.append(updated_binary_names)

        with open(c_file_path, "w") as c_file:
            c_file.writelines(updated_content)

        print(f"Arrays in {c_file_path} updated with new binaries.")
    else:
        print(f"No new binaries found in {c_file_path}. Arrays are up-to-date.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Missing .tab file argument")
        sys.exit(1)

    file_path = sys.argv[1]

    if not os.path.isfile(file_path):
        print(f"Error: File '{file_path}' not found.")
        sys.exit(1)

    tbf = extract_cortex_m4_tbf(file_path)
    total_size = read_tbf_header(tbf)
    app_name = extract_app_name_from_filename(file_path)
    hex_array = binary_to_array(tbf)

    print(f"App Name: {app_name}")

    update_app_binaries_h_file(app_binaries_h_path, app_name, total_size)
    update_app_binaries_c_file(app_binaries_c_path, app_name, hex_array)
    update_app_loader_c_file(app_loader_c_path)
