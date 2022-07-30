FROM netricks-20.04-env

ADD . /root/igris

WORKDIR /root/igris
RUN ./make.py
RUN sudo ./make.py install

WORKDIR /root/igris/tests
RUN ./make.py
RUN ./runtests