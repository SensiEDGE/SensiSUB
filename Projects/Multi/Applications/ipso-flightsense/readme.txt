-------------------------------------------------------------------------------
-                    (C) COPYRIGHT 2016 STMicroelectronics                    -
- File:    readme.txt                                                         -
- Author:  ST Central Labs                                                    -
- Date:    13-Feb-2017                                                        -
- Version: V1.1.0                                                             -

-----------
ipso-flightsense/readme.txt
-----------
This application implements a IPSO Smart Object Profile (OMA LWM2M client) for a 6LoWPAN network, using the OMA LWM2M/CoRE CoAP
protocol suite to connect to a OMA LWM2M Server.
Implemented Objects are:

 * Server
 * Device
 * IPSO Digital Input (NUCLEO board User Button)
 * IPSO Digital Output (a pin of the NUCLEO)
 * IPSO Presence Sensor
 * IPSO Light Control (NUCLEO board green LED and X-NUCLEO-IDS01Ax board red LED)

The code has been developed around the OMA LWM2M and IPSO Smart Object implementations available in Contiki OS.
Relevant code can be found in these folders:
Middlewares/Third_Party/Contiki/apps/oma-lwm2m
Middlewares/Third_Party/Contiki/apps/ipso-objects

Most of the interested files have been modified, keep this in mind if you need to upgrade the Contiki OS libraries.

The basic functionalities of the OMA LWM2M have been extended (mainly lwm2m-engine.c file) to address Registration Update, Block Wise registering (in order to be able to register a larger number of objects on the server).

For this application, the IPSO Digital Output and IPSO Presence Sensor objects have been added, their implementations are in
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-digital-output.c
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-presence.c

In the Middlewares/Third_Party/Contiki/apps/* folders there is the core implementation of the interested functionality.
Then, each interested object (in OMA LWM2M terminology) must have an instance in the Application folder (or in the Contiki_STM32_Library/dev folder for standard resources like button and LEDs).

The per-project settings, including the endpoint name and the enabled objects, can be found (as per Contiki OS convention) in:
Projects/Multi/Applications/ipso-flightsense/Inc/project-conf.h
file.
For this specific application, note that the COAP_OBSERVE_REFRESH_INTERVAL is set to 1, this means that each notification sent to 
the observer(s) of a specific resource is marked as CONFIRMABLE (to prevent the loss of Presence Sensor event). The default value in Contiki OS/Erbium framework is 20.


-----------
IPSO Digital Input
-----------
Core file:
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-button.c
Instance file:
Middlewares/ST/Contiki_STM32_Library/dev/button-sensor.c

It maps the NUCLEO User Button.
Observable resources are
Digital Input State
Digital Input Counter

User can modify the behavior of this objects changing
Digital Input Polarity
Digital Input Debounce
Digital Input Edge Selection
please refer to IPSO Smart Objects guidelines for details.


-----------
IPSO Digital Output
-----------
Core file:
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-digital-output.c
Instance file:
Projects/STM32F401RE-Nucleo/Applications/ipso-flightsense/Src/res-digital-output.c

The PIN that can be set/unset using the Digital Output object can be selected in res-digital-output.c file by the macros
#define CONTROLLED_PORT GPIOA
#define CONTROLLED_PIN	GPIO_PIN_5


-----------
IPSO Presence Sensor
-----------
Core file:
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-presence.c
Instance file:
Projects/Multi/Applications/ipso-flightsense/Src/res-presence.c
Effective sensor reading:
Middlewares/ST/Contiki_STM32_Library/dev/presence-sensor.c

Observable resource is
Digital Input State

The range of the Presence Sensor can be set in 
Projects/Multi/Applications/ipso-flightsense/Inc/my_vl6180x.h
file by the macros
#define PRESENCE_LOW_TH 20
#define PRESENCE_HIGH_TH 25

The default behavior of this object is to work in polling mode, by setting in the project properties
PRESENCE_PERIODIC=1
In this case, a thread will check the status of the presence sensor every PRESENCE_TIMER seconds: this defaults to 1 second and
can be changed in ipso-presence.c file.

Clear To Busy and Busy to Clear delays are implemented as resource but currently do not have any effect on the behavior of the object.
When a presence is detected, the GREEN led is switched ON.


-----------
IPSO Light Control
-----------
Core file:
Middlewares/Third_Party/Contiki/apps/ipso-objects/ipso-leds-control.c
Instance file:
Middlewares/ST/Contiki_STM32_Library/dev/leds-arch.c

It maps the available LEDs, for this application they are two:
GREEN one on the NUCLEO
RED one on the X-NUCLEO-IDS01Ax

Clarification: mind that the core file is actually ipso-leds-control.c and the name of the object is IPSO Light Control. There is another file (ipso-light-control.c) that is NOT used in this case.


-----------
Board Settings 
-----------
Be sure to select the right X-NUCLEO-IDS01A4 or X-NUCLEO-IDS01A5 board in the code by setting the symbol X_NUCLEO_IDS01A4 for the 868MHz bandwidth, and X_NUCLEO_IDS01A5 for the 915MHz bandwidth.

-----------
Hardware and Software environment
-----------
  - This example runs on STM32 Nucleo devices with X-NUCLEO-IDS01A4 (or X-NUCLEO-IDS01A5) stacked below a X-NUCLEO-6180XA1 expansion board.
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board

-----------
How to use it?
-----------    
In order to make the program work, you must do the following:

 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
   
 - WARNING: be sure to grant connectivity towards a OMA LWM2M server: this can be done using the 6LoWPAN WiFi-Bridge provided in the FP-NET-6LPWIFI1 Function Pack, or with a Border Router application available using the osxContiki6LP package along with X-CUBE-SUBG1, or compiling the rpl-border-router example of Contiki.
This sample application has been tested with Leshan (a Java implementation of a LWM2M server, up to vers. 0.1.11-M13).
 
 - Open IAR toolchain and compile the project (see the release note for detailed information about the version).
   Alternatively you can use the Keil uVision toolchain (see the release note for detailed information about the version).
   Alternatively you can use the System Workbench for STM32 (see the release note for detailed information about the version).
 - Program the firmware on the STM32 Nucleo board: you can copy (or drag and drop) the binary file to the USB mass storage location created when you plug the STM32Nucleo board to your PC. If the host is a Linux PC, the STM32 Nucleo F4 device can be found in the /media folder with the name "NUCLEO_F401RE". For example, if the created mass storage location is "/media/NUCLEO_F401RE", then the command to program the board with a binary file named "my_firmware.bin" is simply: cp my_firmware.bin /media/NUCLEO_F401RE. 
   Alternatively, you can program the STM32 Nucleo board directly through one of the supported development toolchains.
   
 - Alternatively, you can download the pre-built binaries in "Binary" folder included in the distributed package. 
   
 - KNOWN ISSUE: Please leave the optimization level that comes with the original project to avoid runtime problems (seen with IAR and Keil, under investigation).

 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.


-----------
Connectivity test
-----------
This sample application demonstrates how a node can connect to a server by the means of the OMA Lightweight M2M (LWM2M) standard. By default, the node will attempt to connect to a public online server (http://leshan.eclipse.org/), which is a Java implementation of a LWM2M server (see http://www.eclipse.org/leshan/ for further information).

In order to debug or to follow the program flow:
 - Open a serial line monitor utility, select the serial port name to which the board is connected and configure it thus:
   + Baud Rate = 115200
   + Parity = None
   + Bits = 8
   + Stopbits = 1
 - The node prints its self generated Client Endpoint.
 - Wait for the node to complete the registration (i.e. it prints "Successfully registered").
 - Open your web browser and insert the URL of the public Leshan server (http://leshan.eclipse.org) if the default use case is implemented, alternatively insert the URL of the local Leshan server.
 - Click on the corresponding Client Endpoint.
 - Interact with the node Resources by means of the buttons provided by Leshan GUI, for example press "Read" to get the actual value of a resource, or press "Observe" to be asynchronously notified (pariodically and/or on sensor value change) about the value of a specific resource.

_______________________________________________________________________________
- (C) COPYRIGHT 2016 STMicroelectronics                   ****END OF FILE**** -
