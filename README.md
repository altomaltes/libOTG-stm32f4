# libOTG-stm32f4
[Embitz](https://www.embitz.org/) Interrupt driven USB stack for stm32f4: host, device and OTG 

## Description:
This is a bare metal zero dependencies USB OTG stack for stm32f4 uc's
The used IDE is lightweigth EMBITZ, so porting to other IDE's must be easy.
There is working examples for three cheap ST boards.
* STM32F429 Discovery. ( device, host and otg )
* STM32F4 Discovery, based on stm32F407. ( device, host and otg )
* STEVAL-3DP001V1, A 3D printer controlled based on stm32f401. ( device and host )

## Goals achieved:
* Same ld linker file for all uc's.  Be carefull with the dead code remover trick on the ld script if you want to change it.
* No assembler code
* USE OF OLD GOOD LIBRARY.


