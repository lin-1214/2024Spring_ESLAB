
import socket
import json
import numpy as np
import matplotlib.pyplot as plt
from collections import deque
import time
window_size = 50  # 窗口大小
timestamps = deque(maxlen=window_size)
temperature_data = deque(maxlen=window_size)
humidity_data = deque(maxlen=window_size)
pressure_data = deque(maxlen=window_size)

magneto_data = {"X":deque(maxlen=window_size),"Y":deque(maxlen=window_size),"Z":deque(maxlen=window_size)}
gyro_data = {"X":deque(maxlen=window_size),"Y":deque(maxlen=window_size),"Z":deque(maxlen=window_size)}
accelero_data = {"X":deque(maxlen=window_size),"Y":deque(maxlen=window_size),"Z":deque(maxlen=window_size)}


plt.ion()  # Turn on interactive mode
fig, axs = plt.subplots(4, 1, figsize=(10, 10))
# Create plots for each type of data
plots = {
    'Temperature, Humidity, Pressure': axs[0],
    'Magneto': axs[1],
    'Gyro': axs[2],
    'Accelero': axs[3]
}

for title, ax in plots.items():
    ax.set_title(title)
    ax.set_xlabel('Time (s)')
    ax.grid(True)
def plot_data():
    # Clear previous plots
    for ax in axs:
        ax.clear()

    # Plot Temperature, Humidity, Pressure
    plots['Temperature, Humidity, Pressure'].plot(timestamps, temperature_data, label='Temperature')
    plots['Temperature, Humidity, Pressure'].plot(timestamps, humidity_data, label='Humidity')
    plots['Temperature, Humidity, Pressure'].plot(timestamps, pressure_data, label='Pressure')
    plots['Temperature, Humidity, Pressure'].legend()

    # Plot Magneto
    plots['Magneto'].plot(timestamps, magneto_data['X'], label='MAGNETO_X')
    plots['Magneto'].plot(timestamps, magneto_data['Y'], label='MAGNETO_Y')
    plots['Magneto'].plot(timestamps, magneto_data['Z'], label='MAGNETO_Z')
    plots['Magneto'].legend()

    # Plot Gyro
    plots['Gyro'].plot(timestamps, gyro_data['X'], label='GYRO_X')
    plots['Gyro'].plot(timestamps, gyro_data['Y'], label='GYRO_Y')
    plots['Gyro'].plot(timestamps, gyro_data['Z'], label='GYRO_Z')
    plots['Gyro'].legend()

    # Plot Accelero
    plots['Accelero'].plot(timestamps, accelero_data['X'], label='ACCELERO_X')
    plots['Accelero'].plot(timestamps, accelero_data['Y'], label='ACCELERO_Y')
    plots['Accelero'].plot(timestamps, accelero_data['Z'], label='ACCELERO_Z')
    plots['Accelero'].legend()

    plt.tight_layout()
    plt.draw()
    plt.pause(0.1)

def data_receiver(data):
    # Simulate receiving data every 500ms
    # Parse JSON data
    parsed_data = json.loads(data)
    timestamp = time.time()

    # Append data to respective lists
    timestamps.append(timestamp)
    temperature_data.append(parsed_data['temperature'])
    humidity_data.append(parsed_data['humidity'])
    pressure_data.append(parsed_data['pressure'])
    magneto_data['X'].append(parsed_data['MAGNETO_X'])
    magneto_data['Y'].append(parsed_data['MAGNETO_Y'])
    magneto_data['Z'].append(parsed_data['MAGNETO_Z'])
    gyro_data['X'].append(parsed_data['GYRO_X'])
    gyro_data['Y'].append(parsed_data['GYRO_Y'])
    gyro_data['Z'].append(parsed_data['GYRO_Z'])
    accelero_data['X'].append(parsed_data['ACCELERO_X'])
    accelero_data['Y'].append(parsed_data['ACCELERO_Y'])
    accelero_data['Z'].append(parsed_data['ACCELERO_Z'])
    
    
HOST = "192.168.50.32" # IP address
PORT = 4000 # Port to listen on (use ports > 1023)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print("Starting server at: ", (HOST, PORT))
    conn, addr = s.accept()
    with conn:
        print("Connected at", addr)
        while True:
            data = conn.recv(1024).decode('utf-8')
            data_receiver(data)
            plot_data()
            