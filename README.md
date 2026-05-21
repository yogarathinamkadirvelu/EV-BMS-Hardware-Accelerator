# ⚡ Real-Time Hardware-Accelerated EV BMS Telemetry

**Target Hardware:** Xilinx Zynq-7000 System-on-Chip (SoC)  
**Development Tools:** AMD Xilinx Vivado & Vitis Unified IDE  
**Team Members:** [YOGARATHINAM K], [PRADEEPKUMAR S], [HEMAVARSHINI M], [PRATHANYAA S]

## 🚗 Project Overview
In modern Electric Vehicles, traction inverters switch at incredibly high frequencies, creating massive Electromagnetic Interference (EMI). This noise corrupts the low-voltage analog sensors of the Battery Management System (BMS), potentially causing false safety shutdowns.

Traditional microcontrollers filter this noise using software (DSP), which consumes 100% of the CPU load and introduces latency. 

**Our Solution:** We built a Hardware-Software Co-Design architecture. We offloaded the computationally expensive DSP filtering to the FPGA fabric using an AXI DMA pipeline. This freed up the ARM Cortex-A9 processor to execute advanced EV physics simulations, State of Charge (SoC) estimation, and safety fault detection with zero latency.

![CPU Bottleneck Diagram](Image2.png) 

## 🛠️ Hardware Architecture (Vivado)
To filter extreme inverter noise without taxing the CPU, we built a custom AXI data highway inside the Zynq-7000 SoC.
1. **Master Controller:** Zynq Processing System (PS) generating a 100MHz fabric clock. 
2. **AXI DMA Engine:** Configured in Simple Transfer mode to autonomously stream raw voltage data from memory (MM2S) to the hardware and capture the clean results (S2MM).
3. **FIR Filter Accelerator:** A 21-tap FIR Compiler IP maps the heavy convolution math directly onto the FPGA's DSP48E slices.

![Vivado Block Design](Vivado_Block_Design.png) 

## 🧠 Bare-Metal Memory & Cache Coherency
In a Hardware-Software Co-Design system, the CPU and the DMA share the same physical DDR memory but have different views of it due to the CPU's L1/L2 cache layers. To prevent the data bus from fetching stale data, we enforce strict programmatic Cache Coherency.
👉 **[View Cache Coherency Implementation Source Code](./code/cache_management.c)**

## 🛡️ EV Physics & Safety State Machine
With a clean, latency-free telemetry stream secured by the FPGA, the ARM Cortex-A9 processor is dedicated entirely to application-layer safety logic.
👉 **[View BMS State Machine Source Code](./code/main.c)**

## 📊 System Validation & Live Experimental Telemetry
To validate the system under real-world conditions, we programmatically injected aggressive 300mV peak-to-peak random noise spikes directly over our baseline voltage data to simulate severe traction inverter interference. 

The live output data stream was channeled via UART to the Vitis Serial Terminal and a secondary Arduino interface. Our experimental results confirmed:
* **0% CPU Load for DSP:** The hardware accelerator handled 100% of the mathematical convolution filtering in parallel fabric.
* **Deterministic Fault Detection:** The application layer maintained a flawless safety state, yielding **zero false-positive safety trips**.

![Serial Monitor Validation](Serial_Monitor.png)
*(Above: Real-time UART data capture demonstrating full noise mitigation)*

📁 *Note: The raw experimental demo video clip, master C-code, and the comprehensive PDF Engineering Project Report are available in this repository for full open-source review.*

## 📅 Completed Build Series Timeline
* **Phase 1:** The Architecture & The Bottleneck 
* **Phase 2:** Hardware Architecture & The AXI DMA Pipeline 
* **Phase 3:** DSP Math, Gain, & Software Pipelines 
* **Phase 4:** Bare-Metal Memory Cache Management 
* **Phase 5:** EV Physics & The Safety State Machine 
* **Phase 6:** System Validation & Open Source Code Drop
