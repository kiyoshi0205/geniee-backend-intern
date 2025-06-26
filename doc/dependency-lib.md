# 依存ライブラリ
すでに自動でinstallされているため参考程度に記しておきます。この辺りの詳細を理解して高速化する必要はありません。

## C++
### MySQL Connector C++
- https://dev.mysql.com/doc/connector-cpp/1.1/en/

DBに接続し、MySQLのデータを取得できる公式ライブラリです。

### RapidJSON
- https://github.com/Tencent/rapidjson
- https://rapidjson.org/index.html

JSON形式のリクエストを解析したり、レスポンスをJSONにして返すために用いているライブラリです。

### fmtlib
- https://github.com/fmtlib/fmt.git

C++20以降で使用可能なstd::formatの互換ライブラリです。GCC12では対応していないのでこちらを用意しました。

### libevent
- http://www.wangafu.net/~nickm/libevent-2.0/doxygen/html/

イベント通知で管理するライブラリです。ここではWebサーバを実現するために使用しています。


## Python
### MySQL-Connector-Python
- https://dev.mysql.com/doc/connector-python/en/

DBに接続し、MySQLのデータを取得できる公式ライブラリです。

## Golang
### Go-MySQL-Driver
- https://github.com/go-sql-driver/mysql

DBに接続し、MySQLのデータを取得できる公式ライブラリです。

### SQLx
- https://github.com/jmoiron/sqlx

上記Go-MySQL-Driverを使いやすくしたラッパーライブラリです。
