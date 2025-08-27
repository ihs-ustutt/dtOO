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

FROM ${CONTAINER}:${TAG} AS base-clone
ARG CBASE=
COPY --from=repo /dtOO${CBASE} /dtOO
SHELL ["/bin/bash", "-c", "-l" ]

ARG NCPU=4

WORKDIR /dtOO
RUN mkdir build

FROM base-clone AS base
COPY --from=base-clone /bin             /bin
COPY --from=base-clone /boot            /boot
COPY --from=base-clone /dev             /dev
COPY --from=base-clone /dtOO-install    /dtOO-install
COPY --from=base-clone /etc             /etc
COPY --from=base-clone /foamFine        /foamFine
COPY --from=base-clone /home            /home
COPY --from=base-clone /lib             /lib
COPY --from=base-clone /lib64           /lib64
COPY --from=base-clone /mnt             /mnt
COPY --from=base-clone /opt             /opt
COPY --from=base-clone /proc            /proc
COPY --from=base-clone /root            /root
COPY --from=base-clone /run             /run
COPY --from=base-clone /sbin            /sbin
COPY --from=base-clone /selinux         /selinux
COPY --from=base-clone /srv             /srv
COPY --from=base-clone /sys             /sys
COPY --from=base-clone /tmp             /tmp
COPY --from=base-clone /usr             /usr
COPY --from=base-clone /var             /var
COPY --from=base-clone /dtOO            /dtOO

SHELL ["/bin/bash", "-c", "-l" ]

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

#FROM base-test AS base
#COPY --from=repo /dtOO /dtOO
#SHELL ["/bin/bash", "-c", "-l" ]
#
#WORKDIR /dtOO/build
#RUN ctest --output-on-failure -L base
#RUN ctest --output-on-failure -L gcc-$(${CC} -dumpversion)

