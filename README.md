# JFlash RamCode
IAR project for jflash ram code

# 原理
- 1 **JFlash** 将flash烧录算法转换为运行在**RAM**上的RamCode
- 2 等待数据，使用相应的`flash_write`、`flash_read`,`flash_erase`等API接口将数据写入falsh

# 扩展
使用**JFlash**添加多个falsh，针对不同的**Flash**添加多个Bank即可

# 支持的Flash
- **XT25F16B**