import re

app_binaries_h_path = "../app_loader/app_binaries.h" 
app_loader_c_path = "../app_loader/main.c"  

binaries = []

# Read the header file to extract available binary names
with open(app_binaries_h_path, "r") as header_file:
    content = header_file.readlines()
    for line in content:
        match = re.match(r"extern\s+uint8_t\s+(\w+)\[\d+\];", line)
        if match:
            binaries.append(match.group(1))

def update_app_loader_c_file(c_file_path):
    # Read existing list of binaries in app_loader/main.c
    with open(c_file_path, "r") as c_file:
        c_file_content = c_file.readlines()

    binaries_match = re.search(r'uint8_t\* binaries\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))
    binary_sizes_match = re.search(r'size_t binary_sizes\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))
    binary_names_match = re.search(r'const char\* binary_names\[\]\s*=\s*\{([^\}]+)\};', ''.join(c_file_content))

    existing_binaries = binaries_match.group(1).split(', ') if binaries_match else []
    existing_binary_sizes = binary_sizes_match.group(1).split(', ') if binary_sizes_match else []
    existing_binary_names = binary_names_match.group(1).split(', ') if binary_names_match else []

    # Identify new binaries not present in app_loader/main.c
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
        
        # Generate the updated arrays
        updated_binaries = f"uint8_t* binaries[] = {{{new_binaries}}};\n"
        updated_binary_sizes = f"size_t binary_sizes[] = {{{new_binary_sizes}}};\n"
        updated_binary_names = f"const char* binary_names[] = {{{new_binary_names}}};\n"
        
        # Replace the old arrays with the updated ones in the C file content
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

        # Ensure missing arrays are added at the end if they were not found
        if not binaries_updated:
            updated_content.append("\n" + updated_binaries)
        if not sizes_updated:
            updated_content.append(updated_binary_sizes)
        if not names_updated:
            updated_content.append(updated_binary_names)

        # Write the updated content back to the C file
        with open(c_file_path, "w") as c_file:
            c_file.writelines(updated_content)

        print(f"Arrays in {c_file_path} updated with new binaries.")
    else:
        print(f"No new binaries found in {c_file_path}. Arrays are up-to-date.")


update_app_loader_c_file(app_loader_c_path)
