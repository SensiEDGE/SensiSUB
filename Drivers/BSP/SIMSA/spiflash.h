#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include <stdint.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#define SPIFLASH_FLASHSIZE   0x080000
#define SPIFLASH_PAGESIZE    256
#define SPIFLASH_PAGENUMBER  ((SPIFLASH_FLASHSIZE) / (SPIFLASH_PAGESIZE))

HAL_StatusTypeDef SpiFlash_Init(void);

HAL_StatusTypeDef SpiFlash_ReadByteArray(uint32_t address, uint8_t* data, uint32_t size);
HAL_StatusTypeDef SpiFlash_WriteByteArray(uint32_t address, uint8_t* data, uint16_t* len, uint16_t maxSize);
HAL_StatusTypeDef SpiFlash_EraseAll();
HAL_StatusTypeDef SpiFlash_ErasePages(uint32_t firstPageAddress, uint16_t numberOfPages);

#endif 