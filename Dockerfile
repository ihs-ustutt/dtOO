ARG CONTAINER=atismer/dtoo-base-opensuse
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
  -DPython3_EXECUTABLE=$(which python3.12) \
  -DDTOO_OCC_78=ON \
  ..
RUN make -j ${NCPU} install

ENV PYTHONPATH=/dtOO-install/tools:/dtOO-install/scripts/python
WORKDIR /dtOO/demo/
