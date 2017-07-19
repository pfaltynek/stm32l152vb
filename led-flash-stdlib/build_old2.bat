@echo off
SET makeconfig=%1

SETLOCAL enabledelayedexpansion

SET target=led-flash-stdlib
SET cfiles_src=./src/main.c ./src/stm32l1xx_it.c ./src/syscalls.c ./src/system_stm32l1xx.c ./src/tiny_printf.c
SET cfiles_libs=./libs/std-periph-driver/src/misc.c ./libs/std-periph-driver/src/stm32l1xx_adc.c ./libs/std-periph-driver/src/stm32l1xx_aes.c ./libs/std-periph-driver/src/stm32l1xx_aes_util.c ./libs/std-periph-driver/src/stm32l1xx_comp.c ./libs/std-periph-driver/src/stm32l1xx_crc.c ./libs/std-periph-driver/src/stm32l1xx_dac.c ./libs/std-periph-driver/src/stm32l1xx_dbgmcu.c ./libs/std-periph-driver/src/stm32l1xx_dma.c ./libs/std-periph-driver/src/stm32l1xx_exti.c ./libs/std-periph-driver/src/stm32l1xx_flash.c ./libs/std-periph-driver/src/stm32l1xx_flash_ramfunc.c ./libs/std-periph-driver/src/stm32l1xx_fsmc.c ./libs/std-periph-driver/src/stm32l1xx_gpio.c ./libs/std-periph-driver/src/stm32l1xx_i2c.c ./libs/std-periph-driver/src/stm32l1xx_iwdg.c ./libs/std-periph-driver/src/stm32l1xx_lcd.c ./libs/std-periph-driver/src/stm32l1xx_opamp.c ./libs/std-periph-driver/src/stm32l1xx_pwr.c ./libs/std-periph-driver/src/stm32l1xx_rcc.c ./libs/std-periph-driver/src/stm32l1xx_rtc.c ./libs/std-periph-driver/src/stm32l1xx_sdio.c ./libs/std-periph-driver/src/stm32l1xx_spi.c ./libs/std-periph-driver/src/stm32l1xx_syscfg.c ./libs/std-periph-driver/src/stm32l1xx_tim.c ./libs/std-periph-driver/src/stm32l1xx_usart.c ./libs/std-periph-driver/src/stm32l1xx_wwdg.c 
SET cfiles_utils=./utils/common/src/fonts.c ./utils/common/src/lcd_log.c ./utils/stm32l152_eval/src/stm32l152_eval.c ./utils/stm32l152_eval/src/stm32l152_eval_glass_lcd.c ./utils/stm32l152_eval/src/stm32l152_eval_i2c_ee.c ./utils/stm32l152_eval/src/stm32l152_eval_i2c_tsensor.c ./utils/stm32l152_eval/src/stm32l152_eval_lcd.c ./utils/stm32l152_eval/src/stm32l152_eval_spi_sd.c

SET cfiles=!cfiles_src! !cfiles_libs! !cfiles_utils!

SET includes_libs=-I./libs/cmsis/inc -I./libs/cmsis/device/inc -I./libs/std-periph-driver/inc
SET includes_utils=-I./utils/common/inc -I./utils/stm32l152_eval/inc

SET includes=-I. -I./inc !includes_libs! !includes_utils!

SET asmfiles=./src/startup_stm32l1xx_md.s

IF "%makeconfig%"=="" ( SET makeconfig=release)

IF "%makeconfig%"=="debug" (
	IF not exist obj/Debug (
		mkdir obj\Debug
	)
	IF not exist bin/Debug (
		mkdir bin\Debug
	)
	SET opt=-g -O0
	SET outpath=Debug
)ELSE IF "%makeconfig%"=="release" (
	IF not exist obj/Release (
		mkdir obj\Release
	)
	IF not exist bin/Release (
		mkdir bin\Release
	)
	SET opt=-O1
	SET outpath=Release
)

SET optall=-c -mthumb -mcpu=cortex-m3 -std=gnu11 -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -specs=nano.specs
SET optallasm=-c -mthumb -mcpu=cortex-m3 -Wa,--warn -x assembler-with-cpp
SET optlinker=-lm -mthumb -mcpu=cortex-m3 -TSTM32L152VB_FLASH.ld -Wl,--start-group -lc -lm -Wl,--end-group -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=./lst/%target%.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80
SET defines=-DUSE_STM32L152_EVAL -DUSE_DEFAULT_TIMEOUT_CALLBACK -DSTM32L1XX_MD -DUSE_STDPERIPH_DRIVER
SET objects=

IF "%makeconfig%"=="clean" ( 
	CALL :Sub_Clean debug
	CALL :Sub_Clean release
) ELSE (
	CALL :Sub_Clean %makeconfig%

	FOR %%x in (%cfiles%) DO (
		REM get file name and extension from path
		echo %%~nxx
		SET outfile=./obj/%outpath%/%%~nxx.o
		arm-none-eabi-gcc %includes% %optall% %opt% %defines% -o !outfile! %%x

		SET objects=!objects! !outfile!
	)

	FOR %%x in (%asmfiles%) DO (
 		REM get file name and extension from path
 		echo %%~nxx
 		SET outfile=./obj/%outpath%/%%~nxx.o
 		arm-none-eabi-gcc %optallasm% %opt% -o !outfile! %%x
 
 		SET objects=!objects! !outfile!
	)

	IF not exist ./lst/ (
		mkdir lst
	)
	arm-none-eabi-gcc -o ./bin/%outpath%/%target%.elf !objects! %optlinker%
	arm-none-eabi-size ./bin/%outpath%/%target%.elf

)
EXIT /B

:Sub_Clean
IF "%1"=="debug" (
	IF exist ./bin/Debug (
		del .\bin\Debug\*.* /q
	)
	IF exist ./obj/Debug (
		del .\obj\Debug\*.* /q
	)
)

IF "%1"=="release" (
	IF exist ./bin/Release (
		del .\bin\Release\*.* /q
	)
	IF exist ./obj/Release (
		del .\obj\Release\*.* /q
	)
)

EXIT /B

