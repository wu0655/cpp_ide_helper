_build_dir=$2
_code_dir=$1

find "${_build_dir}" -name "*.d" > ./filelist.txt

kern_dep_analyzer --in ./filelist.txt --code "${_code_dir}" --out ./filelist.txt


