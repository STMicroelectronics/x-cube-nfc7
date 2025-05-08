---
pagetitle: Release Notes for STM32F4xx-Nucleo Board Drivers
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>STM32F4xx-Nucleo Board Drivers</mark>
Copyright &copy; 2017 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

This directory contains the board drivers to demonstrate the capabilities of the STM32F4xx-Nucleo Board Drivers.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section13" checked aria-hidden="true">
<label for="collapse-section13" aria-hidden="true">V1.2.9 / 22-September-2023</label>
<div>			

## Main Changes

- Update Release_Notes.html to support new format

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section12" aria-hidden="true">
<label for="collapse-section12" aria-hidden="true">V1.2.8 / 11-Fabruary-2022</label>
<div>			

## Main Changes

- All source files: update disclaimer to add reference to the new license agreement

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section11" aria-hidden="true">
<label for="collapse-section11" aria-hidden="true">V1.2.7 / 25-September-2017</label>
<div>			

## Main Changes

- Add general description of BSP drivers
- Remove date & version

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section10" aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">V1.2.6 / 27-January-2017</label>
<div>			

## Main Changes

- Replace __PPP_CLK_ENABLE/DISABLE with __HAL_RCC_PPP_ENABLE/DISABLE
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">V1.2.5 / 06-May-2016</label>
<div>			

## Main Changes

- stm32f4xx_nucleo.c
  - BSP_JOY_GetState() alignment with HAL ADC driver V1.4.4
  
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">V1.2.4 / 12-January-2016</label>
<div>			

## Main Changes

- General updates to fix doxygen errors
- Add STM32F4-Nucleo_BSP_User_Manual.chm file
- Add use of UNUSED(data) in LCD_IO_WriteMultipleData() to fix "Unused variable" warning with SW4STM32

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">V1.2.3 / 13-November-2015</label>
<div>			

## Main Changes

- stm32f4xx_nucleo.c
  - Add SD_IO_CSSState(), SD_IO_WriteReadData() and SD_IO_WriteReadData() APIs for proper support of the Adafruit shield V2.0.1
  - Add new API for Button, LED and Joystick deinitialization: BSP_PB_DeInit(), BSP_LED_DeInit(), BSP_JOY_DeInit() 
  - Replace static APIs SPIx_Read(), SD_IO_ReadByte(), SD_IO_WriteCmd(), SD_IO_WaitResponse() and SD_IO_WriteDummy() by SD_IO_WriteReadData() API.
- stm32f4xx_nucleo.h
  - Add new define "ADAFRUIT_TFT_JOY_SD_ID802" for the Adafruit TFT shield support.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">V1.2.2 / 14-August-2015</label>
<div>			

## Main Changes

- stm32f4xx_nucleo.h
  - Add alias to BUTTON_USER for backward compatibility.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">V1.2.1 / 02-March-2015</label>
<div>			

## Main Changes

- stm32f4xx_nucleo.c/.h
  - Align to STM32F4xx HAL Driver V1.3.0 for __HAL_RCC_PPP_CLK_ENABLE() .

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">V1.2.0 / 26-December-2014</label>
<div>			

## Main Changes

- stm32f4xx_nucleo.c
  - Add LCD_IO_WriteMultipleData() LCD IO function, needed for ST7735 BSP component driver.
- Note: This driver version V1.2.0 needs ST7735 BSP component driver V1.1.0 and later.

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">V1.1.1 / 10-December-2014</label>
<div>

## Main Changes

- stm32f4xx_nucleo.c
  - BSP_BP_Init(): Fix limitation on button pin configuration (change EXTI polarity from rising to falling)


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">V1.1.0 / 19-June-2014</label>
<div>

## Main Changes

- stm32f4xx_nucleo.c/.h
  - Update driver to support LCD, joystick and microSD available on Adafruit 1.8" shield (reference ID 802) by adding the SPI and ADC IO link operations
  - Enhance BSP_PB_Init() function by removing the call of __SYSCFG_CLK_ENABLE() already enabled in the HAL_GPIO_Init()
  - Correct BSP_LED_Off() and BSP_LED_On() by inverting GPIO_PIN_RESET/ GPIO_PIN_SET values written on GPIO pins
  - Comments clean up and typo corrections

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">V1.0.0 / 18-February-2014</label>
<div>

## Main Changes

- First official release

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
