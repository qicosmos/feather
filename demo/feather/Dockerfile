FROM ubuntu:latest AS builder

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu\//g' /etc/apt/sources.list \
  && sed -i 's/http:\/\/security\.ubuntu\.com\/ubuntu/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu\//g' /etc/apt/sources.list \
  && apt-get update \
  && apt-get -y install \
        cmake \
        gcc \
        git \
        make \
        libpqxx-dev \
        libboost-filesystem-dev \
        libboost-system-dev \
        libmysqlclient-dev \
        openssl \
        sqlite \
        uuid-dev \
        zlib1g-dev

RUN cd /tmp \
  && git clone https://github.com/qicosmos/feather \
  && cd feather \
  && git submodule update --init \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make CXX_FLAGS+="-std=c++17"

FROM ubuntu:latest

COPY --from=builder /tmp/feather/build/feather /opt/feather/
COPY --from=builder /tmp/feather/purecpp /opt/feather/purecpp
COPY --from=builder /tmp/feather/feather.sql /opt/feather/feather.sql

RUN sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu\//g' /etc/apt/sources.list \
  && sed -i 's/http:\/\/security\.ubuntu\.com\/ubuntu/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu\//g' /etc/apt/sources.list \
  && apt-get update \
  && apt-get -y install \
        libboost-system1.65.1 \
        libmysqlclient20 \
        mysql-client \
  && rm -rf /var/lib/apt/lists/*

COPY ./data/ ./entrypoint.sh /opt/feather/

WORKDIR /opt/feather

ENTRYPOINT ["bash", "./entrypoint.sh"]
