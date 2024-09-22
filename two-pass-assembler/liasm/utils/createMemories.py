import os


def create_memory_files():
    addresses = 65536  # Number of 16-bit addresses
    zero_value = '0' * 16  # 16-bit value initialized to 0

    # Get the directory of the script
    script_directory = os.path.dirname(os.path.abspath(__file__))
    # Define the path one level above the script's directory
    parent_directory = os.path.join(script_directory, '..')

    ram_path = os.path.join(parent_directory, 'MEMdata.txt')
    rom_path = os.path.join(parent_directory, 'MEMtext.txt')

    try:
        # Create MEMdata.txt and write zeros
        with open(ram_path, 'w') as ram_file:
            for _ in range(addresses):
                ram_file.write(zero_value + '\n')

        # Create MEMtext.txt and write zeros
        with open(rom_path, 'w') as rom_file:
            for _ in range(addresses):
                rom_file.write(zero_value + '\n')
        print(f"Files created successfully in {parent_directory}")
    except Exception as e:
        print(f"Error creating files: {e}")


create_memory_files()
