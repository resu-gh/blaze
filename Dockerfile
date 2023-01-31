from alpine:latest

run apk update

run apk add build-base
run apk add pkgconfig
run apk add openssl-dev
run apk add gtest-dev
run apk add glog-dev
run apk add boost-dev

copy . /blaze

# run apk add cmake
#
# workdir /blaze/build
# run cmake .. -DCMAKE_BUILD_TYPE=RELEASE -D_BLAZE_CLI=ON -D_BLAZE_TEST=ON
# run make -j all test install

run apk add meson

workdir /blaze
run meson setup build --buildtype=release

workdir /blaze/build
run meson compile
run meson test
run meson install

entrypoint ["blaze"]
