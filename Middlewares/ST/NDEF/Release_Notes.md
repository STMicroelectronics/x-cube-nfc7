::: {.row}
::: {.col-sm-12 .col-lg-4}
[Release Notes for]{.small} **NDEF Library**
============================================

Copyright © 2022 STMicroelectronics\

[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}

Purpose
=======

The **NDEF Library** is a software implementation to manage NDEF message
format, as defined by the NFC Forum association. This Library is
compatible with all STM32 devices based on Cortex-M0, Cortex-M3 or
Cortex-M4 cores.
:::

::: {.col-sm-12 .col-lg-8}
Update History
==============

::: {.collapse}
V1.6.0 / 11-March-2024

<div>

Main Changes
------------

-   Update vCard type to store data \>255 bytes (for photo)
-   Update T5T DTA pattern numbers
-   Fix warnings and errors reported by Coverity for type T4T, T5T and
    Wifi record encoding
-   Refine error reported by ndefT5TPollerWriteBytes()
-   Added tests (vCard, empty record)
-   Examples compiled and tested with RFAL v2.10.0

</div>
:::

::: {.collapse}
V1.5.0 / 16-March-2023

<div>

Main Changes
------------

-   Update T2T, T3T, T5T format procedures
-   Update T3T NDEF detect procedure
-   Allow T3T detect procedure to accept RO flag or any RFU, according
    to specification
-   Set T5T\'s context state only when NDEF detect succeeds
-   Optimize writing TLV sequence, skip the \'T\' byte since it remains
    the same
-   Use ndefStatus return type
-   Fix compliancy with WLC 2.0 11.1.1.1
-   Examples compiled and tested with RFAL v2.8.0

</div>
:::

::: {.collapse}
V1.3.3 / 01-December-2022

<div>

Main Changes
------------

-   Bug fix

</div>
:::

::: {.collapse}
V1.3.2 / 28-February-2022

<div>

Main Changes
------------

-   Neutral license added
-   Added LICENSE files

</div>
:::

::: {.collapse}
V1.3.1 / 16-April-2019

<div>

Main Changes
------------

-   Removed nfc04a1\_nfctag.h header from template file for custom board

</div>
:::

::: {.collapse}
V1.3.0 / 13-February-2019

<div>

Main Changes
------------

-   Added a template file for BSP specs alignment.
-   Changed the JPEG define to VCARD\_JPEG.

</div>
:::

::: {.collapse}
V1.2.2 / 08-January-2018

<div>

Main Changes
------------

-   Fix Identify NDEF ID Length position.

</div>
:::

::: {.collapse}
V1.2.1 / 17-November-2016

<div>

Main Changes
------------

-   Fix Bluetooth Localname end string character position.

</div>
:::

::: {.collapse}
V1.2.0 / 17-November-2016

<div>

Main Changes
------------

-   Added read Text record support.
-   Changed Authentication / Encryption none to 0 values for Android
    compliancy (wps defines 1).
-   Bug fix.

</div>
:::

::: {.collapse}
V1.1.0 / 11-February-2016

<div>

Main Changes
------------

-   Update Tag Type 5 wrapper

</div>
:::

::: {.collapse}
V1.0.0 / 25-January-2016

<div>

Main Changes
------------

### First official release

</div>
:::
:::
:::

For complete documentation on **STM32** microcontrollers please visit:
[www.st.com](https://www.st.com/en/microcontrollers-microprocessors.html)

This release note uses up to date web standards and, for this reason,
should not be opened with Internet Explorer but preferably with popular
browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft
Edge.
