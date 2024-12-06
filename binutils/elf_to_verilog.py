import sys

def convert_hex_to_prg_mem(input_data):
    lines = input_data.splitlines()
    instructions = []

    print("Processing input data...")  # Debug log

    # Parse the input hex data
    for line_number, line in enumerate(lines):
        # Skip empty lines
        if not line.strip():
            print(f"Skipping empty line {line_number + 1}")  # Debug log
            continue

        # Extract the valid hex bytes (ignore the first column and trailing characters)
        try:
            _, hex_data = line.split(':', 1)  # Split at the colon to separate the address
        except ValueError:
            print(f"Malformed line {line_number + 1}: {line}")  # Debug log
            continue

        # Keep only valid hex bytes and handle lines with fewer than six bytes
        hex_data = " ".join(hex_data.split()[:6])  # Limit to six bytes max
        bytes_list = hex_data.split()

        # Combine bytes into 2-byte instructions
        for i in range(0, len(bytes_list), 2):
            if i + 1 < len(bytes_list):  # Ensure we have a pair of bytes to process
                # Swap the bytes and form a 16-bit word
                swapped_instruction = bytes_list[i+1] + bytes_list[i]
                instructions.append(swapped_instruction)

    if not instructions:
        print("No valid instructions found in input.")
        return ""

    # Generate the output in the specified format
    output_lines = []
    for index, instruction in enumerate(instructions):
        output_lines.append(f"prg_mem[{index}] = 16'b{instruction};")

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
    output = convert_hex_to_prg_mem(input_data)

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
