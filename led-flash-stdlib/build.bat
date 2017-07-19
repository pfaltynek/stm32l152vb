@echo off
SET makeconfig=%1

SETLOCAL enabledelayedexpansion

REM target file name definition
SET target=led-flash-stdlib

REM Prepare list of file extension for cfiles
SET cfilesexts=.c .cpp

REM Prepare list of file extension for asmfiles
SET asmfilesexts=.s

REM Prepare list of folders with all source files to compile
SET src_libs=./libs/std-periph-driver/src
SET src_utils=./utils/common/src ./utils/stm32l152_eval/src
SET srcfolders=./src %src_libs% %src_utils%

REM Prepare list of source files that can not be build
SET src_files_exclude=./src/kuk.c

REM Prepare include dirs
SET includes_libs=-I./libs/cmsis/inc -I./libs/cmsis/device/inc -I./libs/std-periph-driver/inc
SET includes_utils=-I./utils/common/inc -I./utils/stm32l152_eval/inc
SET includes=-I. -I./inc !includes_libs! !includes_utils!

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
) ELSE IF "%makeconfig%"=="release" (
	IF not exist obj/Release (
		mkdir obj\Release
	)
	IF not exist bin/Release (
		mkdir bin\Release
	)
	SET opt=-O1
	SET outpath=Release
)

SET optallcpp=-c -mthumb -mcpu=cortex-m3 -std=gnu11 -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -specs=nano.specs
SET optallasm=-c -mthumb -mcpu=cortex-m3 -Wa,--warn -x assembler-with-cpp
SET optlink=-lm -mthumb -mcpu=cortex-m3 -TSTM32L152VB_FLASH.ld -Wl,--start-group -lc -lm -Wl,--end-group -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=./lst/%target%.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000
SET defines=-DUSE_STM32L152_EVAL -DUSE_DEFAULT_TIMEOUT_CALLBACK -DSTM32L1XX_MD -DUSE_STDPERIPH_DRIVER
SET objects=

IF "%makeconfig%"=="clean" ( 
	CALL :Sub_Clean debug
	CALL :Sub_Clean release
) ELSE (
	CALL :Sub_Clean %makeconfig%

	REM clear source and assembler files lists
	SET cfiles=
	SET asmfiles=
	REM fill the sources and assembler lists
	FOR %%d in (%srcfolders%) DO (
		CALL :Get_Files %%d
	)

	REM compile all source and assember files
	FOR %%x in (!cfiles!) DO (
		REM get file name and extension from path
		echo %%~nxx
		REM prepare generated object path and file name
		SET outfile=./obj/%outpath%/%%~nxx.o
		REM compile source file
		arm-none-eabi-gcc %includes% %optallcpp% %opt% %defines% -o !outfile! %%x

		SET objects=!objects! !outfile!
	)

	FOR %%x in (!asmfiles!) DO (
 		REM get file name and extension from path
 		echo %%~nxx
		REM prepare generated object path and file name
 		SET outfile=./obj/%outpath%/%%~nxx.o
		REM compile assembler file
 		arm-none-eabi-gcc %optallasm% %opt% -o !outfile! %%x
 
 		SET objects=!objects! !outfile!
	)

	IF not exist ./lst/ (
		mkdir lst
	)
	arm-none-eabi-gcc !objects! -o ./bin/%outpath%/%target%.elf %optlink%
	arm-none-eabi-size ./bin/%outpath%/%target%.elf

)
EXIT /B

REM ---------------------------------------------------------------------------
REM Subroutine to clean Debug or Release folders based on parameter %1
REM ---------------------------------------------------------------------------
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

REM ---------------------------------------------------------------------------
REM Get all source files with paths of the folder sent as %1 parameter
REM ---------------------------------------------------------------------------
:Get_Files
REM here processed C & CPP source files into cfiles list
FOR %%f in (%cfilesexts%) DO (
	CALL :Get_Files_C %1 %%f
)
REM here processed ASM files into asmfiles list
FOR %%f in (%asmfilesexts%) DO (
	CALL :Get_Files_ASM %1 %%f
)
EXIT /B

REM ---------------------------------------------------------------------------
REM Get all C source files with paths of the folder sent as %1 parameter
REM Extension is sent as %2 parameter
REM ---------------------------------------------------------------------------
:Get_Files_C
FOR %%f in (%1/*%2) DO (
	SET excluded=0
	call :IsFileExcludedFromBuild %1/%%f
	IF !excluded!==0 (
		SET cfiles=!cfiles! %1/%%f
	)
)
EXIT /B

REM ---------------------------------------------------------------------------
REM Get all ASM source files with paths of the folder sent as %1 parameter
REM Extension is sent as %2 parameter
REM ---------------------------------------------------------------------------
:Get_Files_ASM
FOR %%f in (%1/*%2) DO (
	SET excluded=0
	call :IsFileExcludedFromBuild %1/%%f
	IF !excluded!==0 (
		SET asmfiles=!asmfiles! %1/%%f
	)
)
EXIT /B

REM ---------------------------------------------------------------------------
REM Check if file sent as %1 parameter has to be excluded from build
REM ---------------------------------------------------------------------------
:IsFileExcludedFromBuild
FOR %%f in (%src_files_exclude%) DO (
	IF %1==%%f (
		SET excluded=1
	)
)
EXIT /B
