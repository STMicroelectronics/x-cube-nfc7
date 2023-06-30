
## <b>EH application for dynamic NFC/RFID tag IC Expansion Board and STM32 Nucleo Boards</b>

This application contains an example which shows 
how to enable the energy harvesting using dynamic NFC/RFID tag IC

Example Description:

This example application shows how to enable the energy harvesting
------------------------------------
WARNING: When starting the project from Example Selector in STM32CubeMX and regenerating it
from ioc file, you may face a build issue. To solve it, assuming you started the project for the
Nucleo-L476RG board, remove from the IDE project the file stm32l4xx_nucleo.c in the Application/User
virtual folder and delete, from Src and Inc folders, the files: stm32l4xx_nucleo.c, stm32l4xx_nucleo.h,
stm32l4xx_nucleo_bus.c, stm32l4xx_nucleo_bus.h and stm32l4xx_nucleo_errno.h.
The same operations apply if you starts the project for another STM32 Nucleo board (e.g. for
Nucleo-L053R8, the files to be removed are stm32l0xx_nucleo.c, stm32l0xx_nucleo.c, stm32l0xx_nucleo.h,
stm32l0xx_nucleo_bus.c, stm32l0xx_nucleo_bus.h and stm32l0xx_nucleo_errno.h).
------------------------------------

Press the user button to enable the EH dynamic register and allow energy
harvesting until the chip is reset
- The EH dynamic is enabled and allows energy harvesting until the chip is reset.
- The blue LED is switched ON when the dynamic register is enabled

Press the user button for more than 2 seconds.
- The EH static register is enabled and allows energy harvesting by default each
  time the chip is powered.
- The green LED is switched ON when the static register is enabled


X-NUCLEO-NFC07A1 dynamic NFC/RFID tag IC expansion board is based on the ST25DV64KC
NFC Type V/RFID tag IC with a dual interface 64 Kbits EEPROM that also features an I2C interface. It
can be powered by the pin of Arduino connector or directly by the received carrier electromagnetic field

The ST25DV64KC features a low power down (LPD) pin, a general purpose output (GPO)
and an energy harvesting feature that lets you output the energy coming from the RF field
on the Vout analog pin and acts on ST1 jumper to enable it

### <b>Keywords</b>

NFC, NDEF, EEPROM, I2C, ST25DV64KC

### <b>Directory contents</b>

 - app_nfc7.c             NFC7 initialization and applicative code
 
 - main.c                 Main program body
 
 - stm32_bus_ex.c         Source file for GNSS1A1 Bus Extension
 
 - stm32**xx_hal_msp.c    Source code for MSP Initialization and de-Initialization

 - stm32**xx_it.c         Source code for interrupt Service Routines

 - stm32**xx_nucleo.c     Source file for the BSP Common driver 
						
 - stm32**xx_nucleo_bus.c Source file for the BSP BUS IO driver
 
 - system_stm32**xx.c     CMSIS Cortex-Mx Device Peripheral Access Layer System Source File

### <b>Hardware and Software environment</b>

 - This example runs on STM32 Nucleo devices with NFC7 expansion board
 - X-NUCLEO-NFC07A1
 
 - This example has been tested with STMicroelectronics:
 - NUCLEO-L053R8 RevC board
 - NUCLEO-L476RG RevC board
 - NUCLEO-F401RE RevC board
 and can be easily tailored to any other supported device and development board.
 
 - For NDEF_URI application demonstration One NFC7 Expansion Board + 
   STM32 Nucleo is programmed as node 
 - NDEF_URI application can be used for dynamic NFC/RFID tag IC Expansion Board

ADDITIONAL_BOARD : X-NUCLEO-NFC07A1 https://www.st.com/en/ecosystems/x-nucleo-nfc07a1.html
ADDITIONAL_COMP : ST25DV64KC https://www.st.com/en/nfc/st25dv64kc.html
  
### <b>How to use it?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - < Add any other step(s) to be done by the user to make the example working >

### <b>Author</b>

SRA Application Team

### <b>License</b>

Copyright (c) 2022 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
