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
![DMA Pipeline](DMA_Pipeline.png) 

## 🧮 Software Post-Processing (Vitis)
Hardware logic is incredibly fast, but it requires careful mathematical handling when crossing back into the ARM CPU domain. We built a C-based DSP pipeline to handle two critical hardware phenomena:
* **DC Gain Compensation:** The hardware FIR filter uses integer math, naturally amplifying our voltage by a factor of exactly 148. We mathematically remove this gain in software.
* **Transient Flush:** A 21-tap filter takes 21 clock cycles to flush out its initial zero-state. If fed directly to the safety logic, this startup "garbage" data would trigger a false Under-Voltage shutdown. Our software silently drops the first 21 samples on boot.

![Software Pipeline](Software_Pipeline.png)

## 📅 The 6-Week Build Public Series
We are documenting the complete build process of this system on LinkedIn! 

* **Week 1:** The Architecture & The Bottleneck 
* **Week 2:** Hardware Architecture & The AXI DMA Pipeline 
* **Week 3:** DSP Math, Gain, & Software Pipelines 
* **Week 4:** [Coming Soon]
* **Week 5:** [Coming Soon]
* **Week 6:** [Coming Soon]
