# IoT-Based Smart Parking System

An IoT-based smart parking prototype that uses IR sensors to detect vehicle entry, exit and parking-slot occupancy. A NodeMCU ESP8266 automatically controls the entry and exit gates using servo motors and sends real-time parking information to Firebase and ThingSpeak for monitoring and visualisation.

**Department of Electrical and Electronics Engineering, Shridevi Institute of Engineering and Technology, Tumakuru (VTU) | Mini Project**

## Problem
Parking areas are becoming difficult to manage manually because of limited space and a growing number of vehicles. Traditional parking systems do not show real-time slot availability, which causes waiting time, traffic congestion and poor use of space. This project automates vehicle detection and shows parking availability through an IoT-based monitoring system.

## Objectives
- Detect vehicle entry and exit using IR sensors
- Automatically control entry and exit gates using servo motors
- Monitor three parking slots and identify each as Empty or Occupied
- Display total vehicle entry and exit counts
- Upload real-time parking information to Firebase and ThingSpeak
- Develop an HTML webpage that displays parking status and vehicle information

## How it works
1. Five IR sensors detect vehicles at the entry, the exit and three individual parking slots.
2. The NodeMCU ESP8266 reads and processes the signals from all five sensors.
3. When a vehicle is detected at the entry, the entry servo motor opens the gate automatically.
4. The three slot sensors identify whether each parking space is Empty or Occupied.
5. When a vehicle leaves, the exit IR sensor detects it and activates the exit servo motor.
6. The NodeMCU updates the total number of vehicles entered and exited.
7. Slot status, vehicle counts and timestamps are uploaded to Firebase.
8. ThingSpeak receives the data and generates graphs of parking activity.
9. An HTML webpage reads the data from Firebase and displays available slots, occupied slots, total vehicles and time information.

## Hardware components
| Component | Purpose |
|---|---|
| NodeMCU ESP8266 | Main controller for sensor processing and Wi-Fi communication |
| IR sensors (5) | Detect vehicles at the entry, the exit and three parking slots |
| Servo motors (2) | Automatically operate the entry and exit gates |
| Breadboard and jumper wires | Connections between components |
| Power supply | Powers the system |

## Tools and technologies
- NodeMCU ESP8266, Arduino IDE, Embedded C/C++
- IR sensor interfacing and servo motor control
- Firebase Realtime Database for data storage
- ThingSpeak for cloud-based data visualisation
- HTML for the monitoring webpage
- Wi-Fi communication and IoT

## Results
- Detects vehicle entry and exit using IR sensors
- Monitors three parking slots and shows their availability
- Operates the entry and exit gates automatically using servo motors
- Maintains total vehicle entry and exit counts
- Updates parking information in Firebase
- Displays parking activity through ThingSpeak graphs
- Shows real-time parking information on the HTML webpage
- Reduces manual monitoring and improves parking management

## Setup note
Wi-Fi credentials and Firebase/ThingSpeak keys have been removed from the code for security. Replace the placeholders in the source file with your own values before uploading it to the board.

## Future scope
- GPS integration for locating available parking areas
- Mobile application for real-time parking information
- Online parking reservation and booking
- Additional parking slots
- Automatic payment and digital ticket generation
- Vehicle identification using RFID or a camera
- Cloud-based management of multiple parking locations

## Team
- Gopal S M
- Gopal Reddy
- Venugopal H L

**Guide:** Dr. Tanuja K S, Associate Professor and HOD, Department of EEE

## Repository contents
- `code/` NodeMCU ESP8266 source code
- `images/` block diagram, circuit diagram and project photos
- `output/` Firebase dashboard, ThingSpeak graphs and webpage screenshots
- `demo/` demonstration video (if available)
