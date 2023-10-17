## 

## 基于STM32H750XB  适用于已有QSPI下载算法的BootLoader(非QSPI XIP)

Bootloader保存在内部flash上 ，通过下载算法将APP下载到QSPI   

Bootloader流程如下：

- 初始化QSPI
- 开启内存映射模式
- 打印logo
- 解初始化所有外设
- 关闭中断
- 恢复默认时钟（HSI）
- 跳转



![image-20231017192241668](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017192241668.png)

包含简要文档说明

![image-20231017192507091](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017192507091.png)

跳转实现如下

![image-20231017194829764](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017194829764.png)



扩展：跳转之前这里未检查QSPI中是否存在APP固件，可通过bin文件的前两个字来判断（注意bin文件格式STM32为小端）