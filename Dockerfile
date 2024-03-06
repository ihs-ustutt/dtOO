FROM atismer/dtoo-base:latest

SHELL ["/bin/bash", "-c", "-l" ]


WORKDIR /
RUN git clone https://github.com/ihs-ustutt/dtOO.git

WORKDIR /dtOO

RUN git submodule init
RUN git submodule update
RUN mkdir build

WORKDIR /dtOO/build
RUN cmake -DCMAKE_INSTALL_PREFIX=${DTOO_EXTERNLIBS} -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPython_EXECUTABLE=/usr/bin/python3.6 ..
RUN make -j ${NCPU} install
RUN ctest -j ${NCPU} -L base
RUN ctest -j ${NCPU} -L gcc-12

ENV PYTHONPATH=/dtOO-install/tools:/dtOO-install/scripts/python
WORKDIR /dtOO/demo/
