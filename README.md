# Real-time Alert System for Auxiliary Transformer Failures

## Overview

Auxiliary transformers are used extensively in railway systems. Indian Railways use them primarily to convert the 25-kilo volt AC supply into 230-240-volt AC supply as required for various applications. Due to their varied use, many of these auxiliary transformers are often positioned in remote locations. In case of failure of AT supply, no proper feedback system exists as of now. Due to this the lead time on failure attention increases rapidly depending on the location of the transformer. Sometimes this may result in signal failure leading to an increase in train traffic. To cut down on human effort and cost involved, as well as provide a system that continuously monitors these auxiliary transformers, an automated system based on GSM technology is suggested. This system periodically provides updates and generates immediate alerts in case of the occurrence of the failure of an auxiliary transformer, hence bringing a drastic reduction on lead time for failure attention where a failure escalates into costly service losses.

## Hardware Requirements

- **GSM Module**: For sending periodic updates and immediate failure alerts.
- **Microcontroller**: To process data from the sensors and control the GSM module.
- **Voltage Sensors**: To monitor the output voltage of the auxiliary transformers.
- **Power Supply Unit**: To power the monitoring system.
- **Enclosure**: Weather-proof casing to protect the system in remote locations.

<img width="262" alt="setup" src="https://github.com/dheerajkallakuri/Real-time-Alert-System-for-Auxiliary-Transformer-Failures/assets/23552796/aa31f915-6369-4853-9c9e-4d4665a7a33d">


## Software Requirements

- **Embedded C/C++**: For programming the microcontroller.
- **GSM Communication Library**: For handling GSM module operations.

## Setup
-  Run `ContactSave.exe` to store phone numbers.
-  Load `FinalProgramReadSignal.ino` on the Arduino Microcontroller 

## Description

The GSM-based monitoring system for auxiliary transformers operates as follows:

1. **Continuous Monitoring**: The system continuously monitors the output voltage of the auxiliary transformers using relays.
2. **GSM Communication**:
    - **Periodic Updates**: The system sends periodic updates to a central monitoring system via the GSM module. These updates include the current status and output voltage of the transformers.
    - **Immediate Alerts**: In case of a failure (e.g., the output voltage drops below a certain threshold), the system generates an immediate alert and sends it to the designated maintenance personnel via SMS.
3. **Failure Response**: Maintenance personnel receive the alert and can promptly address the failure, reducing the lead time and preventing escalation into costly service disruptions.

## Benefits

- **Reduced Lead Time**: Quick detection and reporting of transformer failures enable faster response and repair, reducing downtime.
- **Cost Efficiency**: Automating the monitoring process cuts down on human effort and associated costs.
- **Enhanced Reliability**: Continuous monitoring ensures that transformers are always operating within their optimal range, preventing potential failures.
- **Improved Safety**: Prompt failure detection prevents issues that could lead to signal failures and increased train traffic, ensuring a safer railway operation.

## Paper Preview

### Title: 
**Real-time Alert System for Auxiliary Transformer Failures**

### Authors:
Afrin Dange, Dheeraj Kallakuri, Edwin Fernando, and Hanah Susan Zachariah; 

### Link to Full Paper:
[Read the full paper here](https://ieeexplore.ieee.org/document/9740841)

### Citation:
A. Dange, E. Fernando, H. S. Zachariah and D. Kallakuri, Real-time Alert System for Auxiliary Transformer Failures. *2022 IEEE International Students' Conference on Electrical, Electronics and Computer Science (SCEECS)*, BHOPAL, India, 2022, pp. 1-4. doi: 10.1109/SCEECS54111.2022.9740841. 
