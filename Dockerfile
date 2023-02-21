FROM alpine:latest

RUN apk update

RUN apk add build-base
RUN apk add pkgconfig
RUN apk add openssl-dev
RUN apk add gtest-dev
RUN apk add glog-dev
RUN apk add boost-dev

COPY . /blaze

# RUN apk add cmake
#
# WORKDIR /blaze/build
# RUN cmake .. -DCMAKE_BUILD_TYPE=RELEASE -D_BLAZE_CLI=ON -D_BLAZE_TEST=ON
# RUN make -j all test install

RUN apk add meson

WORKDIR /blaze
RUN meson setup build --buildtype=release

WORKDIR /blaze/build
RUN meson compile
RUN meson test
RUN meson install

ENTRYPOINT ["blaze"]
