export PATH=./build:$PATH
time kernel_helper --code ~/work/nxp/bsp38_porting/u-boot/ --build ~/work/nxp/bsp38_porting/u-boot/out --extname ".o.cmd" --out code.txt
time kernel_helper --code ~/work/nxp/bsp38_porting/u-boot/ --build ~/work/nxp/bsp38_porting/u-boot/out/arch --extname ".dtb.cmd" --out dtb.txt
merge ./code.txt ./dtb.txt -o ./filelist.txt
