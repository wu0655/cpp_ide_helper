export PATH=./build:$PATH
time atf_helper --code ~/work/nxp/bsp38_porting/arm-trusted-firmware/ --build ~/work/nxp/bsp38_porting/arm-trusted-firmware/build --extname ".d" --out filelist.txt

