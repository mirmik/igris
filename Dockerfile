ARG ARCH
FROM netricks/netricks-20.04-env:$ARCH

ADD . /root/igris

WORKDIR /root/igris
RUN ./make.py
RUN ./runtests
RUN sudo ./make.py install