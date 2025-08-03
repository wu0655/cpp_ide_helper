_out_dir=$1
_code_dir=${_out_dir}/source

kernel_helper --code "${_code_dir}" --build "${_out_dir}" --extname ".o.cmd" --out .tmp.code.txt
kernel_helper --code "${_code_dir}" --build "${_out_dir}/arch/arm64/boot/" --extname ".dtb.cmd" --out .tmp.dtb.txt
merge ./.tmp.code.txt ./.tmp.dtb.txt -o ./filelist.txt
rm -f ./.tmp.code.txt ./.tmp.dtb.txt

