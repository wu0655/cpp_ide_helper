# 使用说明
# 背景
- 现在bsp代码中有很多冗余。阅读代码时，经常会跳转到实际没有编译的文件中，浪费时间。

## 思路
- 分析编译器编译过程中生成的dependency文件，将实际用到的源代码导出到filelist.txt中。
- 利用filelist.txt生成cscope和ctags的数据库，提高开发效率。
- 目前测试的，支持kernel/u-boot/arm-trusted-firmware

## 实现
- 使用BS::thread_pool线程池加速。
- 静态链接，不需要其他库支持。

## 获取
- 自己编译
  - 运行all_build.sh
    - 测试环境，ubuntu22.04
- 自己下载ide_helper.zip
    - elf是x86平台的。

## 使用
- 解压ide_helper.zip，并将加入到PATH
- 参考ide_helper下的脚本，使用。

## 其他
- test_all.sh
  - 开发时的测试脚本。可以参考此脚本，配置自己kernel/u-boot/atf代码的路径。
- kernel_gen_filelist.sh
  - 生成kernel filelist.txt
  - 如果需要精细控制，参考次脚本，给kernel_helper传递参数
  - kernel的dependency文件,kernel编译时处理过的。和clang/gcc无关。
- uboot_gen_filelist.sh
  - kernel和u-boot的编译方法和生成的dependency文件格式基本一致，故底层直接使用kenrel_helper
- atf_gen_filelist.sh
  - 针对arm-trusted-firmware的脚本
  - atf生成的dependency文件是原始编译器导出的文件。和kernel不一样
  - 目前gcc编译的是ok的，clang未测试
