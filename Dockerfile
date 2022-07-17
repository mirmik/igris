FROM testlayer

RUN git clone https://github.com/mirmik/igris
RUN cd igris && ./make.py
RUN cd igris && ./make.py install