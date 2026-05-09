# ⚡ Real-Time Hardware-Accelerated EV BMS Telemetry

**Target Hardware:** Xilinx Zynq-7000 System-on-Chip (SoC)  
**Development Tools:** AMD Xilinx Vivado & Vitis Unified IDE  
**Team Members:** [YOGARATHINAM K], [PRADEEP KUMAR S], [HEMAVARSHINI M], [PRATHANYAA S]

## 🚗 Project Overview
In modern Electric Vehicles, traction inverters switch at incredibly high frequencies, creating massive Electromagnetic Interference (EMI). This noise corrupts the low-voltage analog sensors of the Battery Management System (BMS), potentially causing false safety shutdowns.

Traditional microcontrollers filter this noise using software (DSP), which consumes 100% of the CPU load and introduces latency. 

**Our Solution:** We built a Hardware-Software Co-Design architecture. We offloaded the computationally expensive DSP filtering to the FPGA fabric using an AXI DMA pipeline. This freed up the ARM Cortex-A9 processor to execute advanced EV physics simulations, State of Charge (SoC) estimation, and safety fault detection with zero latency.

![CPU Bottleneck Diagram](Image2.png) 
*(Above: The Software CPU bottleneck problem we are solving)*

## 📅 The 6-Week Build Public Series
We are documenting the complete build process of this system on LinkedIn! 

* **Week 1:** The Architecture & The Bottleneck
* **Week 2:** Building the DMA Pipeline in Vivado
* **Week 3:** DSP Math & Hardware Gain
* **Week 4:** Bare-Metal Memory Cache Management
* **Week 5:** EV Physics & The Safety State Machine
* **Week 6:** The Final Oscilloscope Visualization & Code Drop!

*Note: The complete `main.c` source code and full PDF Project Report will be uploaded to this repository in Week 6!*
