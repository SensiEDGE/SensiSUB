-------------------------------------------------------------------------------
-                    (C) COPYRIGHT 2016 STMicroelectronics                    -
- File:    FP-SNS-6LPNODE1_readme.txt                                         -
- Author:  ST Central Labs                                                    -
- Date:    13-Feb-2017                                                        -
- Version: V1.1.0                                                             -

-----------
FP-SNS-6LPNODE1_readme.txt
-----------
FP-SNS-6LPNODE1 is an STM32 ODE Function Pack. Thanks to this package you can connect your IoT node to a 6LoWPAN Wireless Sensors Network and expose the sensors and actuator resources using standard application layer protocols. This package allows jumpstarting IoT development so you can save time in the integration of the different basic functions.  
This software package provides sample applications that allow RESTful access to the sensors and actuators resources on the IoT node by the means of the OMA Lightweight M2M (LWM2M) standard, using the IPSO Smart Objects Guidelines for data representation. 

Please note that under Windows, there may be problems related to the files path being too long, in this case put the package as close to root (c:\) as possible, if still needed shorten the name of the base folder.

For instructions on how to use the sample applications, see the files: 
 * Projects\Multi\Applications\ipso-flightsense\readme.txt
 * Projects\Multi\Applications\ipso-mems\readme.txt
 * Projects\Multi\Applications\ipso-nosensors\readme.txt

WARNING: When rebuilding the projects, please leave the optimization level that comes with the original project to avoid runtime problems (seen with IAR and Keil, under investigation).

---------
Contents
---------
This package contains:
 * FP-SNS-6LPNODE1_readme.txt ................ This readme.txt
   
 * Documentation\FP-SNS-6LPNODE1.chm ...... Documentation of the source code included in this package

 * Drivers\BSP\X_NUCLEO_IKS01A1
   ........................... BSP drivers for the X-NUCLEO-IKS01A1 mems expansion board

 * Drivers\BSP\X_NUCLEO_IKS01A2
   ........................... BSP drivers for the X-NUCLEO-IKS01A2 mems expansion board

 * Drivers\BSP\X-NUCLEO-6180XA1
   ........................... BSP drivers for the X-NUCLEO-6180XA1 flighstense expansion board

 * Drivers\BSP\X-NUCLEO-IDS01Ax
   ........................... BSP drivers for the X-NUCLEO-IDS01Ax SPIRIT1 expansion board

 * Drivers\BSP\Components\spirit1
   ........................... Component drivers for the SPSGRF-868 (or SPSGRF-915) SPIRIT1 sub-1GHz radio

 * Middlewares\ST\STM32_Contiki_Library
   ........................... Porting of the Contiki OS to the STM32 platform

 * Middlewares\Third_Party\Contiki
   ........................... Contiki OS v3.x (see the release note for detailed information about the version)
   
 * Projects\Multi\Applications\
   * ipso-flightsense  ............ Use it to create a IoT node that, other than the base ones, exposes a IPSO Presence Object that can be accessed over a 6LoWPAN network using the LWM2M protocol.  See specific readme.txt for details. 
   * ipso-mems  ............ Use it to create a IoT node that, other than the base ones, exposes enviromental IPSO Objects that can be accessed over a 6LoWPAN network using the LWM2M protocol. See specific readme.txt for details.
   * ipso-nosensors  ............ Use it to create a IoT node that exposes basic OMA LWM2M Objects along with IPSO Button and IPSO Light Control Objects. These can be accessed over a 6LoWPAN network using the LWM2M protocol. See specific readme.txt for details.

_______________________________________________________________________________
- (C) COPYRIGHT 2016 STMicroelectronics                   ****END OF FILE**** -
