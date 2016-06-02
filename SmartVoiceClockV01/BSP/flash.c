

/******************** (C) COPYRIGHT  2016 ********************************

* 文件名  ：
 * 描述    ：
 * 库版本  ：V1.3.0
 * 作者    ：DongXu
 
********************************************************************************/	

#include "flash.h"

union union_temp16
{
    unsigned int un_temp16;
    unsigned char  un_temp8[2];		// example 16: 0x0102  8:[0]2 [1]1
}my_unTemp16;


/******************************************************	
flash 字符串写入
每次存入两个字节
*******************************************************/
void FlashWriteStr( uint32_t flash_add, uint16_t len, unsigned char* data )
{
	//char cp[12];
	//u8 s = 0;
	u16 byteN = 0;
	FLASH_Unlock();
	FLASH_ErasePage(flash_add);
	//sprintf( cp, "len:%d", len);
	//USART1_Puts(cp);
	while( len )
	{
		my_unTemp16.un_temp8[0] = *(data+byteN);
		my_unTemp16.un_temp8[1] = *(data+byteN+1);		
		FLASH_ProgramHalfWord( flash_add+byteN , my_unTemp16.un_temp16 );

		//sprintf( cp, "bye:%d\r\n", s);
		//USART1_Puts(cp);
		if( 1==len )
		{
			//如果数据长度是奇数
			//FLASH_ProgramWord(flash_add+byteN+1, *(data+byteN+2));	
			break;													   
		}
		else
		{
			byteN += 2;
			len -= 2;
		}	
	}
	FLASH_Lock();
}

/******************************************************
flash 字符串读出到指定data中  
地址与写入data地址同 读出的保存类型也必须一致
*******************************************************/
void FlashReadStr( uint32_t flash_add, uint16_t len,unsigned char* data )
{
	u16 byteN = 0;
	while( len )
	{
		my_unTemp16.un_temp16 = *(vu16*)(flash_add+byteN);
		if( 1==len )
		{
			*(data+byteN) = my_unTemp16.un_temp8[0];
			break;			   
		}
		else
		{		
			*(data+byteN) = my_unTemp16.un_temp8[0];
			*(data+byteN+1) = my_unTemp16.un_temp8[1];
			byteN += 2;
			len -= 2;
		}
	}
}

int FlashWrite( uint32_t flash_add, uint32_t data)
{
	FLASH_Status status = FLASH_TIMEOUT;
	FLASH_Unlock();
	do{
		status = FLASH_ErasePage(flash_add);
	}while(status != FLASH_COMPLETE);
	do{
		FLASH_ProgramWord(flash_add, data);	
	}while(status != FLASH_COMPLETE);
	if(*(uint32_t*)flash_add != data)
	{
		return -1;
	}
	
	FLASH_Lock();
	
	return 0;
}

/***************************************************************/
uint32_t FlashReadWord( u32 flash_add)
{
	 return *((uint32_t*)flash_add);	
}

uint8_t FlashReadByte(uint32_t flash_add)
{
	return *((const uint8_t*)flash_add);
}












