FROM ubuntu:19.04
RUN apt update && apt install -y wget xz-utils build-essential g++ libncurses-dev libz-dev libz3-dev git libboost-dev curl ninja-build cmake

# build gtest
RUN git clone --branch release-1.10.0 https://github.com/google/googletest /root/gtest
RUN cmake -B /root/gtest-build -DCMAKE_INSTALL_PREFIX=/root/gtest-install -G Ninja -S /root/gtest
RUN ninja -C /root/gtest-build install
ENV GTEST_SYSROOT /root/gtest-install

# add clang
ENV CLANG_VERSION clang+llvm-9.0.0-x86_64-pc-linux-gnu
ENV CLANG_SYSROOT /clang9
RUN wget -qO- http://releases.llvm.org/9.0.0/${CLANG_VERSION}.tar.xz | unxz | tar -x
RUN mv ${CLANG_VERSION} ${CLANG_SYSROOT}

# deal with clang9's libz3 incorrect paths
RUN ln -s /usr/lib/x86_64-linux-gnu/libz3.so /usr/lib/libz3.so.4.8
RUN ln -s /usr/lib/x86_64-linux-gnu/libz3.so /usr/local/lib/libz3.so