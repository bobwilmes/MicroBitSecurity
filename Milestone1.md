# MicroBitSecurity

##	Executive Summary

**CYBR 8080 Project Milestone 1 - Oct 8, 2019 - Bob Wilmes**

The purpose of this project is to demonstrate a method of secure authentication between a [Google Pixel 3 Android cell phone](https://en.wikipedia.org/wiki/Pixel_3) and a [BBC Micro Bit embedded device](https://en.wikipedia.org/wiki/Micro_Bit) representing an "Internet of Things" or IoT device.  The Android device runs a native application which connects to the IoT device using Bluetooth.  The application sends a seed pattern over a secure communications link which causes the Micro Bit to display a pattern in a 5x5 matrix of LED's on the device. Authentication is performed when a user correctly maps the pattern of LED's which is lit on the Android display. 

The impetus for this project is the California IoT law ([SB327: Information Privacy - Connected Devices](https://leginfo.legislature.ca.gov/faces/billNavClient.xhtml?bill_id=201720180SB327)) which requires that each IoT device be uniquely identified and have a secure mechanism which requires the password to be changed upon first use. This law becomes effective January 1, 2020.  The ability to authenticate using a visual pattern of LED's demonstrates a new capability which is believed will satisfy the complexity for the authentication 

The Bluetooth communications between the Pixel 3 cell phone and the Micro Bit IoT device will be performed over a GATT profile using Bluetooth which requires pairing between the Pixel 3 and the Micro Bit device.  This is performed by resetting the hardware on the Micro Bit device and initiating a Bluetooth scan on the Pixel 3. The user then picks the Micro Bit device from any listed on the scan from the Pixel 3.  Once the initial communication is started, the Micro Bit will generate
a six digit numeric code which is entered into the Android app to complete the communications pairing. 

The Micro Bit has a panel with a 5 by 5 element LED display. The authentication protocol will randomly generate a 25 bit number which will indicate which combination of LED's on the display will be lit. That number will be sent over a secure channel to the Micro Bit device and the LED's will lite. The end user will then use a finger tip to select the LED's as displayed on the Pixel 3 mobile device.  If selected correctly, the Pixel 3 will send an unlock signal to the MicroBit and display a check mark to indicate authentication success. If the authentication ws unsuccessful or communications is lost, the Micro Bit will display an "X" character on the 5 by 5 LED panel.

|                |[Google Pixel 3](https://store.google.com/product/pixel_3_specs)                 |[BBC Micro:Bit](https://tech.microbit.org/hardware/)                |
|----------------|-------------------------------|-----------------------------|
|Operating System|Android 9.0 Pie                |Micro:Bit DAL on ARM mbed    |
|CPU             |Qualcomm Snapdron 845          |Nordic nRF 51822 Cortex M0   |
|Bluetooth       |Bluetooth 5.0 + LE             |Bluetooth 4.1 + LE          |


## Project Goals

 1. Create a Android native app that can successfully communicate using Bluetooth to the Micro Bit
 2. Create a C++ Micro:Bit application from the Micro Bit ARM operating system
 3. Demonstrate the successful Bluetooth pairing between the Pixel 3 and the Micro Bit
 4. Demonstrate the generation of the secure LED pattern on the Micro Bit
 5. Demonstrate the capture of the secure LED pattern on the Pixel 3
 6. Demonstrate the success (check mark) or failure ( X character) on the Micro Bit

## Summary

By demonstrating the use of a pattern of LED's on the Micro Bit device, this technique could be expanded for other uses of cues for visual authentication. For example, the camera on the Pixel 3 mobile phone could be programmed to read the pattern of LED's, thereby removing potential errors in the process of capturing the on/off status on the Android application. This would potentially be a novel way for IoT devices to securely authenticate if they lacked keyboards or other sensory inputs.

## Images
![Google Pixel 3](https://images-na.ssl-images-amazon.com/images/I/71JvslYorPL._AC_SL1500_.jpg)
Picture 1 - Google Pixel 3 mobile phone


![BBC Micro:Bit IoT Device](https://images-na.ssl-images-amazon.com/images/I/515+1Wrp++L._AC_.jpg)
Picture 2 - BBC Micro:Bit IoT Device

![BBC Micro:Bit LED Array](https://os.mbed.com/media/uploads/JonnyA/microbit_platform_image_2.png)
Picture 3 - BBC Micro:Bit IoT Device - LED array
