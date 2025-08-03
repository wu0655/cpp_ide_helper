_build_dir=$2
_code_dir=$1

atf_helper --code "${_code_dir}" --build "${_build_dir}" --extname ".d" --out filelist.txt


