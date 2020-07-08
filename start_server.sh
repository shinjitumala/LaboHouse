die() {
    trap - SIGINT SIGQUIT
    pkill -P $$
}

make -j17
if [[ $? != 0 ]]; then exit 0; fi

trap die SIGINT SIGQUIT
ssh -TNCR 12271:localhost:12345 R &
./LaboHouse 
pkill -P $$
