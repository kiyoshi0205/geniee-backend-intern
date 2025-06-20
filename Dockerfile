FROM mirror.gcr.io/ubuntu:22.04

# 日本標準時刻を設定
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


# 必要なパッケージをインストール
RUN apt-get update \
    && apt-get install -y software-properties-common \
    && add-apt-repository -y ppa:deadsnakes/ppa \
    && add-apt-repository -y ppa:prometheus-cpp/prometheus-cpp-stable \
    && apt-get update \
    && apt-get install -y \
    libevent-dev \
    libmysqlcppconn-dev \
    prometheus-cpp-dev \
    make \
    g++-12 \
    gdb \
    linux-tools-generic \
    zip \
    mysql-server \
    python3.12 \
    python3.12-venv \
    golang-1.22

# コマンドラインツールをインストール
RUN apt-get install -y \
    sudo \
    curl \
    htop \
    tree \
    wget \
    vim \
    lsof \
    procps \
    net-tools \
    iputils-ping \
    dnsutils \
    git \
    sysstat \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを設定
WORKDIR /app

# MySQLの設定
RUN service mysql start && \
    echo "ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password by 'r4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ';" | mysql -uroot

# Pythonの仮想環境を設定
RUN python3.12 -m venv /root/.venv

# 環境変数を設定
ENV PATH="/root/.venv/bin:$PATH"

# コンテナ起動時にMySQLを起動
CMD service mysql start && bash
