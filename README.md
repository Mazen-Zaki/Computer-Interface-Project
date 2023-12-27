# Computer-Interface-Project
Project Description
This repository contains firmware for interfacing a PC with an STM32F401RC microcontroller through UART using TTL communication. The project is structured with drivers such as RCC, GPIO, ADC, NVIC, SYSTIC, and UART in the MCAL layer. Within the MCAL layer, functionalities for LCD, LDR, and LM35 temperature sensor are integrated.

![WhatsApp Image 2023-12-23 at 22 30 33_006aba9b](https://github.com/Mazen-Zaki/Smart-Home-Project/assets/76655984/5f9fd906-3d7e-41f8-af78-f044140ee2b5)


# Features:
UART Communication: Establishes bidirectional communication between the PC and the microcontroller via UART.
Sensors Integration: Includes functionalities for LM35 temperature sensor, LDR sensor, and gas sensor (connected to GPIO).
Actuators Control: Controls various actuators such as a fan and motors based on sensor readings or user commands received through UART.
Code Overview:
The main application code orchestrates the following functionalities:

UART Callback: Handles incoming UART data and processes it accordingly.
Sensor Functions:
Temperature Sensor: Reads temperature from LM35 and sends data to the PC.
Gas Sensor: Monitors gas sensor input and sends corresponding data to the PC.
LDR Sensor: Reads light levels through LDR and communicates it to the PC.
Actuators Control:
Fan Control: Regulates fan operation based on temperature sensor readings.
Motor Control: Manages motor functions for specific applications.
LED Control: Controls LEDs based on received commands.
How to Use:
To utilize this codebase, set up the STM32F401RC microcontroller, configure GPIO pins, and ensure proper connections for sensors and actuators. Customize the main function as per your specific project requirements, integrating additional functionalities if needed.

Feel free to explore and adapt this codebase for your embedded system projects!

 
