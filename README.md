# Cerebroband
<h1>🧠 CerebroBand — Wearable Stroke Risk Detection Device (Without ECG)</h1>

<h2>1️⃣ Overall System Architecture</h2>

<p><strong>System Flow:</strong></p>
<p><code>Sensors → ESP32 → Data Analysis Algorithm → Bluetooth/Wi-Fi → Cloud → Dashboard + SOS</code></p>

<ul>
  <li>Continuously collects physiological data</li>
  <li>Processes data locally (edge processing)</li>
  <li>Detects abnormalities</li>
  <li>Transmits data to mobile application</li>
  <li>Activates emergency alerts in critical situations</li>
</ul>

<hr>

<h2>2️⃣ Sensors Used</h2>

<h3>❤️ PPG Sensor (Heart Rate + SpO₂)</h3>

<p><strong>Component:</strong> MAX30102</p>

<p><strong>Measures:</strong></p>
<ul>
  <li>Heart Rate (BPM)</li>
  <li>Heart Rate Variability (HRV)</li>
  <li>Blood Oxygen Saturation (SpO₂)</li>
</ul>

<p><strong>How it works:</strong></p>
<p>
Uses photoplethysmography (PPG). Red and infrared light pass through tissue,
and a photodiode measures reflected light. Signal variations determine pulse and oxygen level.
</p>

<p><strong>Detects:</strong></p>
<ul>
  <li>Irregular rhythm</li>
  <li>Suspected atrial fibrillation</li>
  <li>Tachycardia / Bradycardia</li>
  <li>Sudden SpO₂ drops</li>
</ul>

<hr>

<h3>🏃 Accelerometer + Gyroscope</h3>

<p><strong>Component:</strong> MPU6050</p>

<p><strong>Measures:</strong></p>
<ul>
  <li>3-axis acceleration</li>
  <li>Orientation and rotation</li>
</ul>

<p><strong>Used for:</strong></p>
<ul>
  <li>Fall detection</li>
  <li>Sudden loss of balance</li>
  <li>Immobility detection</li>
</ul>

<hr>

<h3>🌡 Temperature Sensor</h3>

<p><strong>Component:</strong> Built-in sensor or MLX90614</p>

<ul>
  <li>Monitors general condition</li>
  <li>Provides additional health parameter</li>
</ul>

<hr>

<h3>📍 GPS Module</h3>

<p><strong>Component:</strong> Neo-6M</p>

<ul>
  <li>Sends coordinates during SOS</li>
  <li>Shares location with relatives</li>
  <li>Confirms immobility</li>
</ul>

<hr>

<h2>3️⃣ Central Controller</h2>

<h3>💻 ESP32 Microcontroller</h3>

<p><strong>Why ESP32:</strong></p>
<ul>
  <li>Built-in Wi-Fi</li>
  <li>Built-in Bluetooth</li>
  <li>Low power consumption</li>
  <li>Compact size</li>
  <li>Real-time processing capability</li>
</ul>

<p><strong>Functions:</strong></p>
<ul>
  <li>Reads sensor data</li>
  <li>Filters noise</li>
  <li>Calculates beat intervals</li>
  <li>Detects abnormalities</li>
  <li>Sends data to cloud</li>
</ul>

<hr>

<h2>4️⃣ Stroke Risk Detection Logic</h2>

<h3>📊 Heart Rhythm Irregularity</h3>
<p>Unstable beat intervals may indicate atrial fibrillation.</p>

<h3>📉 SpO₂ Drop</h3>
<p>SpO₂ below 90% may indicate hypoxia.</p>

<h3>🚨 Combined Risk Logic</h3>
<ul>
  <li>Irregular rhythm + tachycardia</li>
  <li>Fall detected + abnormal heart rate</li>
  <li>Long immobility + sudden HR spike</li>
  <li>Critically low SpO₂</li>
</ul>

<hr>

<h2>5️⃣ Emergency Alert System</h2>

<ul>
  <li>Buzzer activation</li>
  <li>LED flashing indicator</li>
  <li>Mobile app notification</li>
  <li>GPS coordinates transmission</li>
  <li>Physical SOS button</li>
</ul>

<hr>

<h2>6️⃣ Power System</h2>

<ul>
  <li>3.7V Li-Po Battery</li>
  <li>TP4056 Charging Module</li>
  <li>Over-discharge protection</li>
  <li>24–36 hours battery life</li>
</ul>

<hr>

<h2>7️⃣ Key Advantages</h2>

<ul>
  <li>Preventive medical monitoring</li>
  <li>Real-time abnormality detection</li>
  <li>Multi-sensor data fusion</li>
  <li>Automatic emergency activation</li>
</ul>
