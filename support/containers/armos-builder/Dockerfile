FROM ubuntu:22.04 as build
LABEL org.opencontainers.image.source https://github.com/tristanred/ArmOS

ENV DEBIAN_FRONTEND=noninteractive

COPY setup.sh /setup.sh
COPY build.sh /build.sh

RUN bash /setup.sh

ENV PREFIX="/ArmOS/opt/cross"
ENV TARGET=arm-none-eabi
ENV PATH="$PREFIX/bin:$PATH"

WORKDIR /buildsrc
RUN bash /build.sh

FROM ubuntu:22.04

# Have to redo env ?
ENV PREFIX="/ArmOS/opt/cross"
ENV TARGET=arm-none-eabi
ENV PATH="$PREFIX/bin:$PATH"

COPY --from=build "/ArmOS" "/ArmOS"