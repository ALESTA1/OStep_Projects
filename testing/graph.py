import matplotlib.pyplot as plt

# Data from the test results
data = {
    100: {
        100: 0.5444, 300: 0.4602, 500: 0.7409, 700: 0.7615, 900: 0.7880,
        1100: 0.8094, 1300: 0.6148, 1500: 0.8012, 1700: 0.6688, 1900: 0.6517
    },
    1000: {
        100: 0.3805, 300: 0.5654, 500: 0.6819, 700: 0.7049, 900: 1.4205,
        1100: 0.8256, 1300: 0.8262, 1500: 0.8284, 1700: 0.7367, 1900: 0.8166
    },
    5000: {
        100: 0.2555, 300: 0.4111, 500: 0.6357, 700: 0.7557, 900: 0.1927,
        1100: 0.1183, 1300: 0.0941, 1500: 0.0964, 1700: 0.1555, 1900: 0.0980
    }
}

# Create a graph for each page count
for num_files, ratios in data.items():
    plt.figure(figsize=(8, 5))

    lengths = list(ratios.keys())
    pzip_wzip_ratios = list(ratios.values())

    # Plot for each specific page count
    plt.plot(lengths, pzip_wzip_ratios, marker='o', label=f'{num_files} files')

    # Set labels, title, and grid
    plt.xlabel('Content Length (number of lines per file)')
    plt.ylabel('pzipTime/wzipTime Ratio')
    plt.title(f'pzipTime/wzipTime Ratio vs Content Length for {num_files} Files')
    plt.grid(True)

    # Save the plot as a file
    plt.savefig(f'pzip_wzip_ratio_{num_files}_files.png')

    print(f"Plot for {num_files} files saved as 'pzip_wzip_ratio_{num_files}_files.png'.")
