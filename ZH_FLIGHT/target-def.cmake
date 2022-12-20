set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

# find_program(ARM_SIZE_EXECUTABLE arm-none-eabi-size)
# find_program(ARM_GDB_EXECUTABLE arm-none-eabi-gdb)
# find_program(ARM_OBJCOPY_EXECUTABLE arm-none-eabi-objcopy)
# find_program(ARM_OBJDUMP_EXECUTABLE arm-none-eabi-objdump)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(MCU_FAMILY STM32H7B0xx)
set(MCU_HAL USE_HAL_DRIVER)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7B0VBTx_FLASH.ld)
#内核相关
set(CPU "-mcpu=cortex-m7")
set(FPU "-mfpu=fpv5-d16")
set(FLOAT_ABI "-mfloat-abi=hard")

add_definitions(
    -D${MCU_FAMILY}
    -D${MCU_HAL}
)

# generate flags from user variables
set(OPT "-Og")
if(CMAKE_BUILD_TYPE MATCHES Debug)
set(DBG_FLAGS "-g -gdwarf-2")
endif()

set(MCU "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")

set(CMAKE_C_FLAGS "${MCU} ${OPT} -Wall -fdata-sections -ffunction-sections ${DBG_FLAGS} " CACHE INTERNAL "C compiler flags")
#CPP 
set(CMAKE_CXX_FLAGS "${MCU} ${OPT} -fno-rtti -fno-exceptions -fno-builtin -Wall -fdata-sections -ffunction-sections  ${DBG_FLAGS} " CACHE INTERNAL "Cxx compiler flags")
#set(CMAKE_CXX_FLAGS "${MCU} ${OPT} -Wall -fdata-sections -ffunction-sections ${DBG_FLAGS} " CACHE INTERNAL "Cxx compiler flags")
#ASFLAGS
set(CMAKE_ASM_FLAGS "${MCU} -x assembler-with-cpp ${DBG_FLAGS} " CACHE INTERNAL "ASM compiler flags")

set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -specs=nano.specs -specs=nosys.specs -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections" CACHE INTERNAL "Exe linker flags")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-lc -lm -lnosys" CACHE INTERNAL "Shared linker flags")