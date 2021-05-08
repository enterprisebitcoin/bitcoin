FROM ubuntu:latest

ENV TZ=US/Central
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get -y upgrade && apt-get -y install zsh bash nano tmux git build-essential libtool \
    autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev \
    libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev python3 python3-pip \
    libpq-dev python3-dev build-essential postgresql-server-dev-all libodb-2.4 libodb-dev libodb-pgsql-2.4 \
    libodb-pgsql-dev libpq5 libodb-boost-2.4 libodb-boost-dev libvips libvips-dev


ARG BUILD_VERSION
ENV BUILD_VERSION=$BUILD_VERSION
RUN echo Build Time: $BUILD_VERSION

RUN git clone https://github.com/PierreRochard/enterprisebitcoin-db.git enterprisebitcoin-db

RUN pip3 install -r enterprisebitcoin-db/requirements.txt

RUN pip3 install git+https://github.com/pierrerochard/general#egg=general
RUN pip3 install git+https://git@github.com/pierrerochard/enterprisebitcoin-db#egg=enterprise_bitcoin

COPY ".env" enterprisebitcoin-db/

# Copy the entrypoint script.
COPY "start-tweeter.sh" .
RUN chmod +x start-tweeter.sh
