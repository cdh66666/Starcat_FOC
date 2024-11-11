# Starcat_FOC
 星猫无刷FOC大功率驱动器

# hardware

- PCB电路板采用嘉立创专业版制作，.epro工程文件已存放在hardware里，嘉立创EDA->文件->导入->专业版即可导入！
- 放了PCB的布线布局图和实物图。

# firmware

- 主控采用stm32F405RGT6
- 代码采用stm32cubeide编写
- 采用DRV8301无刷驱动芯片

# 待解决问题

1. 无刷FOC三环未调好。
2. 未采用freertos协调各个进程（oled，can，串口，闭环控制等）
3. can通信代码未编写。
4. 电机参数辨识只实现了极对数，方向，零点角度，还有相电阻，相电感，等等
