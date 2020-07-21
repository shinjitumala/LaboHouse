die() {
    trap - SIGINT SIGQUIT
    pkill -P $$
}

make -j17
if [[ $? != 0 ]]; then exit 0; fi

trap die SIGINT SIGQUIT
ssh -TNCR 12271:localhost:12345 R &
ssh -TNCR 12273:localhost:42069 R &
ASAN_OPTIONS="abort_on_error=1:disable_coredump=0" ./LaboHouse 
pkill -P $$
