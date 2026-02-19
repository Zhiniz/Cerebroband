# Cerebroband
<h1 align="center">🧠 CerebroBand</h1>
<h3 align="center">Wearable Stroke Risk Detection Device (Without ECG)</h3>

<hr>

<h2>1. System Overview</h2>

<p>
CerebroBand is a wearable preventive health monitoring device designed to detect early signs
of stroke and cardiovascular abnormalities through continuous physiological monitoring.
</p>

<p><strong>System Architecture:</strong></p>

<pre>
Sensors → ESP32 → Edge Processing Algorithm → Bluetooth/Wi-Fi → Cloud Server → Dashboard + SOS
</pre>

<ul>
  <li>Continuous real-time physiological monitoring</li>
  <li>On-device signal processing (edge computing)</li>
  <li>Abnormality detection and risk evaluation</li>
  <li>Wireless data transmission</li>
  <li>Automatic emergency activation</li>
</ul>

<hr>

<h2>2. Hardware Components</h2>

<h3>2.1 PPG Sensor (MAX30102)</h3>

<p><strong>Measured Parameters:</strong></p>
<ul>
  <li>Heart Rate (BPM)</li>
  <li>Heart Rate Variability (HRV)</li>
  <li>Blood Oxygen Saturation (SpO₂)</li>
</ul>

<p><strong>Operating Principle:</strong></p>
<p>
The sensor uses photoplethysmography (PPG). Red and infrared light penetrate tissue,
and a photodiode measures variations in reflected light intensity.
Signal fluctuations are processed to calculate heart rate and oxygen saturation.
</p>

<p><strong>Clinical Relevance:</strong></p>
<ul>
  <li>Detection of irregular heart rhythm</li>
  <li>Suspicion of atrial fibrillation</li>
  <li>Tachycardia and bradycardia identification</li>
  <li>Hypoxia detection (SpO₂ &lt; 90%)</li>
</ul>

<hr>

<h3>2.2 Motion Sensor (MPU6050)</h3>

<p><strong>Measured Parameters:</strong></p>
<ul>
  <li>3-axis acceleration</li>
  <li>Angular velocity and orientation</li>
</ul>

<p><strong>Functions:</strong></p>
<ul>
  <li>Fall detection</li>
  <li>Sudden loss of balance identification</li>
  <li>Prolonged immobility monitoring</li>
</ul>

<hr>

<h3>2.3 Temperature Sensor</h3>

<p><strong>Component:</strong> Integrated sensor or MLX90614</p>

<ul>
  <li>Monitoring general physiological condition</li>
  <li>Providing additional risk assessment parameter</li>
</ul>

<hr>

<h3>2.4 GPS Module (Neo-6M)</h3>

<ul>
  <li>Real-time location tracking</li>
  <li>Transmission of coordinates during emergency events</li>
  <li>Verification of user immobility</li>
</ul>

<hr>

<h2>3. Processing Unit</h2>

<h3>ESP32 Microcontroller</h3>

<p><strong>Key Features:</strong></p>
<ul>
  <li>Integrated Wi-Fi and Bluetooth connectivity</li>
  <li>Low power consumption</li>
  <li>Real-time signal processing capability</li>
  <li>Compact form factor</li>
</ul>

<p><strong>Responsibilities:</strong></p>
<ul>
  <li>Sensor data acquisition</li>
  <li>Signal filtering and noise reduction</li>
  <li>Beat-to-beat interval calculation</li>
  <li>Abnormal pattern recognition</li>
  <li>Cloud data transmission</li>
</ul>

<hr>

<h2>4. Risk Detection Algorithm</h2>

<h3>4.1 Heart Rhythm Irregularity Analysis</h3>
<p>
Beat-to-beat interval instability is analyzed to detect potential atrial fibrillation
and other arrhythmias.
</p>

<h3>4.2 Oxygen Saturation Monitoring</h3>
<p>
SpO₂ levels below 90% are classified as critical and may indicate hypoxia.
</p>

<h3>4.3 Multi-Parameter Risk Logic</h3>

<p>Emergency mode is triggered under the following combined conditions:</p>

<ul>
  <li>Irregular rhythm combined with tachycardia</li>
  <li>Detected fall with abnormal heart rate</li>
  <li>Prolonged immobility with sudden heart rate spike</li>
  <li>Critically low oxygen saturation</li>
</ul>

<hr>

<h2>5. Emergency System</h2>

<ul>
  <li>Audible alarm activation</li>
  <li>LED visual alert</li>
  <li>Mobile application notification</li>
  <li>GPS coordinate transmission</li>
  <li>Physical SOS button</li>
</ul>

<hr>

<h2>6. Power System</h2>

<ul>
  <li>3.7V Li-Po battery</li>
  <li>TP4056 charging module</li>
  <li>Over-discharge protection circuit</li>
  <li>24–36 hours operational autonomy</li>
</ul>

<hr>

<h2>7. Key Advantages</h2>

<ul>
  <li>Preventive cardiovascular monitoring</li>
  <li>Multi-sensor data fusion</li>
  <li>Edge-based real-time analysis</li>
  <li>Automatic emergency response system</li>
</ul>

<hr>

<p align="center"><strong>CerebroBand — Real-Time Preventive Health Monitoring</strong></p>
