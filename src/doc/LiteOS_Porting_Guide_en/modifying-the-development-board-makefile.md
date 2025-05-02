# Modifying the Development Board Makefile<a name="EN-US_TOPIC_0314628531"></a>

1.  Replace  **Cloud\_STM32F429IGTx\_FIRE**  with  **STM32F407\_OpenEdv**.
2.  The  **STM32F407\_OpenEdv**  directory does not contain certain files and subdirectories compared with the  **Cloud\_STM32F429IGTx\_FIRE**  project directory. You need to delete the reference to these files in Makefile, that is, delete the following content:

    ```
    ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
            BSP_DRIVER_SRC = \
            Drivers/i2c/i2c.c \
            Drivers/mpu6050/mpu6050.c
            C_SOURCES += $(BSP_DRIVER_SRC)
    endif
    ```

    ```
    ifeq ($(LOSCFG_COMPONENTS_FS), y)
        ifeq ($(LOSCFG_FATFS_USE_SD_CARD), y)
            FS_SRC += \
                Src/fatfs_sd_hal.c \
                Src/stm324x9i_eval_sd.c \
                Src/sd_diskio.c
        endif
        ifeq ($(LOSCFG_FATFS_USE_SPI_FLASH), y)
            FS_SRC += \
                Src/fatfs_spi_hal.c
        endif
        ifeq ($(LOSCFG_COMPONENTS_FS_SPIFFS), y)
            FS_SRC += \
                Src/spiffs_hal.c
        endif
        C_SOURCES += $(FS_SRC)
    endif
    ```

    ```
    BSP_INC = \
            --I $(LITEOSTOPDIR)/targets/TM32F407_OpenEdv/Drivers/BSP/STM324x9I_EVAL \ 
    HARDWARE_INC = \
            -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Hardware/Inc
            BOARD_INCLUDES += $(HARDWARE_INC)
    ```

    ```
    HARDWARE_SRC =  \
            ${wildcard Hardware/Src/*.c}
            C_SOURCES += $(HARDWARE_SRC)
    USER_SRC = \
            Src/flash_adaptor.c \
    ```

    ```
    BSP_DRIVER_INC = \
           -I $(LITEOSTOPDIR)/targets/TM32F407_OpenEdv/Drivers/i2c \
           -I $(LITEOSTOPDIR)/targets/TM32F407_OpenEdv/Drivers/mpu6050
    BOARD_INCLUDES += $(BSP_DRIVER_INC)
    ```

3.  Search for the keyword  **STM32F429**  and replace it with  **STM32F407**.
4.  If you need to add your own source file, you can add the source file to the  _USER\_SRC_  variable.

