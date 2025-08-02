export PATH=./build:$PATH
time kernel_helper --code ~/work/nxp/bsp38_porting/linux-yocto-5.15/ --build ~/work/nxp/bsp38_porting/linux-yocto-5.15/out --extname ".o.cmd" --out code.txt
time kernel_helper --code ~/work/nxp/bsp38_porting/linux-yocto-5.15/ --build ~/work/nxp/bsp38_porting/linux-yocto-5.15//out/arch/arm64/boot/ --extname ".dtb.cmd" --out dtb.txt
merge ./code.txt ./dtb.txt -o ./filelist.txt
