# Fingerprint
Arduino-based fingerprint sensor that opens an Olin College dorm door (may work on other doors too).

Parts required:
- Arduino Nano or Uno
- Breadboard, Pushbutton switch, LED
- High torque hobby servo (I used MG996R)
- Zip Ties
- Rubber band
- Material for servo arm and servo mount
- 5V power supply (I used cut off USB cable)
- Duct tape :)

Steps:
- Assemble the breadboard according to the schematic.
- Mount the servo to the door and attach the servo arm.
- Connect the arm with a string to the doorhandle.
- Connect a rubber band to the end slot and stretch it just shy of back-driving the servo (when the servo has power).
- Wire everything to the door and test.

Tips:
- If the fingerprint sensor is not detected over serial, the debug LED on the Arduino will flash.
- Disconnect the Arduino from the fingerprint sensor when flashing, as the USB and sensor use the same UART.

Interface:
- By default, device is in fingerprint reading mode. Place your finger on the sensor to authenticate.
- When you place your finger, the servo will always move in preparation to open the door. If rejected, it will move back.
- You can put the door in "unlocked" mode by holding the button until the unlock LED illuminates. Then any fingerprint will be accepted. Turn off with another long press.
- Enter the menu by short-pressing the button (<1sec). The sensor LED will flash which slot you are on. Press the button again to change slot.
- Press the button until the LED turns blue to record a print to the selected slot. The LED will flash to confirm the slot number, then turn blue. Place your finger to record. Red indicates failure, try again.
- Press the button until the LED turns blue, then white to delete the print from the slot. The LED will breathe white to confirm deletion.

Please feel free to contact me if you want help implementing this. My code has no warranty attached, but I'd be interested to hear from you anyway.
