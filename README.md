> :warning: **This Project is still under development**
# Just walk out store using IOT
_IOT based replication of Amazon Go Store with automated inventory management, Real time facial detection and various sensors embedded with Raspberry pi, Arduino and ESP32_

**Table of Contents**

- [Features](#Features)
- [Hardware Needed](#Hardware-Needed)
- [Installing Node-Red and influxDB](#Installing-Node-Red-and-influxDB)
- [Installing OpenCV and Face Detection](#Installing-OpenCV-and-Face-Detection)
## Features
- Face detection using OpenCV on Raspberry pi.
- Inventory/Stock monitoring with weight sensors.
- QR Code and RFID for entrance and exit systems.

## Hardware Needed
- Raspberry PI 4/3b + (with fan and heatsink) - _Running Node-red, face detection and influxdb._
- 1/4 Cmos 640X480 USB Camera with Collapsible Cable for Raspberry Pi 
- Arduino nano - _serial input from weight sensors._
- ESP8266 (NodeMCU) - _RFID and controlling servo for entrance and exit._
- ESP32 Cam - _QR Code Scanning for entrance and exit._ 
- PN532 NFC RFID Read / Write Module V3 Kit
- HX711 Dual-Channel 24 Bit Precision A/D weight Pressure Sensor
- PIR Motion Sensor Detector Module HC-SR501
- TowerPro MG90S Mini Digital Servo Motor
- Weighing Load Cell Sensor 5kg YZC-131 With Wires
- 2x 0.96 Inch I2C/IIC 4pin OLED Display Module

> [!NOTE]
> It is adviced to use Raspberry pi 4 or above as the whole system is hosted on it.
> You can use any RFID module but the code for it will need to be adjusted.
> For turnstiles at Entrance and Exit you can use a higher capacity servo.
> It is adviced to use a higher capacity Load cell if you are going to use heavy weight.

## Installing Node-Red and influxDB
**Step 1:** Connect and open a terminal in Raspberry pi over VNC or Ignore this step if you are using Raspberry PI directly over monitor.

>***Note: If you Don't know how to enable VNC over ssh  follow the steps [here](https://www.raspberrypi.org/documentation/remote-access/vnc/)*** 

**Step 2:** Update your Raspberry PI.
```
sudo apt-get update && sudo apt-get upgrade
```
**Step 3:** Make sure you have latest Node-Red Installed on your Raspberry pi.
```bash
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```
**Step 4:** enable node red on startup.
```
sudo systemctl enable nodered.service
```
**Step 5:** Add the InfluxDB key.
```
curl https://repos.influxdata.com/influxdata-archive.key | gpg --dearmor | sudo tee/usr/share/keyrings/influxdb-archive-keyring.gpg >/dev/null
```
**Step 6:** Add its repository to the sources list.
```
echo "deb [signed-by=/usr/share/keyrings/influxdb-archive-keyring.gpg] https://repos.influxdata.com/debian stable main" | sudo tee /etc/apt/sources.list.d/influxdb.list
```
**Step 7:** Install Influxdb.
```
sudo apt update && sudo apt install influxdb
```
**Step 8:** Enable Influx on startup.
```
sudo systemctl unmask influxdb
sudo systemctl enable influxdb
sudo systemctl start influxdb
```
## Installing OpenCV and Face Detection
**Step1**: Make sure you have PI camera installed.
```
pip install picamera[array]
```
**Step 2**: Update your Raspberry PI.
```
sudo apt-get update && sudo apt-get upgrade
```
**Step 3**: Install necessary Prerequisites
```
sudo apt-get install build-essential cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 python3-pyqt5 python3-dev -y
```
> Note: If you encounter any issue with any library, ignore it but make sure to run this command atleast once. 

**Step 4**: Install OpenCV with pip.
```
pip install opencv-python
```
> Note: If you still get an error message such as Could not find a version that satisfies the requirement opencv-contrib-python (from versions: ) No matching distribution found for opencv-contrib-python, try the alternative to use apt-get instead of pip.
```
sudo apt-get install python-opencv
```
**Step 5** 
```
pip install imutils
```
**Step 6**: Install Face Detection.
``` 
pip install face-recognition --no-cache-dir
```
**Step 7**: Clone carolinedunn github repo.
```
git clone https://github.com/carolinedunn/facial_recognition
```