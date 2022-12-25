FROM ubuntu:22.04

COPY setup.sh /setup.sh
COPY build.sh /build.sh

RUN bash /setup.sh

ENV PREFIX="/ArmOS/opt/cross"
ENV TARGET=arm-none-eabi
ENV PATH="$PREFIX/bin:$PATH"

WORKDIR /buildsrc
RUN bash /build.sh