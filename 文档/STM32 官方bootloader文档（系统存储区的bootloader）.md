## 阅读STM32的bootloader文档（系统存储区的bootloader）

##  

参考资料：https://www.st.com/resource/en/application_note/an2606-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf

![image-20230924140820063](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924140820063.png)

这个引导加载程序允许用户通过各种通信协议来编程微控制器的 Flash 内存

![image-20230924140858750](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924140858750.png)



此手册涵盖几乎所有系列

![image-20230924141004997](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924141004997.png)

前面章节为一般信息，相关文档以及术语表，以及通用引导加载程序描述(重点)，接着我们需要阅读特定系列的引导加载程序

![image-20230924141148242](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924141148242.png)



## 相关文档

这里描述了具体的引导加载程序应用参考手册（支持的协议具体实现）

![image-20230924141705520](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924141705520.png)

```c
 直接复制搜索可以找到
– AN3154: CAN protocol used in the STM32 bootloader
– AN3155: USART protocol used in the STM32 bootloader
– AN3156: USB DFU protocol used in the STM32 bootloader
– AN4221: I2C protocol used in the STM32 bootloader
– AN4286: SPI protocol used in the STM32 bootloader
– AN5405: FDCAN protocol used in the STM32 bootloader
```

比如usart

![image-20230924142421760](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924142421760.png)

目前只是了解个大概，需要深入了解各协议则选择对应手册细看

![image-20230924142531925](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924142531925.png)



## 术语表

以H7为例描述了涵盖的具体设备

![image-20230924142851395](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924142851395.png)



## 4.1通用引导加载程序（重点）



### 引导程序激活

![image-20230924154349407](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924154349407.png)

bootloader 的激活和执行依赖于一系列的条件和配置。

### Bootloader 激活

bootloader 的激活可以通过表格 2 中描述的一种模式来实现。这些模式通常涉及到 Boot0 和 Boot1 引脚的状态，以及一些选项字节的配置。（见下表）

### Dual Boot 机制

如果启用了从 Bank2 启动的选项，bootloader 将执行 Dual Boot 机制。这通常用于支持双back Flash 的 STM32 微控制器，允许系统从两个不同的 Flash back中的任何一个启动。这为固件更新提供了一种安全机制，因为可以在一个back中更新固件，而另一个back保持原始固件。

双back参考文档：https://www.st.com/resource/en/application_note/an4826-stm32f7-series-flash-memory-dual-bank-mode-stmicroelectronics.pdf    介绍了F7系列的双闪存模式

### 读出保护 Level2

当激活读出保护 Level2 时，STM32 在任何情况下都不会在系统存储器上启动，bootloader 也不能被执行，除非从 Flash 用户代码中跳转到它。在这种情况下，除 Get、GetID 和 GetVersion 命令外，所有其他命令都无法访问。

​     （Level0：用户可以读取、写入和擦除 Flash 内存，也可以访问所有的 RAM 和外设）

​     （Level1：在此级别下，Flash 内存和备份寄存器的内容不能通过 JTAG/SWD 调试端口读取。）

​     （Level2：此级别提供更高级别的保护，一旦设定为 Level2，JTAG/SWD 接口将被禁用，无法连接调试   器，也无法通过这些端口读取 Flash 内存和备份寄存器的内容）



### 引导加载激活模式

每个模式（Pattern）都有一组条件（Condition），这些条件定义了哪些引脚和位需要被设置或清除以激活bootloader。

存在多个模式，每个模式都有不同的条件组合。这意味着有多种方法可以激活bootloader，具体取决于硬件配置和option bytes的设置。

不同的STM32产品可能会有不同的实现和配置。例如，BOOT_LOCK的实现可能会因产品而异。

即使在bootloader模式下，用户也可以通过特定的命令（如Go命令）来执行存储在Flash中的用户代码。

![image-20230924155909402](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924155909402.png)

![image-20230924155930398](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924155930398.png)

![image-20230924160020877](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924160020877.png)

1. **Boot0 和 Boot1**：
   - 这是微控制器上的两个引脚，用于控制bootloader的激活。它们的状态（0或1）决定了微控制器在复位后的启动行为。
2. **BFB2(bit)**：
   - 这是一个位，与双bank flash有关，控制从哪个bank启动。
3. **nDBANK 和 nDBOOT**：
   - 这些位与双bank flash的配置有关。
4. **BOOT_ADD0 和 BOOT_ADD1**：
   - 这些是option bytes，用于配置启动地址。
5. **nBoot0_SW、nSWBoot0 和 nBOOT0_SEL**：
   - 这些是控制boot0行为的软件位。
6. **BOOT_LOCK**：
   - 这可能是一个安全特性，用于锁定bootloader，防止未经授权的访问。
7. **TZen 和 RSSCMD**：
   - 与安全和保护特性有关。
8. **System Memory Jump**：
   - 用户可以通过从用户代码中跳转到系统存储器来执行bootloader。在跳转之前，用户必须禁用所有外设时钟、禁用PLL、禁用中断并清除挂起的中断。
9. **Exit Bootloader**：
   - 用户可以通过硬件复位或使用Go命令来退出bootloader模式并执行用户代码。

这里比较重要的是 **配置启动地址** 以及**System Memory Jump**



- STM32contexM3/4 使用两个boot引脚启动(启动地址固定)     contexM7可通一个boot引脚 并配置选项字节调整启动区域





## 4.2引导程序识别



![image-20230924171534010](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924171534010.png)

![image-20230924143850058](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924143850058.png)

补充：

![image-20230924143955936](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924143955936.png)

![image-20230924144010888](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924144010888.png)

### 引导加载程序的标识信息

此表提供了引导加载程序的标识信息

![image-20230924144117854](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924144117854.png)

假设以H750为例，我们通过读取该内存可得到ID

![image-20230924144441513](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924144441513.png)



## 4.3硬件连接要求

### USART

对于USART需要在主机侧连接上拉电阻，根据硬件选择合适的电阻

![image-20230924144811367](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924144811367.png)

### USB

DFU  设备固件升级（Device Firmware Upgrade）   USB连接至主机

这个额外的电路允许在需要时使用VBus将上拉电阻连接到DP引脚，参考产品章节（描述系统内存启动模式下STM32配置表，了解是否需要将外部上拉电阻连接到DP引脚）

![image-20230924145434280](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924145434280.png)

### I2C

这规定了i2c的上拉电阻为1.8k

![image-20230924150136166](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924150136166.png)

![image-20230924150153537](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924150153537.png)

### SPI

这里是标准的四线SPI其中的片选线需要连接到GND，以及时钟线需要接一个10k的下拉电阻

![image-20230924150413973](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924150413973.png)

### CAN

必须增加一个120Ω的电阻作为终端电阻

![image-20230924150848155](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924150848155.png)



### 小结注意

![image-20230924151922188](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924151922188.png)

注意：在 bootloader 进入 DFU 模式之前，用户不应该将 USB 线缆插入到设备中。一旦选择阶段结束，即 DFU 模式被激活后，用户可以插入 USB 线缆，另外建议在 bootloader 的启动（检测阶段）时，将未使用的 bootloader 接口的 RX 引脚（例如 USART_RX、SPI_MOSI、CAN_RX 和 USB D+/D- 线）保持在已知的电平（高或低）。如果这些引脚在检测阶段悬空，可能会导致激活未使用的接口。这意味着，为了避免误激活，应该将这些未使用的接口的接收引脚设置为确定的电平状态。





## 4.4引导加载程序的内存管理

![image-20230924153036642](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924153036642.png)

这段信息主要描述了 STM32 的 bootloader 在进行内存管理时的一些特性和限制。

### 1. **写操作的对齐：**

- 使用 bootloader 命令进行的所有写操作必须是 Word 对齐的，即地址必须是 4 的倍数。要写入的数据的数量也必须是 4 的倍数。但是，非对齐的半页写地址是被接受的。

### 2. **大量擦除操作：**

- 有些产品的 bootloader 不支持大量擦除操作。要使用 bootloader 执行大量擦除操作，有两个选项： a. 使用 Erase 命令逐个擦除所有扇区。 b. 将保护级别设置为 Level 1，然后将其设置为 Level 0（使用 Read protect 命令，然后使用 Read Unprotect 命令）。这个操作会导致内部 flash 内存的大量擦除。

​     （Level0：用户可以读取、写入和擦除 Flash 内存，也可以访问所有的 RAM 和外设）

​     （Level1：在此级别下，Flash 内存和备份寄存器的内容不能通过 JTAG/SWD 调试端口读取。）

​     （Level2：此级别提供更高级别的保护，一旦设定为 Level2，JTAG/SWD 接口将被禁用，无法连接调试   器，也无法通过这些端口读取 Flash 内存和备份寄存器的内容）



### 3. **数据内存支持：**

- STM32 L1 和 L0 系列的 bootloader 固件支持数据内存，除了标准内存（内部 flash、内部 SRAM、选项字节和系统存储器）。这个区域的起始地址和大小取决于产品，具体信息可以参考产品参考手册。数据内存可以读取和写入，但不能使用 Erase 命令擦除。写入数据内存时，bootloader 固件会管理这个位置的擦除操作。写入数据内存必须是 Word 对齐的，数据的数量也必须是 4 的倍数。要擦除数据内存位置，可以在该位置写入零。

### 4. **OTP 内存支持：**

- F2、F4、F7 和 L4 系列的 bootloader 固件支持 OTP（One-Time Programmable，一次性可编程）内存。OTP 内存可以读取和写入，但不能使用 Erase 命令擦除。写入 OTP 内存时，需要确保相关的保护位没有被重置。

### 5. **内部 Flash 内存写操作格式：**

- 对于 STM32 F2、F4 和 F7 系列，内部 flash 内存的写操作格式取决于电压范围。默认情况下，只允许以一个字节的格式进行写操作。为了提高写操作的速度，用户必须应用允许通过 Half-Word、Word 或 Double-Word 进行写操作的适当电压范围，并通过 bootloader 软件在运行时更新这个配置。



![image-20230924163136216](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924163136216.png)

### 地址描述

- `0xFFFF0000`：这个地址控制电压范围的当前值。该地址存储1字节的数据，表示不同的电压范围和操作模式。
- `0xFFFF0001`、`0xFFFF0002` 和 `0xFFFF0003`：这些地址被保留，存储1字节的数据，其默认值为`0xFF`。这些地址的其他值都不受支持，并会被NACK（非应答）。

### 电压范围

- `0x00`：[1.8 V, 2.1 V]
- `0x01`：[2.1 V, 2.4 V]
- `0x02`：[2.4 V, 2.7 V]
- `0x03`：[2.7 V, 3.6 V]
- `0x04`：[2.7 V, 3.6 V]，并且使用双字写/擦除操作。在这种情况下，必须通过VPP引脚提供9 V电压。



表格5列出了不同的内存区域以及它们对各种命令的支持：

- **Flash**：支持写、读、擦除和Go命令。
- **RAM**：支持写、读和Go命令，不支持擦除命令。
- **System memory**：仅支持读命令。
- **Data memory**：支持写和读命令，不支持擦除和Go命令。
- **OTP memory**：支持写和读命令，不支持擦除和Go命令。



### 总结：

在 STM32 中，一个 Word 是 32 位或 4 字节。Word 对齐意味着内存地址必须是 4 的倍数。例如，0x20000000、0x20000004、0x20000008 等都是 Word 对齐的地址。

不仅地址需要是 4 的倍数，写入的数据的数量（以字节为单位）也必须是 4 的倍数。这意味着，每次写操作都应该写入 4 字节、8 字节、12 字节等。

尽管有上述的对齐要求，但 bootloader 允许非对齐的半页写地址。这可能意味着，在半页范围内，写操作的起始地址可以不是 4 的倍数，但写入的数据仍然需要满足 4 的倍数要求。

### 为什么需要对齐？

- **性能：** 对齐的访问通常比非对齐的访问更快。非对齐的访问可能需要多个 CPU 周期来完成。
- **硬件要求：** 一些硬件架构只支持对齐的访问，非对齐的访问会导致硬件异常。
- **简化设计：** 对齐要求简化了硬件和软件的设计，因为它们只需要处理对齐的情况。



## 4.5引导程序波特率监测

![image-20230924164501919](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924164501919.png)

![image-20230924164521241](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924164521241.png)





描述了STM32设备上实现的两种UART接口波特率检测机制，并解释了如何处理软件抖动（jitter）。

### 两种波特率检测机制：

1. **软件波特率检测**：
   - 使用内部HSI和定时器来实现。
   - 使用GPIO作为输入，检测下降沿和上升沿。
   - 这种机制的设备受到软件抖动的影响，波特率计算的可变误差可以达到±5%。
   - 为了克服软件抖动，可以使用多次重试连接，直到收到正确的答案。
   - 降低波特率值（例如，使用56000 bps而不是115200 bps）也可以减少软件抖动。
   - 表6提供了波特率为115200 bps时最大的软件抖动值。
2. **UART自动波特率检测**：
   - 使用这种机制的设备不会出现任何软件抖动。

### 处理软件抖动：

- 主机连接到STM32 bootloader UART接口时，应支持相当于±5%的波特率偏差。
- 如果答案不正确，可以重置设备并重试连接，直到收到正确的答案。
- 一旦收到正确的答案，通信的其余部分不会受到软件抖动的影响。



## 4.6编程约束

写入 flash memory 的地址必须按照一定的规则（Table 7 中的规定）对齐，否则写入操作会失败。这里的“对齐”通常是指地址必须是某个数（例如 4 或 8）的倍数，这样的地址被认为是“对齐”的。如果地址不满足这个条件，就被认为是“不对齐”的，写入操作就会失败。

![image-20230924170443712](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924170443712.png)

![image-20230924170511521](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924170511521.png)

这里举了几个例子

### 4字节对齐：

- 地址 `0x08000014` 是4字节对齐的，因为它是4的倍数，所以写入操作会成功。
- 地址 `0x08000012` 不是4字节对齐的，因为它不是4的倍数，所以写入操作会失败。

### 8字节对齐：

- 地址 `0x08000010` 是8字节对齐的，因为它是8的倍数，所以写入操作会成功。
- 地址 `0x08000014` 不是8字节对齐的，因为它不是8的倍数，所以写入操作会失败。

在一些产品（例如 STM32F4 和 STM32F7）上，可以通过在设备特性空间中写入数据来改变对齐约束。

这意味着，在某些特定的产品上，原本需要8字节对齐的地址，可能可以通过修改设备的一些设置，使其支持4字节对齐，从而改变对齐的要求。

## 4.7安全退出内存



![image-20230924171703007](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924171703007.png)

“ExitSecureMemory feature”（退出安全内存功能）是一种安全特性，用于隔离处理敏感信息（秘密）的安全启动代码/数据与应用程序代码。

### 安全内存区域：

- 该区域用于存放处理敏感信息的安全启动代码/数据。
- 通过 FLASH_CR 寄存器中的一个可设置的位 SEC_PROT（一次性写入）来控制访问。
- 在启动时执行一次，然后通过写入 SEC_PROT 位来锁定。
- 在该安全内存区域中受保护的代码在下一次重置并解锁 SEC_PROT 位之前是隐藏的。

### 宽度和大小：

- 安全内存区域的宽度（flash memory pages 的数量）是通过一个选项字节 SEC_SIZE 在 flash memory FLASH_SEC_R 寄存器中定义的。

### ExitSecureMemory 功能：

- 当用户启动代码跳转到这个软件时，它允许将 SEC_PROT 位设置为 1，然后跳转到应用程序代码。
- 在跳转到 ExitSecureMemory 函数之前，SEC_SIZE 必须被设置为所需的值。

简而言之，这个功能允许用户在启动时执行一些安全代码，然后锁定这部分代码以防止未经授权的访问，最后跳转到应用程序代码。在跳转之前，用户需要设置好 SEC_SIZE，以确定安全内存区域的大小。

![image-20230924171720090](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924171720090.png)

![image-20230924172157264](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924172157264.png)

### 无参数跳转到安全内存函数

在这种情况下，应用程序必须紧接着定义的安全内存加载。

### 使用两个参数跳转到安全内存函数

1. **Magic number（魔数）**

   - ```
     0x08192A3C
     ```

     - 在单Bank产品的情况下，用于在 flash 中保护启动代码/数据并跳转。
     - 在双Bank产品的情况下，用于保护 Bank1 中的启动代码/数据并跳转。

   - ```
     0x08192A3D
     ```

     - 在双Bank产品的情况下，用于保护启动代码/数据并跳转到 Bank2 中的应用程序。

2. **User address = Application address（用户地址 = 应用程序地址）**

   - 在这种情况下，应用程序可以加载到任何所需的地址（按照用户定义的地址）。

### 解释：

- **无参数跳转**：这种方法下，应用程序必须紧接着安全内存区域加载，这意味着应用程序的加载地址是固定的，即安全内存区域的结束地址。
- **带参数跳转**：这种方法更为灵活，允许用户指定应用程序的加载地址，并使用“魔数”来指定启动代码/数据的保护方式和跳转的Bank（Bank1 或 Bank2）。

这两种跳转方法提供了不同级别的灵活性，以满足不同的系统需求。

### 安全退出内存的入口地址

![image-20230924172311947](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924172311947.png)

更多细节

![image-20230924172454278](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924172454278.png)

这里的流程图详细描述了跳转过程（重要）



## 4.8独立看门狗

![image-20230924174122888](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924174122888.png)



### 主要点：

1. **Bootloader 不启用 IWDG：** Bootloader 本身不会启用 IWDG，但如果 IWDG 已经通过硬件（通过选项字节）或软件（在跳转到 Bootloader 的应用中）启用，Bootloader 会尝试更新预分频值（pre-scaler value）。
2. **预分频值更新位（PVU）：** 如果在 Bootloader 启动时 IWDG 没有被启用（无论是通过硬件启动还是从应用跳转），当 Bootloader 尝试更改预分频值时，看门狗预分频值更新位（PVU）会被设置为 `0x1`。
3. **PVU 位的状态：** 由于 IWDG 没有被启用，预分频值的更新永远不会发生，因此 PVU 位的值会保持为 `0x1`，即使在从 Bootloader 跳转之后也是如此。
4. **跳转到应用程序时的考虑：** 当使用 Bootloader 跳转到应用程序，并且需要启用 IWDG 时，需要考虑到 IWDG_SR 寄存器中的 PVU 位已经被设置为 `0x1`。

### 意义：

这段描述主要是在说明，在使用 Bootloader 时，如果需要在应用程序中启用 IWDG，需要考虑到 Bootloader 本身不会启用 IWDG，并且在尝试更改预分频值时，PVU 位会被设置，并且会保持这个状态。因此，开发者需要在设计应用程序时考虑到这一点，以确保 IWDG 的正确配置和使用。

接下来通过一个具体的系列来看一下

## 52.STM32H74xxx/75xxx devices bootloader（重点）



在这里，STM32H74xxx/75xxx 系列的 bootloader 通过应用一个特定的模式，即“Pattern 10”来激活。“Pattern 10”是一种特定的配置或设置，用于激活 bootloader。这个模式的具体细节和配置应该在文档的“Table 2”中有详细描述。configuration in system memory boot mode配置为系统内存启动

![image-20230924174822025](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924174822025.png)

![image-20230924175052625](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175052625.png)



![image-20230924175308791](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175308791.png)

![image-20230924175440732](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175440732.png)

![image-20230924175451656](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175451656.png)

![image-20230924175505017](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175505017.png)

![image-20230924175515322](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175515322.png)

![image-20230924175550532](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175550532.png)

![image-20230924175613468](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175613468.png)

以上的表描述了所有的引脚与配置（重重重点）

接下来看这张图

## 引导程序选择机制

非常经典的图，过一遍流程吧，系统复位之后配置内部时钟HSI为（64M 不同系列的可能不同）

系统初始化（包括：时钟初始化，gpio初始化，独立看门狗初始化，滴答定时器初始化）

配置USB--->I2C--->SPI

监测到FD-CAN帧跳到FD-CAN-LOOP

监测到usart接受到0x7f 则禁用所有中断和其他接口时钟并配置usart,最后再loop

…………其他类似

![image-20230924175853089](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924175853089.png)

这里有个问题？

## iwdg可以不使用么？

`IWDG` 不是一个必须要使用的组件。它的使用取决于系统的需求和设计考虑。如果系统设计者认为系统在运行过程中不太可能遇到无法恢复的故障，或者系统可以容忍这种故障，那么 `IWDG` 可以不被配置和使用。

在 Bootloader 中配置 `IWDG` 可能是出于以下几个原因：

1. **防止系统卡死：** 如果 Bootloader 在加载主程序时发生故障，`IWDG` 可以重置系统，防止系统卡死。
2. **增加可靠性：** 在关键的系统和应用中，`IWDG` 可以作为一个额外的安全措施，以增加系统的可靠性。
3. **满足特定需求：** 在某些特定的应用和环境中，使用 `IWDG` 可能是一个要求或标准。

## 设备相关bootloader参数设置

由于太多了，就不贴出来了，看文档78章节

![image-20230924181609873](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924181609873.png)

![image-20230924181720609](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20230924181720609.png)





