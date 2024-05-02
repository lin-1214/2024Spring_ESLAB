# Intro
This project FRI filter and verify the result from STM32. 

# Setup
## 1. Gather the sine wave result of FRI filter
We will first need to gather the output data of STM32, which is the result after filtering the sine wave signal with frequency 1000Hz and 15000Hz. Type the data into file `./data/sine_output.txt`

## 2. Run verification code
Execute the code below in the terminal to verify the filtering result of sine wave:

    python3 verify.py -si

## 3. Gather the input data & output data of the real data
With the sensor on STM32, we get the input data and the filtered data. Type the data into file `./data/input.txt` and `./data/output.txt` respectively.

## 4. Run verification code
Execute the code below in the terminal to verify the filtering result of sine wave:

    python3 verify.py -d

## 5. Check the result waveform
The plot results are saved in `./plots`, check whether the filtering result gather from STM32 aligns to the one done by python.  
