_build_dir=$1
_code_dir=${_build_dir}/source

find "${_build_dir}" -name "*.o.cmd" > ./filelist.txt
find "${_build_dir}/arch/arm64/boot/" -name "*.dtb.cmd" >> ./filelist.txt
kern_dep_analyzer --in ./filelist.txt --build "${_build_dir}" --out ./filelist.txt
check_dep --in ./filelist.txt --code "${_code_dir}" --build "${_build_dir}" --out ./filelist.txt
