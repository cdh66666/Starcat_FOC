程序说明：
       此程序是基于群主（小时候可吊了）源码修改移植版本，CSDN 和  B站地址如下;
       https://blog.csdn.net/qq_35947329/article/details/115483413?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522162635128216780366530465%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=162635128216780366530465&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-2-115483413.first_rank_v2_pc_rank_v29&utm_term=FOC&spm=1018.2226.3001.4187
       https://www.bilibili.com/video/BV1y64y1x7Sb?spm_id_from=333.337.search-card.all.click

项目说明：
      此项目作为入门 有感FOC 使用。选用了 4010-74KV-12V 无刷云台电机，绘制了定制版PCB，完美贴合电机背面。
硬件搭配是STM32F103C8T6+DRV8313+INA2402+MT6816.   
修改群主程序并移植了IQmath库后，  PWM-20kHz,FOC10kHz，速度闭环1kHz，电机流畅运行。


板子供电：12-14V，  无防反插，插反必烧。