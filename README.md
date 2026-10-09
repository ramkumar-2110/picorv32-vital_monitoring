# RISC-V-Based Real-Time Cardiac Monitoring System

## Project Description

This project develops a real-time, PPG-based cardiac monitoring system using a PicoRV32 RISC-V System-on-Chip (SoC) implemented on a ZedBoard FPGA. The MAX30102 sensor captures red and infrared photoplethysmography (PPG) signals and communicates with the processor through I2C.

The system uses a memory-mapped FIR hardware accelerator to filter acquired PPG samples. Firmware manages sensor configuration, data acquisition, periodic processing, and communication with a PC through UART. The acquired signals are processed to estimate heart rate and peripheral oxygen saturation (SpO2), while a PC-side application displays measurement results and PPG waveforms. An LED indicates when estimated values cross configured thresholds.

The project also compares software-based FIR filtering with hardware-accelerated filtering by measuring execution cycles and processing latency.

**Note:** The MAX30102 measures optical PPG signals; it does not directly measure ECG. Heart-rate and SpO2 values are estimates for educational and research purposes, not medical diagnoses.

## My Role: Firmware and Application Development

My responsibility is to develop the embedded firmware and PC-side application and integrate them with the PicoRV32-based SoC.

### 1. Firmware Development

- Develop embedded C firmware for the PicoRV32 RISC-V processor.
- Implement an I2C driver to configure the MAX30102 and read red and infrared samples.
- Implement timer-driven sampling and data-processing routines.
- Develop firmware drivers for the memory-mapped FIR accelerator, including start and completion handling.
- Process filtered PPG data to estimate heart rate and SpO2.
- Transmit sensor samples, calculated values, and diagnostic information through UART.
- Implement threshold-based LED indication.

### 2. Hardware–Software Integration

- Integrate firmware with the SoC's I2C, timer, UART, and FIR interfaces.
- Debug sensor communication, peripheral access, and data flow.
- Compare software FIR filtering with hardware-accelerated filtering.
- Measure execution cycles, processing latency, and real-time performance.

### 3. PC Application Development

- Develop a PC-side application to receive UART data.
- Display heart-rate and SpO2 estimates and sensor status.
- Plot red and infrared PPG waveforms in real time.
- Record received data for testing and performance analysis.

## Technologies Used

- **Processor:** PicoRV32 RISC-V
- **FPGA Platform:** ZedBoard
- **Sensor:** MAX30102
- **Programming Languages:** Embedded C and Python
- **Communication Interfaces:** I2C and UART
- **Signal Processing:** FIR filtering and PPG analysis
- **Hardware Integration:** Memory-mapped registers and timer interrupts

## Expected Outcome

A working prototype that acquires PPG data from the MAX30102, filters the samples using the FIR accelerator, reports measurements to a PC application, and enables performance comparison between software-based and hardware-accelerated FIR filtering.
