import subprocess
import time
import os
import shutil
import filecmp
import matplotlib.pyplot as plt

# Paths to pzip and wzip binaries
pzip_path = "../Concurrency/pzip/pzip"
wzip_path = "../Utils/winzip/wzip"
wunzip_path = "../Utils/winzip/wunzip"

# Number of files to test with
page_counts = [100, 1000, 5000]
line_lengths = range(100, 2001, 200)  # Vary content lengths from 100 to 2000

# Output file names
output_dir = "sample_texts"
pzip_output = "pzip_output.rle"
wzip_output = "wzip_output.rle"
pzip_decompressed = "pzip_decompressed.txt"
wzip_decompressed = "wzip_decompressed.txt"

# Ensure output directory exists
os.makedirs(output_dir, exist_ok=True)

# Sample content generator
def generate_content(file_number, line_length):
    return f"This is aaaaaaaaaaaaaabbbbbbbbbbbbbbb {file_number}.\n" * line_length

# Function to create multiple sample text files
def create_sample_files(num_files, line_length):
    for i in range(1, num_files + 1):
        file_path = os.path.join(output_dir, f"sample_file_{i}.txt")
        with open(file_path, "w") as file:
            file.write(generate_content(i, line_length))

# Function to run a command and measure execution time
def run_command(command, output_file):
    start_time = time.time()
    with open(output_file, 'w') as f:
        subprocess.run(command, stdout=f, stderr=subprocess.PIPE)
    return time.time() - start_time

# Function to measure pzip and wzip times and return the ratio
def measure_performance(num_files, line_length):
    # Create the sample files with the given content length
    create_sample_files(num_files, line_length)

    # Collect all the sample files
    input_files = [os.path.join(output_dir, f"sample_file_{i}.txt") for i in range(1, num_files + 1)]

    # Benchmark pzip
    pzip_command = [pzip_path] + input_files
    pzip_time = run_command(pzip_command, pzip_output)

    # Benchmark wzip
    wzip_command = [wzip_path] + input_files
    wzip_time = run_command(wzip_command, wzip_output)

    return pzip_time / wzip_time

# Plotting the results
def plot_results(results):
    for num_files, data in results.items():
        line_lengths = list(data.keys())
        ratios = [data[length] for length in line_lengths]
        plt.plot(line_lengths, ratios, label=f'{num_files} pages')

    plt.xlabel('Content Length (number of lines per page)')
    plt.ylabel('pzipTime/wzipTime')
    plt.title('pzipTime/wzipTime vs Number of Pages and Content Length')
    plt.legend()
    plt.grid(True)
    plt.show()

# Main function to run tests and plot the results
def main():
    results = {page_count: {} for page_count in page_counts}

    # Measure performance for each page count and line length combination
    for num_files in page_counts:
        print(f"Testing {num_files} files:")
        for line_length in line_lengths:
            print(f"  - Content length: {line_length}")
            ratio = measure_performance(num_files, line_length)
            results[num_files][line_length] = ratio
            print(f"    pzipTime/wzipTime ratio: {ratio:.4f}")

    # Plot the results
    plot_results(results)

if __name__ == "__main__":
    main()
