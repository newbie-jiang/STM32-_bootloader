## keil生成二进制文件

![image-20231014213301509](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231014213301509.png)

![image-20231014213400800](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231014213400800.png)



```
并在后缀添加：   --bin -o  ./STM32H750XB/STM32H750XB.bin  ./STM32H750XB/STM32H750XB.axf
```

其中./STM32H750XB/STM32H750XB.bin    指定生成路径以及文件名

./STM32H750XB/STM32H750XB.axf    为依赖axf文件地址

如下，可根据自己的目录更改

![image-20231014220422262](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231014220422262.png)

编译可获得二进制文件

![image-20231014220524051](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231014220524051.png)