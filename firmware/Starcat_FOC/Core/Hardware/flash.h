// flash.h
#ifndef HARDWARE_FLASH_H_
#define HARDWARE_FLASH_H_

#include "main.h"
#include <stdio.h>

// FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) //扇区0起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) //扇区1起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) //扇区2起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) //扇区3起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) //扇区4起始地址, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) //扇区5起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) //扇区6起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) //扇区7起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) //扇区8起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) //扇区9起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) //扇区10起始地址,128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) //扇区11起始地址,128 Kbytes

#define MAX_DATA_COUNT 15    // 每组数据个数
#define LAST_SECTOR          FLASH_SECTOR_10
#define SECOND_LAST_SECTOR   FLASH_SECTOR_11

// 将多个数据写入 Flash（指定组）
void writeMultipleDataToFlash(float *data, int dataCount, int groupNumber);
// 从 Flash 读取多个数据（指定组）
void readMultipleDataFromFlash(float *data, int dataCount, int groupNumber);
// 检查 Flash 是否已经存储数据（指定组）
int isDataStoredInFlash(int groupNumber);
// 因为是通过判断第一个数据是否为1000，来判断是否校准过，所以将一位改为零就可以清除校准数据（指定组）
void clear_Data(int groupNumber);

// 根据组号计算扇区号
int calculateSectorNumber(int groupNumber);

#endif /* HARDWARE_FLASH_H_ */
