# Precision temperature sensor calibration tool for openFFUcontrol PTS
A Desktop application to perform calibration of temperature sensors for
openFFUcontrol. The software interfaces to a master sensor connected to an
HP/Agilent scanner with RTD measurement card as well as an openFFUcontrol
PTS as secondary high precision master sensor.
Values of reference and test resistors can be set as well as calibration
offset. At the end of the calibration process a calibration certificate is
created as pdf.

## Building and installing
First make sure to have Qt5 and libmodbus-devel installed on your system.
Create a directory for the build
```
mkdir bin
```

Create the Makefile
```
cd bin
qmake ../src
```

Compile the application wih a number of concurrent jobs of your choice

```
make -j 8
```

Run the application
```
./openffucontrol-calibration
```

Connect the Agilent Scanner to ttyS0 and a modbus interface to ttyUSB0.
Power up the sensors with an openFFUcontrol OCU and connect the modbus
interface to the sensors. Place sensors in a high accuracy water filled
dewar flask and wait for stabilization. A magnetic stirrer should be used in
the dewar. The calibration should be performed as near as possible to the
operation point.
