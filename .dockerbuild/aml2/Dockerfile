# https://hub.docker.com/_/amazonlinux
FROM amazonlinux:2.0.20220121.0 AS web

# DL4006警告対応
# https://github.com/hadolint/hadolint/wiki/DL4006
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# amazon-linux-extras のインストール
RUN amazon-linux-extras install -y

# nginx install
RUN amazon-linux-extras install nginx1

# PHP install
RUN amazon-linux-extras install php8.0

# yum 関係と必要なものいろいろ入れる
RUN yum update -y && yum install -y \
    git tree glibc-langpack-ja update-motd openssh-server vim hostname which procps systemd sudo tar unzip && \
    yum clean all

# yum コンパイルに必要なライブラリを追加
RUN yum install -y php.aarch64 php-devel.aarch64 gcc make

# xml処理用ライブラリを追加
RUN yum install -y php-xml.aarch64
# RUN yum install -y expat.aarch64
# RUN yum install -y expat-devel.aarch64
# RUN yum install -y xml-common.noarch

#node.js／npm install
RUN curl -fsSL https://rpm.nodesource.com/setup_16.x | bash -
RUN yum install -y nodejs

# # composer install
# RUN curl -sS https://getcomposer.org/installer | php
# RUN mv composer.phar /usr/local/bin/composer

# # man コマンドを使えるようにする
# # https://zenn.dev/okz/articles/39d7faa7a6d59a
# RUN yum -y --setopt=tsflags='' install man-pages-ja man-pages-dev-ja && yum clean all

# 日本語対応
ENV LANG ja_JP.utf8
ENV LC_ALL ja_JP.utf8
ENV TZ Asia/Tokyo
RUN yum reinstall -y glibc-common

# vim の設定ファイル
# root と ec2-user 両方に反映させたいのでスケルトンにも追加しておく
COPY .vimrc /root/.vimrc
COPY .vimrc /etc/skel/.vimrc

# SSH接続用設定
# PasswordAuthentication と Port の設定を変更
RUN mkdir -m 700 /root/.ssh && mkdir -m 700 /etc/skel/.ssh
RUN sed -ri 's/^#PasswordAuthentication yes/PasswordAuthentication no/' /etc/ssh/sshd_config \
  && sed -ri 's/^PasswordAuthentication yes/#PasswordAuthentication yes/' /etc/ssh/sshd_config \
  && sed -ri 's/^#Port 22/Port 20022/' /etc/ssh/sshd_config \
  && echo 'root:password' | chpasswd

# motd の更新を実行
# 一応 enable も指定しておく
RUN update-motd --enable && update-motd

# ec2-user の作成
# 本物らしくパスワードなしで sudo が使えるようにしておく
ARG USERNAME=ec2-user
RUN useradd -m -s /bin/bash $USERNAME && echo "$USERNAME ALL=NOPASSWD: ALL" >> /etc/sudoers

# nginx DocumentRoot変更
RUN sed -i -e "s/root         \/usr\/share\/nginx\/html;/root \/usr\/local\/app\/html;/" /etc/nginx/nginx.conf

# nginx 自動起動設定
RUN systemctl enable nginx
RUN systemctl is-enabled nginx

# 正常にコンパイルとインストールが実行されないので以下は保留
# # jcomsia_hash_lib.soのコンパイルとインストール
# RUN cd /usr/local/app/src \
#   && phpize \
#   && ./configure \
#   && make \
#   && make install

# # jcomsia_hash_lib.soをコピー
# COPY ../src/modules/jcomsia_hash_lib.so /usr/lib64/php/modules

# # jcomsia_hash_lib.soの読み込み設定ファイルをコピー
# COPY 40-jcomsia_hash_lib.ini /etc/php.d/

# systemd実行対応用
VOLUME [ "/sys/fs/cgroup" ]
CMD ["/usr/sbin/init"]
