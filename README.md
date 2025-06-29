# ⚙️ Advanced FCC Module for CNN Acceleration  
> Fully Connected Layer Hardware Accelerator implemented in Verilog (ZYBO Z7-20 + AXI4)

---

## 📌 Project Overview

This project implements a **hardware-accelerated Fully Connected Layer (FCC)** for CNNs using Verilog HDL.  
It enhances the original multiplication-only FCC logic by supporting:

- ✅ Signed/Unsigned data handling
- ✅ Bias addition
- ✅ ReLU activation function
- ✅ AXI4 interface for communication with ARM core

Designed to run on **Xilinx ZYBO Z7-20 (Zynq-7000 SoC)** using **Vivado + Vitis**.

---

## 📁 Directory Structure

Advanced_Fcc_module/
├── src/ # Verilog source files
│ ├── fcc_module.v # Core multiply-accumulate logic
│ ├── relu_module.v # ReLU activation
│ ├── top_axi.v # AXI4 interfaced top module
│ └── data_mover.v # Data flow management
├── tb/ # Testbench files
├── main.c # Vitis C source to test accelerator
├── vivado_project/ # Vivado block design & constraints
├── sim/ # Vivado simulation outputs
└── README.md # This file

yaml
복사
편집

---

## 🔧 Technologies Used

| Tool         | Details                                   |
|--------------|--------------------------------------------|
| **HDL**      | Verilog                                    |
| **FPGA**     | ZYBO Z7-20 (Zynq-7000)                     |
| **Toolchain**| Vivado 2022.1, Vitis SDK                   |
| **Interface**| AXI4-Lite                                  |
| **C/Driver** | ARM-side control using main.c              |

---

## 📐 Architecture

```txt
  Input → FCC Module → ReLU → Result Register → AXI Read
            │            │
         (bias)       (threshold=0)
Signed Handling: Supports signed input and weight multiplication

Bias: Applied after MAC operation

ReLU: Zeroes out negative values

AXI: Used for interfacing with ARM CPU

✅ Functional Highlights
fcc_module.v: Multiply-Accumulate unit with signed input handling

relu_module.v: Implements assign out = (in > 0) ? in : 0;

top_axi.v: Memory-mapped AXI wrapper for FPGA ↔ ARM interface

main.c: ARM-side driver to feed inputs and read outputs

🧪 Simulation & Testing
RTL simulation done via Vivado Simulation

main.c used to send random 8-bit signed inputs via AXI

Output result verified vs. software FCC output

🧾 Result (Performance Comparison)
Method	Latency (μs)
Software (C)	1898.18
Hardware (FPGA)	41.64
🔥 Speedup	~45x faster

🎬 Demo
📽️ [YouTube Demo Link (Optional)]
📁 Simulation Report & Waveform Snapshots

🛠️ To Build & Run
🧱 Vivado
Open Vivado → Create project → Import vivado_project/

Run Synthesis → Generate Bitstream

Export to Vitis

🧠 Vitis
Open Vitis → New Application Project → Link main.c

Build & Run on ZYBO Z7-20

📌 Author
Taehoon Kang (강태훈)
📧 jjwkth1@gmail.com
📗 Naver Blog

