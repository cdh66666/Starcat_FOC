#include "NTC.h"

const float Rp=10000.0; //10K
const float T2 = (273.15+25.0);;//T2
const float Bx = 3434.0;//B
const float Ka = 273.15;

const float resistanceTable[100] = {1000.0,2000.0,3000.0,4000.0,5000.0,6000.0,7000.0,8000.0,
		9000.0,10000.0,11000.0,12000.0,13000.0,14000.0,15000.0,16000.0,17000.0,18000.0,19000.0,
		20000.0,21000.0,22000.0,23000.0,24000.0,25000.0,26000.0,27000.0,28000.0,29000.0,30000.0,
		31000.0,32000.0,33000.0,34000.0,35000.0,36000.0,37000.0,38000.0,39000.0,40000.0,41000.0,
		42000.0,43000.0,44000.0,45000.0,46000.0,47000.0,48000.0,49000.0,50000.0,51000.0,52000.0,
		53000.0,54000.0,55000.0,56000.0,57000.0,58000.0,59000.0,60000.0,61000.0,62000.0,63000.0,
		64000.0,65000.0,66000.0,67000.0,68000.0,69000.0,70000.0,71000.0,72000.0,73000.0,74000.0,
		75000.0,76000.0,77000.0,78000.0,79000.0,80000.0,81000.0,82000.0,83000.0,84000.0,85000.0,
		86000.0,87000.0,88000.0,89000.0,90000.0,91000.0,92000.0,93000.0,94000.0,95000.0,96000.0,
		97000.0,98000.0,99000.0,100000.0};

const float temperatureTable[100] = {99.50,73.43,59.80,50.77,44.09,38.84,34.53,30.89,27.75,
		25.00,22.55,20.35,18.36,16.54,14.86,13.31,11.87,10.52,9.26,8.08,6.96,5.90,4.89,3.94,
		3.03,2.16,1.33,0.53,-0.23,-0.96,-1.67,-2.35,-3.00,-3.64,-4.25,-4.84,-5.41,-5.97,-6.51,
		-7.03,-7.54,-8.03,-8.51,-8.98,-9.44,-9.88,-10.32,-10.74,-11.15,-11.55,-11.95,-12.33,
		-12.71,-13.08,-13.44,-13.79,-14.14,-14.48,-14.81,-15.14,-15.46,-15.77,-16.08,-16.38,
		-16.68,-16.97,-17.26,-17.54,-17.82,-18.09,-18.36,-18.62,-18.88,-19.14,-19.39,-19.64,
		-19.88,-20.13,-20.36,-20.60,-20.83,-21.05,-21.28,-21.50,-21.72,-21.93,-22.15,-22.35,
		-22.56,-22.77,-22.97,-23.17,-23.36,-23.56,-23.75,-23.94,-24.13,-24.31,-24.49,-24.67};

//获取温度值
float Get_Temp(float Rt)
{
//	float Rt;
	float temp;
//	Rt = Get_TempResistor();
	//like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),
	temp = Rt/Rp;
	temp = log(temp);//ln(Rt/Rp)
	temp/=Bx;//ln(Rt/Rp)/B
	temp+=(1/T2);
	temp = 1/(temp);
	temp-=Ka;
	return temp;
}


// 生成查找表
void generateLookupTable(void)
{
    // 假设电阻值从 1000 到 100000 欧姆，步长为 1000
    const int numEntries = 100;
    float resistanceStep = 99000.0 / (numEntries - 1);

    // 打印电阻值数组
    printf("const float resistanceTable[%d] = {", numEntries);
    for (int i = 0; i < numEntries; i++)
    {
        float resistance = 1000.0 + i * resistanceStep;
        if (i < numEntries - 1)
        {
            printf("%.1f,", resistance);
        }
        else
        {
            printf("%.1f", resistance);
        }
    }
    printf("};\n");

    // 打印温度值数组
    printf("const float temperatureTable[%d] = {", numEntries);
    for (int i = 0; i < numEntries; i++)
    {
        float resistance = 1000.0 + i * resistanceStep;
        float temp = Get_Temp(resistance);
        if (i < numEntries - 1)
        {
            printf("%.2f,", temp);
        }
        else
        {
            printf("%.2f", temp);
        }
    }
    printf("};\n");
}


// 二分查找获取温度
float getTemperatureFromTable(float Rt)
{
    int low = 0;
    int high = 99;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (resistanceTable[mid] == Rt)
        {
            return temperatureTable[mid];
        }
        else if (resistanceTable[mid] < Rt)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    // 线性插值
    if (low == 0)
    {
        return temperatureTable[0];
    }
    if (high == 99)
    {
        return temperatureTable[99];
    }

    float ratio = (Rt - resistanceTable[high]) / (resistanceTable[low] - resistanceTable[high]);
    return temperatureTable[high] + ratio * (temperatureTable[low] - temperatureTable[high]);

}

