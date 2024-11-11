// flash.c
#include "flash.h"

// 根据组号计算扇区号
int calculateSectorNumber(int groupNumber)
{
    if (groupNumber % 2 == 0)
    {
        return SECOND_LAST_SECTOR;
    }
    else
    {
        return LAST_SECTOR;
    }
}

// 因为是通过判断第一个数据是否为1000，来判断是否校准过，所以将一位改为零就可以清除校准数据（指定组）
void clear_Data(int groupNumber)
{
    float data[1] = {0};
    // 将多个数据写入 Flash
    writeMultipleDataToFlash(data, 1, groupNumber);
}

// 将多个数据写入 Flash（指定组）
void writeMultipleDataToFlash(float *data, int dataCount, int groupNumber)
{
    HAL_FLASH_Unlock();

    // 定义擦除扇区的错误状态和扇区地址
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    // 根据组号计算扇区号
    int sectorNumber = calculateSectorNumber(groupNumber);
    EraseInitStruct.Sector = sectorNumber;
    EraseInitStruct.NbSectors = 1;
    uint32_t SectorError;

    // 擦除扇区
    HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);

    // 根据扇区起始地址计算偏移地址
    uint32_t offset;
    if (sectorNumber == SECOND_LAST_SECTOR)
    {
        offset = ADDR_FLASH_SECTOR_11;
    }
    else
    {
        offset = ADDR_FLASH_SECTOR_10;
    }

    uint32_t *flashAddress = (uint32_t *)(offset);

    for (int i = 0; i < dataCount; i++)
    {
        // 将 float 数据转换为 32 位整数存储到 Flash
        uint32_t data32 = *(uint32_t *)&data[i];
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)flashAddress, data32);
        flashAddress++;
    }

    HAL_FLASH_Lock();
}

// 从 Flash 读取多个数据（指定组）
void readMultipleDataFromFlash(float *data, int dataCount, int groupNumber)
{
    // 根据组号计算扇区号
    int sectorNumber = calculateSectorNumber(groupNumber);

    // 根据扇区起始地址计算偏移地址
    uint32_t offset;
    if (sectorNumber == SECOND_LAST_SECTOR)
    {
        offset = ADDR_FLASH_SECTOR_11;
    }
    else
    {
        offset = ADDR_FLASH_SECTOR_10;
    }

    uint32_t *flashAddress = (uint32_t *)(offset);

    for (int i = 0; i < dataCount; i++)
    {
        data[i] = *(float *)flashAddress++;
    }
}

// 检查 Flash 是否已经存储了指定数量的数据（指定组）
int isDataStoredInFlash(int groupNumber)
{
    // 根据组号计算扇区号
    int sectorNumber = calculateSectorNumber(groupNumber);

    // 根据扇区起始地址计算偏移地址
    uint32_t offset;
    if (sectorNumber == SECOND_LAST_SECTOR)
    {
        offset = ADDR_FLASH_SECTOR_11;
    }
    else
    {
        offset = ADDR_FLASH_SECTOR_10;
    }

    uint32_t *flashAddress = (uint32_t *)(offset);

    // 我会将第一位设置成1000，如果我已经写过参数了的话
    if (*(float *)flashAddress > 999 && *(float *)flashAddress < 1001)
    {
        return 1;
    }
    return 0;
}
