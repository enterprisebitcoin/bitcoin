FROM ubuntu:latest

ENV TZ=US/Central
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get -y upgrade && apt-get -y install zsh bash nano tmux git build-essential libtool \
    autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev \
    libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev python3 python3-pip \
    libpq-dev python3-dev build-essential postgresql-server-dev-all

RUN apt-get -y install libodb-2.4 libodb-dev libodb-pgsql-2.4 libodb-pgsql-dev libpq5 libodb-boost-2.4 libodb-boost-dev

RUN git clone https://github.com/enterprisebitcoin/bitcoin.git bitcoin


RUN cd bitcoin && git checkout enterprise-0.18-inoutdata && git pull && ./autogen.sh

RUN cd bitcoin && ./configure --disable-wallet --without-gui --enable-debug --with-incompatible-bdb --with-miniupnpc=no

RUN cd bitcoin && make clean && make -j 8 && make install

ARG BUILD_VERSION
ENV BUILD_VERSION=$BUILD_VERSION
RUN echo Build Time: $BUILD_VERSION

RUN git clone https://github.com/PierreRochard/enterprisebitcoin-db.git enterprisebitcoin-db

RUN pip3 install -r enterprisebitcoin-db/requirements.txt

RUN pip3 install git+https://github.com/pierrerochard/general#egg=general
RUN pip3 install git+https://github.com/pierrerochard/enterprisebitcoin-db#egg=enterprise_bitcoin

COPY ".env" bitcoin/

