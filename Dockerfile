ARG CONTAINER=atismer/dtoo-base
ARG TAG=latest

FROM alpine/git AS repo
WORKDIR /
RUN git clone https://github.com/ihs-ustutt/dtOO.git
WORKDIR /dtOO
ARG GIT_REV=main
RUN git checkout ${GIT_REV}
RUN git submodule init
RUN git submodule update
COPY . /dtOO.local

FROM ${CONTAINER}:${TAG} AS base
ARG CBASE=
COPY --from=repo /dtOO${CBASE} /dtOO
SHELL ["/bin/bash", "-c", "-l" ]

ARG NCPU=4

WORKDIR /dtOO
RUN mkdir build

WORKDIR /dtOO/build
RUN cmake \
  -DCMAKE_INSTALL_PREFIX=${DTOO_EXTERNLIBS} \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DPython3_EXECUTABLE=/usr/bin/python3.11 \
  -DDTOO_OCC_78=ON \
  ..
RUN make -j ${NCPU} install
RUN ctest --output-on-failure -L base
RUN ctest --output-on-failure -L gcc-$(${CC} -dumpversion)

ENV PYTHONPATH=/dtOO-install/tools:/dtOO-install/scripts/python
WORKDIR /dtOO/demo/
