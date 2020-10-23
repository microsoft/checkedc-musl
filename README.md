# About
This is a version of [musl libc](http://www.musl-libc.org) that has been
converted to [Checked C](https://github.com/Microsoft/checkedc). The sources
here have been mirrored from [this](https://git.musl-libc.org/cgit/musl) repo.
This version of `musl` is meant to compile with the
[checkedc-clang](https://github.com/microsoft/checkedc-clang) compiler.

## Build musl with Checked C clang
```
export PATH=</path/to/checkedc-clang/bin>
TOP_DIR=/some/dir
cd $TOP_DIR
git clone https://github.com/microsoft/checkedc-musl.git src
mkdir build && cd build
CC=clang CFLAGS=-fPIC LDFLAGS=-fPIC ../src/configure --prefix=$PWD --disable-shared
make clean && make -j32 && make install
```

## Test musl with libc-test
[libc-test](https://wiki.musl-libc.org/libc-test.html) is meant to test musl.
We have a version of `libc-test` called
[checkedc-libc-test](https://github.com/microsoft/checkedc-libc-test) that has
been modified to test `checkedc-musl`. Refer to that repo for the build steps
for `checkedc-libc-test`.

Note: Before building `checkedc-libc-test` make sure you set the following
environment variable:
```
export MUSL_PATH=$TOP_DIR/build
```
