# Fingerprint
Arduino-based fingerprint sensor that opens an Olin College dorm door (may work on other doors too).

This repository contains the general directions, all code and electronics, and some of the mechanical parts you will need. However, you will find that you need to design your own mounts for the servo and the fingerprint sensor.

<img src="https://github.com/vrhinehart/fingerprint/blob/main/images/exterior.JPG" height="400"> <img src="https://github.com/vrhinehart/fingerprint/blob/main/images/interior.JPG" height="400">

Parts required:
- Arduino Nano or Uno
- ID809 fingerprint sensor [DFRobot](https://www.dfrobot.com/product-2051.html) [Amazon](https://www.amazon.com/DFROBOT-Capacitive-Fingerprint-Scanner-Sensor/dp/B08JBRKB9T)
- Breadboard
- Pushbutton switch
- LED (I used red)
- High torque hobby servo (I used MG996R)
- Material for servo arm and servo mount (wood, metal, plastic)
- 5V power supply (I used cut off USB cable)
- String or Paracord
- Zip Ties
- Double sided tape (I used 3M VHB)
- Duct tape :)

Steps:
- Assemble the breadboard according to the schematic.
- 3D print, laser cut, or otherwise make the servo arm according to the included .STL or .DXF (mm) file.
- Attach the arm to the servo using the 4-arm control horn and zip ties. Wait to put in the screw.
- Mount the servo to the door as shown in the images. You can use a block of wood or whatever you want.
- Attach the fingerprint sensor and Arduino to the door as you see fit.
- Wire everything up as shown in the schematic. Use the 5V power supply for the Arduino, wired from the nearest power source as needed.
- Power the Arduino and let the servo go to its home position.
- Adjust the position of the arm so that its angle matches the pictures. Put in the servo screw.
- Install the string through the hole in the arm and duct tape the other end around the doorhandle. Tension it so that the handle is just barely at rest and there is tension on the string.

Tips:
- If the fingerprint sensor is not detected over serial, the debug LED on the Arduino will flash.
- Disconnect the Arduino from the fingerprint sensor when flashing, as the USB and sensor use the same UART.
- If the sensor LED turns cyan, it's waiting for a fingerprint. This means it was triggered to enter the verification mode, but the finger was removed. Scan a finger to exit this mode.
- You may want to cover the debug LEDs on the Arduino with tape. Otherwise they will shine in the night and possibly be confused with the "unlocked" indicator.

Interface:
- By default, device is in fingerprint reading mode. Place your finger on the sensor to authenticate.
- When you place your finger, the servo will always move in preparation to open the door. If rejected, it will move back.
- You can put the door in "unlocked" mode by holding the button until the unlock LED illuminates. Then any fingerprint will be accepted. Turn off with another long press.
- Enter the menu by short-pressing the button (<1sec). The sensor LED will flash which slot you are on. Press the button again to change slot.
- Press the button until the LED turns blue to record a print to the selected slot. The LED will flash to confirm the slot number, then turn blue. Place your finger to record. Red indicates failure, try again.
- Press the button until the LED turns blue, then white to delete the print from the slot. The LED will breathe white to confirm deletion.

Please feel free to contact me if you want help implementing this. My code has no warranty attached, but I'd be interested to hear from you anyway.
