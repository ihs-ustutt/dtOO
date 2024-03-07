ARG TAG=latest
FROM atismer/dtoo-base:${TAG}

SHELL ["/bin/bash", "-c", "-l" ]

ENV NCPU=4

WORKDIR /
RUN git clone https://github.com/ihs-ustutt/dtOO.git

WORKDIR /dtOO
ARG GIT_REV=main
RUN git checkout ${GIT_REV}

RUN git submodule init
RUN git submodule update
RUN mkdir build

WORKDIR /dtOO/build
RUN cmake -DCMAKE_INSTALL_PREFIX=${DTOO_EXTERNLIBS} -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPython_EXECUTABLE=/usr/bin/python3.6 ..
RUN make -j ${NCPU} install
RUN ctest -j ${NCPU} -L base
RUN ctest -j ${NCPU} -L gcc-$(${CC} -dumpversion)

ENV PYTHONPATH=/dtOO-install/tools:/dtOO-install/scripts/python
WORKDIR /dtOO/demo/
