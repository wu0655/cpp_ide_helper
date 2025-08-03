echo "++++atf test+++++"
./atf_gen_filelist.sh ~/work/nxp/bsp38_porting/arm-trusted-firmware/ ~/work/nxp/bsp38_porting/arm-trusted-firmware/build
mv ./filelist.txt ./atf_list.txt
# shellcheck disable=SC2028
echo "++++uboot test+++++"
./uboot_gen_filelist.sh ~/work/nxp/bsp38_porting/u-boot/out/
mv ./filelist.txt ./uboot_list.txt
echo "++++kernel test+++++"
./kernel_gen_filelist.sh ~/work/nxp/bsp38_porting/linux-yocto-5.15/out
mv ./filelist.txt ./kernel_list.txt