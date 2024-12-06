import sys

def convert_bin_to_data_mem(input_data):
    lines = input_data.splitlines()
    data_memory = []

    print("Processing input data...")  # Debug log

    # Parse the input binary data
    for line_number, line in enumerate(lines):
        # Skip empty lines
        if not line.strip():
            print(f"Skipping empty line {line_number + 1}")  # Debug log
            continue

        # Extract the valid binary data (ignore the first column and trailing characters)
        try:
            _, bin_data = line.split(':', 1)  # Split at the colon to separate the address
        except ValueError:
            print(f"Malformed line {line_number + 1}: {line}")  # Debug log
            continue

        # Remove any non-binary characters (e.g., spaces, comments)
        bin_data = "".join(bin_data.split())  # Remove all spaces

        # Ensure we only consider valid 8-bit binary strings
        for i in range(0, len(bin_data), 8):
            byte = bin_data[i:i+8]
            if len(byte) == 8:  # Only take valid 8-bit chunks
                data_memory.append(byte)

    if not data_memory:
        print("No valid data found in input.")
        return ""

    # Generate the output in the specified format
    output_lines = []
    for index, data_byte in enumerate(data_memory):
        output_lines.append(f"data_mem[{index}] = 8'b{data_byte};")

    return "\n".join(output_lines)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <input_file> [output_file]")
        sys.exit(1)

    # Read input data from file
    input_file = sys.argv[1]
    try:
        with open(input_file, 'r') as f:
            input_data = f.read()
    except FileNotFoundError:
        print(f"Error: Input file '{input_file}' not found.")
        sys.exit(1)

    # Process the input data
    output = convert_bin_to_data_mem(input_data)

    if not output.strip():
        print("No output generated. Check your input data.")
        sys.exit(0)

    # Write to output file or print to console
    if len(sys.argv) > 2:
        output_file = sys.argv[2]
        try:
            with open(output_file, 'w') as f:
                f.write(output)
            print(f"Output written to '{output_file}'.")
        except IOError:
            print(f"Error: Unable to write to file '{output_file}'.")
            sys.exit(1)
    else:
        print("Generated Output:")
        print(output)
