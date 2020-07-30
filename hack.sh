set -eux
make -j48 && make install
(
    cd ~/work/libc-test
    make clean && make -j48
    make sum
)

failed() {
    (cd ~/work/libc-test; make less)
}

test_log() {
    less ~/work/libc-test/src/REPORT
}
