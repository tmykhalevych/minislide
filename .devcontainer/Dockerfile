FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC
USER root

# Default C++ dev env utils
RUN apt-get update
RUN apt-get install -y build-essential zsh git wget cmake clang-format doxygen gdb libgtest-dev

# Google Test
RUN cd /usr/src/googletest && \
    cmake . && \
    cmake --build . --target install

# zsh
RUN sh -c "$(wget -O- https://github.com/deluan/zsh-in-docker/releases/download/v1.1.2/zsh-in-docker.sh)" -- \
    -t robbyrussell \
    -p git

# Python
RUN apt install -y python3 python3-venv
RUN python3 -m venv "/root/.venv"
ENV PATH="${PATH}:/root/.venv/bin"
RUN pip install pre-commit

# Project dependencies
RUN apt-get install -y gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib gdb-multiarch \
                       pkg-config libusb-1.0-0-dev usbutils libtool

# OpenOCD
RUN mkdir /usr/src/openocd && \
    cd /usr/src/openocd && \
    git clone https://github.com/raspberrypi/openocd.git --branch picoprobe --depth=1 --no-single-branch . && \
    ./bootstrap && \
    ./configure --enable-picoprobe && \
    make CFLAGS="-Wno-misleading-indentation -Wno-stringop-overflow -Wno-format-truncation" && \
    make install
