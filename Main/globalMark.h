#ifndef GLOBALMARK_H
#define GLOBALMARK_H

/*************************************
*通用设置头文件，定义通用功能
************************************/

//定义了部分嵌入式上运行的功能
#define PLATFORM_ARM

//启用debug
#define PROJECT_DEBUT

//启用程序加载画面
#define SPLASH_SCREEN

//指定插件加载路径，可通过QT_PLUGIN_PATH外部环境变量设置
#define US_PLUGINS_PATH "/home/plugins"

//tr编码
#define US_CODEC_FOR_TR "utf-8"

//回主页时间，秒数
#define US_SCREEN_BACK_TIME (600)

//屏保时间，秒数。实际屏保时间是：US_SCREEN_BACK_TIME + US_SCREEN_SLEEP_TIME
#define US_SCREEN_SLEEP_TIME (1200)


//中文语言文件路径,需要语言切换的可以设置
//#define US_CHINESE_PATH ""

//英文语言文件路径,需要语言切换的可以设置
//#define US_ENGLISH_PATH ""


#endif // GLOBALMARK_H
