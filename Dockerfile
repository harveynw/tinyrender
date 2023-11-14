# NOTE! If building this on an ARM machine, you will need to pass: docker build --platform linux/amd64, for now.
FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive 

# CMake
RUN apt-get update && apt-get -y install build-essential cmake protobuf-compiler libx11-dev xorg-dev openbox git

# Building tinyrender
WORKDIR ./tinyrender
COPY / .

RUN if [ -d "build" ]; then rm -Rf build; fi
RUN if [ -d "build-web" ]; then rm -Rf build-web; fi

RUN cmake -S . -B build
RUN cmake --build build
