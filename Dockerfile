FROM testlayer

RUN git clone https://github.com/mirmik/igris
RUN cd igris && ./make.py
RUN cd igris && ./make.py install
RUN cd igris/tests && ./make.py 
RUN cd igris/tests && ./runtests 