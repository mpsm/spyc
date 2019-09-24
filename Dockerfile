FROM ubuntu:19.04
RUN apt update
RUN apt install -y wget xz-utils build-essential g++ libncurses-dev libz-dev libz3-dev git libgtest-dev libboost-dev
RUN ln -s /usr/lib/x86_64-linux-gnu/libz3.so /usr/lib/libz3.so.4.8

ENV CLANG_VERSION clang+llvm-9.0.0-x86_64-pc-linux-gnu
ENV CLANG_SYSROOT /clang9
RUN wget -qO- http://releases.llvm.org/9.0.0/${CLANG_VERSION}.tar.xz | unxz | tar -x
RUN mv ${CLANG_VERSION} ${CLANG_SYSROOT}