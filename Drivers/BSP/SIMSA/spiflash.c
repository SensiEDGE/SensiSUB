#include "spiflash.h"
#include "stm32l4xx_periph_conf.h"

#ifdef USE_STM32F4XX_NUCLEO
#include "stm32f4xx_SPI.h"
#endif /* USE_STM32F4XX_NUCLEO */

#ifdef USE_STM32L4XX_NUCLEO
#include "stm32l4xx_SPI.h"
#endif /* USE_STM32L4XX_NUCLEO */

#ifdef USE_STM32L0XX_NUCLEO
#include "stm32l0xx_SPI.h"
#endif /* USE_STM32L0XX_NUCLEO */

/* Private defines begin -----------------------------------------------------*/
#define SPIFLASH_CS_PIN GPIO_PIN_9
#define SPIFLASH_CS_PORT GPIOA

#define SPIFLASH_CS_INIT() do {             \
    GPIO_InitTypeDef init = {               \
        .Pin = SPIFLASH_CS_PIN,             \
        .Mode = GPIO_MODE_OUTPUT_PP,        \
        .Pull = GPIO_NOPULL,                \
        .Speed = GPIO_SPEED_FREQ_HIGH,      \
        .Alternate = 0                      \
    };                                      \
                                            \
    HAL_GPIO_Init(SPIFLASH_CS_PORT, &init); \
} while(0);                            

#define SPIFLASH_CS_ON()   HAL_GPIO_WritePin(SPIFLASH_CS_PORT, SPIFLASH_CS_PIN, GPIO_PIN_RESET)
#define SPIFLASH_CS_OFF()  HAL_GPIO_WritePin(SPIFLASH_CS_PORT, SPIFLASH_CS_PIN, GPIO_PIN_SET)

#define SPIFLASH_MANUFACTURER_ID   0x1F
#define SPIFLASH_DEVICE_ID_PART1   0x44
#define SPIFLASH_DEVICE_ID_PART2   0x02
#define SPIFLASH_EXNTENDED_DEV_INF 0x00

#define SPIFLASH_BEGINADDR         0x000000
#define SPIFLASH_FINISHADDR        SPIFLASH_FLASHSIZE - 1
#define SPIFLASH_TIMEOUT           10000

/*------------------------------IMPORTANT-------------------------------------*/
//BLE module is connected to the same SPI, and it calls interrupt on pin A0.
//When interrup is called during working with flash, errors might occur.
//These macroses must be called to protect weak sections of code. 
#define SPIFLASH_ENTER_CRITICAL_SECTION()  HAL_NVIC_DisableIRQ(SPI1_CMN_DEFAULT_EXTI_IRQn)
#define SPIFLASH_EXIT_CRITICAL_SECTION()   HAL_NVIC_EnableIRQ(SPI1_CMN_DEFAULT_EXTI_IRQn)
/*----------------------------------------------------------------------------*/
            
/* Private defines end -------------------------------------------------------*/

/* Private function prototypes begin -----------------------------------------*/
HAL_StatusTypeDef SpiFlash_ReadDeviceId(uint8_t* devId, uint8_t len);
inline HAL_StatusTypeDef SpiFlash_ReadStatusReg(uint8_t* statusReg, uint8_t len);
HAL_StatusTypeDef SpiFlash_ErasePage(uint32_t pageAddress);
HAL_StatusTypeDef SpiFlash_ReadPage(uint32_t pageAddress, uint8_t* data);
HAL_StatusTypeDef SpiFlash_Read(uint32_t address, uint8_t* data, uint32_t size);
HAL_StatusTypeDef SpiFlash_WritePage(uint32_t pageAddress, uint8_t* data);
HAL_StatusTypeDef SpiFlash_WritePartPage(uint32_t pageAddress, uint8_t* data, 
                                         uint16_t* len, uint16_t maxSize,
                                         uint8_t offset);

HAL_StatusTypeDef SpiFlash_WaitForReady(uint32_t timeout);

HAL_StatusTypeDef SpiFlash_Write(uint32_t pageAddress, uint8_t* data, 
                                 uint16_t* len, uint16_t maxSize);

HAL_StatusTypeDef SpiFlash_WriteEnable();
HAL_StatusTypeDef SpiFlash_WriteDisable();
/* Private function prototypes end  ------------------------------------------*/

/* Private variables begin ---------------------------------------------------*/
enum SpiFlash_Commands
{
    readArray = 0x0B,
    pageErase = 0x81,
    chipErase = 0x60,
    bytePageProgram = 0x02,
    writeEnable = 0x06,
    writeDisable = 0x04,
    protectSector = 0x36,
    unprotectSector = 0x39,
    readSectorProtectionRegister = 0x3C,
    readStatusRegister = 0x05,
    writeStatusRegisterByte1 = 0x01,
    writeStatusRegisterByte2 = 0x31,
    reset = 0xF0,
    readDeviceId = 0x9F,
};

struct SpiFlash_StatusRegister
{
    //first byte
    uint16_t RDY_BSY  : 1;
    uint16_t WEL      : 1;
    uint16_t SWP      : 2;
    uint16_t WPP      : 1;
    uint16_t EPE      : 1;
    uint16_t SPM      : 1;
    uint16_t SPRL     : 1;
    //second byte
    uint16_t RDY_BS   : 1;
    uint16_t RES_13   : 3;
    uint16_t RSTE     : 1;
    uint16_t RES_57   : 3;
};

struct SpiFlash_DeviceId
{
    uint8_t manufacturerId;
    uint8_t deviceIdPart1;
    uint8_t deviceIdPart2;
    uint8_t extendedDevInf;
};
/* Private variables end -----------------------------------------------------*/

/* Public functions realization begin ----------------------------------------*/

/**
* @brief   Initialize SPI peripheral and CS pin for driver.
*          Try to read spi flash device id.
* @param   none.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_Init(void)
{
    SPI_Global_Init();
    struct SpiFlash_DeviceId devId  = {0};
    HAL_StatusTypeDef status;
    
    SPIFLASH_CS_INIT();
    //need to protect weak section of code
    SPIFLASH_ENTER_CRITICAL_SECTION();
    status = SpiFlash_ReadDeviceId((uint8_t*)&devId, sizeof(devId));
    if(status == HAL_OK){
        if((devId.manufacturerId == SPIFLASH_MANUFACTURER_ID) && 
           (devId.deviceIdPart1 == SPIFLASH_DEVICE_ID_PART1) &&
           (devId.deviceIdPart2 == SPIFLASH_DEVICE_ID_PART2) &&
           (devId.extendedDevInf == SPIFLASH_EXNTENDED_DEV_INF))
        {
            SPIFLASH_EXIT_CRITICAL_SECTION();
            return HAL_OK;
        }
    }
    SPIFLASH_EXIT_CRITICAL_SECTION();
    return HAL_ERROR;
}

/**
* @brief   Public function. Read a number of bytes from spi flash.
*          When the last byte (07FFFFh) of the memory array of spi flash 
*          has been read, the spi flash will continue reading back 
*          at the beginning of the array (000000h)
* @param   address: Specify the begin address of data that is needed to be read.
* @param   data:    Specify the pointer to an array, where to save data.
* @param   size: Specify the number of data to read.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_ReadByteArray(uint32_t address, uint8_t* data, uint32_t size)
{
    if((data == NULL) || (size == 0) || (address > SPIFLASH_FINISHADDR)){
        return HAL_ERROR;
    }
    HAL_StatusTypeDef status;
    SPIFLASH_ENTER_CRITICAL_SECTION();
    status = SpiFlash_Read(address, data, size);
    SPIFLASH_EXIT_CRITICAL_SECTION();
    return status;
}

/**
* @brief   Public function. Write a number of bytes to spi flash.
* @param   address: Specify the begin address where to save data.
* @param   data:    Specify the pointer to an array, where are located data to be saved.
* @param   len:     Specify the pointer to a variable where to save the length 
*                   of saved data.
* @param   maxSize: Specify the maximum number of data to save.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WriteByteArray(uint32_t address, uint8_t* data, 
                                          uint16_t* len, uint16_t maxSize)
{
    if((data == NULL) || (len == NULL) || (maxSize == 0)){
        return HAL_ERROR;
    }
    *len = 0;
    HAL_StatusTypeDef status;
    SPIFLASH_ENTER_CRITICAL_SECTION();
    if((address % SPIFLASH_PAGESIZE) != 0){
        uint8_t offset = address % SPIFLASH_PAGESIZE;
        uint8_t size = SPIFLASH_PAGESIZE - offset;
        if(maxSize < size){
            size = maxSize;
        }
        status = SpiFlash_WritePartPage((address - offset), data, len, size, offset);
        maxSize -= size;
        if(maxSize == 0 || status != HAL_OK){
            SpiFlash_WriteDisable();
            SPIFLASH_EXIT_CRITICAL_SECTION();
            return status;
        } else {
            address += size;
            data += size;
        }
    }
    //if received page begin address
    if(maxSize == SPIFLASH_PAGESIZE){
        status = SpiFlash_Write(address, data, len, maxSize);
        SpiFlash_WriteDisable();
        SPIFLASH_EXIT_CRITICAL_SECTION();
        return status;
    } else if (maxSize > SPIFLASH_PAGESIZE){
        uint8_t numberOfPages = maxSize / SPIFLASH_PAGESIZE;
        uint8_t remainder = maxSize % SPIFLASH_PAGESIZE;
        
        for(uint8_t i = 0; i < numberOfPages; i++)
        {
            address += i * SPIFLASH_PAGESIZE;
            data += i * SPIFLASH_PAGESIZE;
            status = SpiFlash_Write(address, data, len, SPIFLASH_PAGESIZE);
            if(status != HAL_OK){
                SpiFlash_WriteDisable();
                SPIFLASH_EXIT_CRITICAL_SECTION();
                return status;
            }
        }
        address += SPIFLASH_PAGESIZE;
        data += SPIFLASH_PAGESIZE;
        status = SpiFlash_WritePartPage(address, data, len, remainder, 0);
        SpiFlash_WriteDisable();
        SPIFLASH_EXIT_CRITICAL_SECTION();
        return status;
    }else{
        //maxSize < SPIFLASH_PAGESIZE
        status = SpiFlash_WritePartPage(address, data, len, maxSize, 0);
        SpiFlash_WriteDisable();
        SPIFLASH_EXIT_CRITICAL_SECTION();
        return status;
    }
}

/**
* @brief   Erase the entire spi flash.
* @param   none.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_EraseAll()
{
    HAL_StatusTypeDef status;
    uint8_t command = chipErase;
    SPIFLASH_ENTER_CRITICAL_SECTION();
    status = SpiFlash_WriteEnable();
    if(status != HAL_OK){
        SpiFlash_WriteDisable();
        SPIFLASH_EXIT_CRITICAL_SECTION();
        return status;
    }
    
    SPIFLASH_CS_ON();
    //send erase command
    status = HAL_SPI_Transmit(&SpiHandle, &command, sizeof(command), SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    if(status != HAL_OK){
        SpiFlash_WriteDisable();
        SPIFLASH_EXIT_CRITICAL_SECTION();
        return status;
    }
    status = SpiFlash_WaitForReady(SPIFLASH_TIMEOUT);
    SpiFlash_WriteDisable();
    SPIFLASH_EXIT_CRITICAL_SECTION();
    return status;
}

/**
* @brief   Erase a number of pages in spi flash.
* @param   pageAddress:   Specify the page address.
* @param   numberOfPages: Specify the number of pages to erase.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_ErasePages(uint32_t pageAddress, uint16_t numberOfPages)
{
    if(((pageAddress % SPIFLASH_PAGESIZE) != 0) || (numberOfPages == 0)
       || (numberOfPages > (SPIFLASH_FLASHSIZE - pageAddress) / SPIFLASH_PAGESIZE))
    {
        return HAL_ERROR;
    }
    HAL_StatusTypeDef status;
    SPIFLASH_ENTER_CRITICAL_SECTION();
    for(uint16_t i = 0; i < numberOfPages; i++){
        status = SpiFlash_WriteEnable();
        if(status != HAL_OK){
            SpiFlash_WriteDisable();
            SPIFLASH_EXIT_CRITICAL_SECTION();
            return status;
        }
        status = SpiFlash_ErasePage(pageAddress);
        pageAddress += SPIFLASH_PAGESIZE;
        if(status != HAL_OK){
            SpiFlash_WriteDisable();
            SPIFLASH_EXIT_CRITICAL_SECTION();
            return status;
        }
    }
    SpiFlash_WriteDisable();
    SPIFLASH_EXIT_CRITICAL_SECTION();
    return status;
}



/* Public functions realization end ------------------------------------------*/

/* Private functions realization begin ---------------------------------------*/

/**
* @brief   Private function. Read device id from spi flash.
* @param   devId:   Specify the pointer to variable where to save device id.
* @param   len:     Specify the length of device id.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_ReadDeviceId(uint8_t* devId, uint8_t len)
{
    HAL_StatusTypeDef status;
    uint8_t command = readDeviceId;
    SPIFLASH_CS_ON();
    status = HAL_SPI_Transmit(&SpiHandle, &command, 1, SPIFLASH_TIMEOUT);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPIEx_FlushRxFifo(&SpiHandle);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPI_Receive(&SpiHandle, devId, len, SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    return status;
}

/**
* @brief   Private function. Read status register from spi flash.
* @param   statusReg: Specify the pointer to variable where to save data.
* @param   len:       Specify the length of status register.
* @retval  HAL status
*/
inline HAL_StatusTypeDef SpiFlash_ReadStatusReg(uint8_t* statusReg, uint8_t len)
{
    HAL_StatusTypeDef status;
    uint8_t command = readStatusRegister;
    SPIFLASH_CS_ON();
    status = HAL_SPI_Transmit(&SpiHandle, &command, 1, SPIFLASH_TIMEOUT);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPIEx_FlushRxFifo(&SpiHandle);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPI_Receive(&SpiHandle, statusReg, len, SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    return status;
}

/**
* @brief   Private function. Wait when spi flash will be 
*          ready for next operation.
* @param   timeout: Timeout duration.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WaitForReady(uint32_t timeout)
{
    if(timeout == 0 || timeout == HAL_MAX_DELAY){
        return HAL_TIMEOUT;
    }
    
    HAL_StatusTypeDef status;
    uint32_t tickstart = HAL_GetTick();
    while((HAL_GetTick() - tickstart) <  timeout){
        struct SpiFlash_StatusRegister reg = {0};
        status = SpiFlash_ReadStatusReg((uint8_t*)&reg, sizeof(reg));
        
        if(status != HAL_OK){
            return status;
        }
        if(reg.RDY_BSY == 0){
            return HAL_OK;
        }
    }
    return HAL_TIMEOUT;
}

/**
* @brief   Erase one page in spi flash.
* @param   pageAddress: Specify the page address.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_ErasePage(uint32_t pageAddress)
{
    if((pageAddress % SPIFLASH_PAGESIZE) != 0){
        return HAL_ERROR;
    }
    HAL_StatusTypeDef status;
    uint8_t command[4] = {
        pageErase,
        (pageAddress & 0x070000) >> 16,
        (pageAddress & 0xFF00) >> 8,
        0
    };
    
    SPIFLASH_CS_ON();
    //send erase command
    status = HAL_SPI_Transmit(&SpiHandle, command, sizeof(command), SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    
    if(status != HAL_OK){
        return status;
    }
    return SpiFlash_WaitForReady(SPIFLASH_TIMEOUT);
}

/**
* @brief   Public function. Read one page (SPIFLASH_PAGESIZE of bytes)
*          from spi flash.
* @param   pageAddress: Specify the page begin address.
* @param   data:        Specify the pointer to an array, where to save data.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_ReadPage(uint32_t pageAddress, uint8_t* data)
{
    if((data == NULL) || 
       (pageAddress > (SPIFLASH_FLASHSIZE - SPIFLASH_PAGESIZE)) ||
        ((pageAddress % SPIFLASH_PAGESIZE) != 0))
    {
        return HAL_ERROR;
    }
    return SpiFlash_Read(pageAddress, data, SPIFLASH_PAGESIZE);
}

/**
* @brief   Private function. Write one page (SPIFLASH_PAGESIZE of bytes) 
*          to spi flash.
* @param   pageAddress: Specify the page begin address.
* @param   data:        Specify the pointer to an array, 
*                       where are located data to be saved.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WritePage(uint32_t pageAddress, uint8_t* data)
{
    if(((pageAddress % SPIFLASH_PAGESIZE) != 0) || 
        (data == NULL) || (pageAddress >= SPIFLASH_FLASHSIZE))
    {
        return HAL_ERROR;
    }
    
    HAL_StatusTypeDef status;
    uint8_t command[4] = {
        bytePageProgram,
        (pageAddress & 0x070000) >> 16,
        (pageAddress & 0xFF00) >> 8,
        0
    };
    SPIFLASH_CS_ON();
    //send program command
    status = HAL_SPI_Transmit(&SpiHandle, command, sizeof(command), SPIFLASH_TIMEOUT);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    //send data
    status = HAL_SPI_Transmit(&SpiHandle, data, SPIFLASH_PAGESIZE, SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    if(status != HAL_OK){
        return status;
    }
    return SpiFlash_WaitForReady(SPIFLASH_TIMEOUT);
}

/**
* @brief   Private function. It is used to write data array to spi flash 
*          when it's length is less then SPIFLASH_PAGESIZE
* @param   pageAddress: Specify the page begin address.
* @param   data:        Specify the pointer to an array, where are located data 
*                       to be written to spi flash.
* @param   len:         Specify the pointer to a variable where to save  
*                       the length of saved data.
* @param   size:        Specify the number of data to save.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WritePartPage(uint32_t pageAddress, uint8_t* data, 
                                         uint16_t* len, uint16_t size,
                                         uint8_t offset)
{
    uint8_t tmpArr[SPIFLASH_PAGESIZE];
    HAL_StatusTypeDef status;
    uint16_t tmpLen = 0;
    
    //read one page from spi flash
    status = SpiFlash_ReadPage(pageAddress, tmpArr);
    if(status != HAL_OK){
        return status;
    }
    //copy data that is needed to be written to tmpArr
    //(modify read buffer, saving bytes that we don't want to modify)
    memcpy((tmpArr + offset), data, size);
    
    //write modified buffer to spi flash
    status = SpiFlash_Write(pageAddress, tmpArr, &tmpLen, sizeof(tmpArr));
    if(status == HAL_OK){
        *len += size;
    }
    return status;
}

/**
* @brief   Private function. Erase one page and then write new data there.
* @param   address: Specify the begin address where to save data.
* @param   data:    Specify the pointer to an array, where are located data to be saved.
* @param   len:     Specify the pointer to a variable where to save the length 
*                   of saved data.
* @param   maxSize: Specify the maximum number of data to save.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_Write(uint32_t address, uint8_t* data, 
                                 uint16_t* len, uint16_t maxSize)
{
    if((maxSize == 0) || (data == NULL) || (len == NULL)){
        return HAL_ERROR;
    }
    if(((address % SPIFLASH_PAGESIZE) != 0) || (address >= SPIFLASH_FINISHADDR)){
       return HAL_ERROR;
    }
    HAL_StatusTypeDef status;
    status = SpiFlash_WriteEnable();
    if(status != HAL_OK){
        return status;
    }
    status = SpiFlash_ErasePage(address);
    if(status != HAL_OK){
        return status;
    }
    status = SpiFlash_WriteEnable();
    if(status != HAL_OK){
        return status;
    }
    status = SpiFlash_WritePage(address, data);
    if(status != HAL_OK){
        return status;
    }
    *len += maxSize;
    return status;
}

/**
* @brief   Private function. Read a number of bytes from spi flash.
*          When the last byte (07FFFFh) of the memory array of spi flash 
*          has been read, the spi flash will continue reading back 
*          at the beginning of the array (000000h)
* @param   address: Specify the begin address of data that is needed to be read.
* @param   data:    Specify the pointer to an array, where to save data.
* @param   size: Specify the number of data to read.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_Read(uint32_t address, uint8_t* data, uint32_t size)
{
    if((data == NULL) || (size == 0) || (address > SPIFLASH_FINISHADDR)){
        return HAL_ERROR;
    }
    
    HAL_StatusTypeDef status;
    uint8_t command[5] = {
        readArray,
        (address & 0x00070000) >> 16,
        (address & 0x0000FF00) >> 8,
        address & 0x000000FF,
        0
    };
    SPIFLASH_CS_ON();
    status = HAL_SPI_Transmit(&SpiHandle, command, sizeof(command), SPIFLASH_TIMEOUT);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPIEx_FlushRxFifo(&SpiHandle);
    if(status != HAL_OK){
        SPIFLASH_CS_OFF();
        return status;
    }
    status = HAL_SPI_Receive(&SpiHandle, data, size, SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    return status;
}

/**
* @brief   Private function. Set the Write Enable Latch (WEL) bit in the 
*          Status Register to a logical “1” state.
* @param   none.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WriteEnable()
{
    HAL_StatusTypeDef status;
    uint8_t cnt = 0;
    do{
        struct SpiFlash_StatusRegister reg = {0};
        status = SpiFlash_ReadStatusReg((uint8_t*)&reg, sizeof(reg));
        if(status != HAL_OK){
            return status;
        }
        //WEL == 0 - device is write disabled
        //WEL == 1 - device is write enabled
        if(reg.WEL != 1){
            uint8_t command = writeEnable;
            SPIFLASH_CS_ON();
            //send command
            status = HAL_SPI_Transmit(&SpiHandle, &command, 1, SPIFLASH_TIMEOUT);
            SPIFLASH_CS_OFF();
            if(status != HAL_OK){
                return status;
            }
            continue;
        }
        //SWP == 0 - All sectors are software unprotected
        //SWP == 1 - Some sectors are software protected.
        //SWP == 3 - All sectors are software protected, default
        if(reg.SWP != 0){
            uint8_t command[2] = {
                writeStatusRegisterByte1, 
                // 0x00 - Global Unprotect
                // 0xFF - Global Protect
                0};
            SPIFLASH_CS_ON();
            //send command
            status = HAL_SPI_Transmit(&SpiHandle, command, sizeof(command), SPIFLASH_TIMEOUT);
            SPIFLASH_CS_OFF();
            if(status != HAL_OK){
                return status;
            }
            continue;
        }
        return HAL_OK;
    }while(++cnt < 5);
    
    return HAL_ERROR;
}

/**
* @brief   Private function. Set the Write Enable Latch (WEL) bit in the 
*          Status Register to a logical “0” state.
* @param   none.
* @retval  HAL status
*/
HAL_StatusTypeDef SpiFlash_WriteDisable()
{
    HAL_StatusTypeDef status;
    uint8_t command = writeDisable;
    SPIFLASH_CS_ON();
    //send command
    status = HAL_SPI_Transmit(&SpiHandle, &command, 1, SPIFLASH_TIMEOUT);
    SPIFLASH_CS_OFF();
    return status;
}
/* Private functions realization end -----------------------------------------*/