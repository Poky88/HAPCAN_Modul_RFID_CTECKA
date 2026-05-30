# HAPCAN Extension Module - NFC Tag Reader

This repository contains complete hardware and software resources for the development and implementation of a new extension module for the HAPCAN home automation system. The module enables contactless user identification using RFID technology and subsequent control of smart home output elements via the CAN bus. This project was created as part of a bachelor's thesis.

<p align="center">
  <img width="40%" alt="PCB - Module-3D-TOP" src="https://github.com/user-attachments/assets/32bfc24e-4efe-4e4c-bd67-89a0c52af81c" />
  <br>
  <i>Fig. 1: Module - 3D visualization</i>
</p>


## Features
- <ins>Contactless control:</ins> Output actuator control using RFID technology
- <ins>Access management:</ins> Setting access rights via the official HAPCAN Programmer application
- <ins>Universal output:</ins> Ability to control any actuator operating at 24 V
- <ins>Easy installation:</ins> Housed in a protective enclosure for standard in-wall flush-mount electrical boxes
- <ins>Connection options:</ins> The module can be connected to the CAN bus using an RJ45 connector or a screw terminal block
- <ins>Full compatibility:</ins> The device respects the HAPCAN protocol and communicates seamlessly with it
- <ins>Tag indication:</ins> Indicator LEDs signal when a tag is scanned


# Hardware
The schematic design and printed circuit board (PCB) layout were created using KiCad. All project source files and complete manufacturing files are available in the respective folders.


| PCB Model - TOP | PCB Model - BOT |
| :---: | :---: |
| <img width="60%" alt="PCB - JLCPCB - TOP" src="https://github.com/user-attachments/assets/11aba09b-88bc-48cc-9fa7-2b01672ae39d" /> | <img width="60%" alt="PCB - JLCPCB - BOT" src="https://github.com/user-attachments/assets/1573caef-8063-4d5f-86e3-649a53c6a4c2" />|
<p align="center">
  <i> Fig. 2: PCB module visualization </i>
</p>
  

# Software

The complete firmware was developed in the Arduino IDE environment and is divided into two main parts. The operation of the NFC reader is handled by the `Hapcan_NFC.cpp` source file. CAN bus communication, module configuration, and interaction with the Hapcan Programmer application are managed by the second source file, `HapcanProtocol.cpp`. Both parts include corresponding header files for function declarations and library imports. To help understand the core program logic, flowchart diagrams of both main processes are provided below. The complete source codes are located in the attached folder.

<p align="center">
  <img width="50%" alt="Hapcan_NFC-flowchart drawio" src="https://github.com/user-attachments/assets/8f330b48-8d9e-4592-b60a-06143c630d38" />
  <br>
  <i>Fig. 3: Flowchart - NFC Reader Operation</i>
</p>


<p align="center">
  <img width="50%" alt="HapcanProtocol-flowchart drawio (1)" src="https://github.com/user-attachments/assets/16f24a4d-6b0b-4fc2-9c68-fbed4c1e641f" />
  <br>
  <i>Fig. 4: Flowchart - HAPCAN Protocol Operation</i>
</p>

# Module Construction

The complete module is housed inside a custom-designed protective enclosure. This case serves not only a protective function but also secures the NFC reader in the optimal position for reliable tag scanning. The fully assembled module is designed for easy mounting into a standard flush-mount in-wall electrical box. A detailed assembly and installation guide is described in the attached documentation, and the 3D models required for manufacturing the enclosure are included in the electronic appendices of the thesis. 

The following diagram illustrates an example wiring diagram using an electric lock. For correct operation, the module must be connected to the CAN bus and linked with a Button module.

<p align="center">
<img width="60%" alt="Screenshot 2026-05-30 093349" src="https://github.com/user-attachments/assets/afaede53-46b1-4e0d-a248-b437dd44ca3f" />
  <br>
  <i>Fig. 5: Typical wiring diagram</i>
</p>


<p align="center">
  <img width="40%" alt="Typical module assembly" src="https://github.com/user-attachments/assets/1b67e641-791f-4e68-b1b6-8cba85b5f1b9" />
  <br>
  <i>Fig. 6: Assembled module</i>
</p>

# File Structure

<pre>
Hardware/
└── KiCad 9.0/                  # PCB design projects
    ├── production
    ├── Module - DPS.kicad_pcb
    ├── Module - DPS.kicad_pro
    └── Module - DPS.kicad_sch
Software/
└── Arduino IDE 2.3.8/          # Firmware source files
    ├── HAPCAN.ino
    ├── HAPCAN_NFC.cpp
    ├── HAPCAN_NFC.h
    ├── HAPCANProtocol.cpp
    └── HAPCANProtocol.h
3D tisk/
└── Inventor 2026/              # 3D model of the protective enclosure
    ├── Cover - top part.ipt
    ├── Cover - assembly.iam
    └── Cover - bottom.ipt

Documentacion/                  # PDF documentation of the module
    └── Documentation.pdf
</pre>
