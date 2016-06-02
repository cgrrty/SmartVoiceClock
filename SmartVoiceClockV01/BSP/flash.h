#ifndef _FLASH_H
#define _FLASH_H

#include "stm32f0xx.h"

#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x0800D400)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   /* End @ of user Flash area */


typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  volatile uint16_t vu16;
/****************************************************************/
int FlashWrite( uint32_t flash_add, uint32_t data);
uint32_t FlashReadWord( u32 flash_add);
uint8_t FlashReadByte(uint32_t flash_add);

void FlashWriteStr( uint32_t flash_add, uint16_t len, unsigned char* data );
void FlashReadStr( uint32_t flash_add, uint16_t len, unsigned char* data );
/***************************************************************/


#endif
