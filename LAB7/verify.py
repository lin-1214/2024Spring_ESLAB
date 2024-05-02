import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import lfilter, square

# def read_input_sig():
#     # Read the input signal from the file
#     buf = []
#     with open("./data/input.txt", "r") as f:
#         if (f.readlines() == []):
#             return buf
#         buf.append(f.readlines()[0])
#     return buf

# def read_output_sig(path):
#     # Write the output signal to the file
#     buf = []
#     with open(path, "r") as f:
#         if (f.readlines() == []):
#             return buf
#         buf.append(f.readlines()[0])
#     return buf

# Sample rate and test signal length
sample_rate = 48000
test_length = 320

# Input mode (-si/sq/d)
mode = sys.argv[1]

t = np.arange(test_length) / sample_rate
freq_1, freq_2, freq_3 = 0, 0, 0
input_sig = None
output_sig = None

if (mode == "-si"):
    # FRI filter for sine wave
    freq_1 = 1000
    freq_2 = 15000
    input_sig = 0.5 * np.sin(2 * np.pi * freq_1 * t) + 0.5 * np.sin(2 * np.pi * freq_2 * t)
    Lines = None
    output_sig = []

    with open("./data/sine_output.txt", "r") as f:
        Lines = f.readlines()
    for line in Lines:
        output_sig.append(float(line.replace("\n", "")))

elif (mode == "-sq"):
    # FIR filter for square wave
    freq_1 = 1000
    freq_2 = 5000
    freq_3 = 9000
    input_sig = 1/3 * (square(2 * np.pi * freq_1 * t) + square(2 * np.pi * freq_2 * t) + square(2 * np.pi * freq_3 * t))
    Lines = None
    output_sig = []

    with open("./data/sine_output.txt", "r") as f:
        Lines = f.readlines()
    for line in Lines:
        output_sig.append(float(line.replace("\n", "")))
    
elif (mode == "-d"):
    # FIR filter for real data
    input_sig = []
    output_sig = []
    Lines = None
    with open("./data/input.txt", "r") as f:
        Lines = f.readlines()
    for line in Lines:
        input_sig.append(float(line.replace("\n", "")))

    Lines = None
    with open("./data/output.txt", "r") as f:
        Lines = f.readlines()
    for line in Lines:
        output_sig.append(float(line.replace("\n", "")))

    if (input_sig == [] or output_sig == []):
        print("[-] Error: No data found in .txt file")
        sys.exit(1)


fir_coeffs = np.array(
    [
        -0.0018225230, -0.0015879294, +0.0000000000, +0.0036977508, +0.0080754303, +0.0085302217, -0.0000000000, -0.0173976984,
        -0.0341458607, -0.0333591565, +0.0000000000, +0.0676308395, +0.1522061835, +0.2229246956, +0.2504960933, +0.2229246956,
        +0.1522061835, +0.0676308395, +0.0000000000, -0.0333591565, -0.0341458607, -0.0173976984, -0.0000000000, +0.0085302217,
        +0.0080754303, +0.0036977508, +0.0000000000, -0.0015879294, -0.0018225230
    ]
)

verify_sig = lfilter(fir_coeffs, 1.0, input_sig)

# Plot the input signal
plt.figure(figsize=(10, 12))
plt.subplot(3, 1, 1)
plt.plot(t, input_sig)
plt.title("Input Signal")
plt.xlabel("Sample point")
plt.ylabel("Amplitude")
plt.grid(True)

# Plot the verify signal
plt.subplot(3, 1, 2)
plt.plot(t, verify_sig)
plt.title("Verify Signal ( made by Python )")
plt.xlabel("Sample point")
plt.ylabel("Amplitude")
plt.grid(True)

# Plot the output signal from STM32
plt.subplot(3, 1, 3)
plt.plot(t, output_sig)
plt.title("Output Signal ( from STM32 )")
plt.xlabel("Sample point")
plt.ylabel("Amplitude")
plt.grid(True)

plt.tight_layout()

if (mode == "-si"):
    plt.savefig("./plots/fir_sine_wave.png")
elif (mode == "-sq"):
    plt.savefig("./plots/fir_square_wave.png")
else:
    plt.savefig("./plots/fir_data.png")

print("[+] Verification plotting of FIR filter signal is done")

