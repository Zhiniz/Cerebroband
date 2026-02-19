# Cerebroband
CerebroBand — Wearable Stroke Risk Detection Device (Without ECG)
1️⃣ Overall System Architecture
The system operates according to the following structure:
Sensors → ESP32 → Data Analysis Algorithm → Bluetooth/Wi-Fi → Cloud → Dashboard + SOS
The device:
Continuously collects physiological data
Processes it locally (edge processing)
Detects abnormalities
Transmits data to the mobile application
Activates emergency alerts in critical situations
2️⃣ Sensors Used
❤️ 1. PPG Sensor for Heart Rate and SpO₂
Component: MAX30102
Measures:
Heart rate (BPM)
Heart rate variability (HRV)
Blood oxygen saturation (SpO₂)
How it works:
The system uses photoplethysmography (PPG).
Red and infrared light pass through body tissues, and a photodiode measures the reflected signal.
Changes in light absorption allow calculation of pulse and oxygen saturation.
Why ECG is not required:
Even without ECG, the system can detect:
Irregular heart rhythm
Suspected atrial fibrillation
Tachycardia / bradycardia
Sudden drops in SpO₂
The algorithm analyzes peak-to-peak intervals in the signal.
🏃 2. Accelerometer + Gyroscope
Component: MPU6050
Measures:
3-axis acceleration
Rotation and orientation
Used for:
Fall detection
Detecting sudden loss of balance
Identifying prolonged immobility after impact
The algorithm detects:
Sudden acceleration spike
Transition to horizontal position
No movement for a defined time threshold
🌡 3. Body Temperature Sensor
Built-in temperature sensor or MLX90614.
Purpose:
Monitoring general physical condition
Additional parameter for risk analysis
📍 4. GPS Module
Component: Neo-6M
Purpose:
Sending coordinates during SOS
Sharing location with relatives
Confirming immobility
3️⃣ Central Controller
💻 ESP32 Microcontroller
Why ESP32 was selected:
Built-in Wi-Fi
Built-in Bluetooth
Low power consumption
Sufficient processing power
Compact size
ESP32:
Reads sensor data
Filters signal noise
Calculates beat-to-beat intervals
Detects abnormalities
Sends data to the cloud
4️⃣ Stroke Risk Detection Logic
Since there is no ECG sensor, detection is based on:
📊 1. Heart Rhythm Irregularity
Unstable beat intervals may indicate atrial fibrillation.
📉 2. Sudden SpO₂ Decrease
SpO₂ < 90% indicates possible hypoxia.
🚨 3. Combined Risk Logic
Emergency alert is triggered if:
Irregular rhythm + tachycardia
Fall detected + abnormal heart rate
Long immobility + sudden HR spike
Critically low SpO₂
5️⃣ Emergency Alert System
In critical situations:
Buzzer activates
LED indicator flashes
Notification is sent via mobile app
GPS coordinates are transmitted
SOS mode is activated
A physical emergency button is also included.
6️⃣ Power System
3.7V Li-Po battery
TP4056 charging module
Over-discharge protection
24–36 hours battery life
7️⃣ How CerebroBand Differs from Regular Wearables
Most fitness devices (such as Apple Watch) focus primarily on activity tracking.
CerebroBand:
Functions as a preventive medical system
Analyzes heart rhythm patterns
Combines heart rate + SpO₂ + motion data
Automatically activates emergency alerts
