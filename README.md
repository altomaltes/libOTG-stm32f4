# libOTG-stm32f4
[Embitz](https://www.embitz.org/) Interrupt driven USB stack for stm32f4: host, device and OTG 

## Description:
This is a bare metal zero dependencies USB OTG stack for stm32f4 uc's
The used IDE is lightweigth EMBITZ, so porting to other IDE's must be easy.
There is working examples for three cheap ST boards.
* STM32F429 Discovery. ( device, host and otg )
* STM32F4 Discovery, based on stm32F407. ( device, host and otg )
* STEVAL-3DP001V1, A 3D printer controlled based on stm32f401. ( device and host )

## Features:
* Same ld linker file for all uc's. Beware of the dead code remover trick on the ld script if you want to change it.
* No assembler code
* Back to old good LIBRARY.
* Both cores support
* Hub host support.
* Test code for serial CDC and HID ( device ) and Mass storage and keyboard for host

## Achiveable goals:
* Support for similar boards (STM32F7, they share same engine).
* Both cores simultaneous support.
* Add my STM32 device support fom many other uc's. (also Infineon XMC2xxx )
* Add more host an device drivers.  

## Other goals:
* Dual role OTG.
* DMA support.


