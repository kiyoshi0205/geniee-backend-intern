# コマンド

## 言語選択
このリポジトリには複数の言語で書かれたサンプル実装があります。現状対応している言語は以下の3つです。括弧内はバージョンなどの情報です。
- C++ (C++20 コンパイラ: gcc12.3.0)
- Python3 (3.12.5)
- Golang (1.22.2)

現状、言語選択はルートディレクトリの `Makefile` にて選択されているものが使用されます。デフォルトはC++です。
他の言語に変更したい場合は、お好きなエディタで使用言語を切り替えてください。

例えば使用言語をPythonに変更したい場合は、`Makefile`にて次のように `#` で不必要な設定をコメントアウトしてください。
```diff
-LANGUAGE = cpp
-# LANGUAGE = python
+# LANGUAGE = cpp
+LANGUAGE = python
 # LANGUAGE = golang
```

上記3言語以外でもチャレンジすることは可能です。その場合、講師に使用したい言語をお伝えください。

---

## コンパイル
プログラムを編集したあと、(コンパイル言語を選択した方は)実行形式にするためにコンパイルする必要があります。コマンドは以下です。
```shell
user@mac$ make build
```
実行ファイルとして `auction-worker` が生成されます。

---

## 実行
必要に応じてコンパイルをしたのち、以下のコマンドを叩くと各言語に応じたサーバが起動します。デバッグや動作確認にお使いください。
( `make run` 時に現在起動しているプロセスを停止するため、複数起動できないことに注意してください)
```shell
user@mac$ make run
```

この状態になると外部からリクエストを飛ばすことができるようになります。(のちの`DBデータの初期化`部分で説明するように、事前にDBを何かしらの方法で初期化する必要があります)  
例えば `./data` 配下にテストリクエストを置いているので、`curl` コマンドでサーバにリクエストを送信できます。
```shell
user@mac$ curl -v -X POST -d @"$HOME/geniee-backend-intern/data/request.json" localhost:6562/auction
```

こんな感じのリクエストが帰って来れば成功です。こちらは `SEED=123` 、 `DATASET=small` 状態のDBでの結果です。
```json
{"auction_id":"xxxxxxxxxxx","ads":"4RmUoVPrWHaAN7JvdkGMJhXz2eN1xPZ1oy0Yja6gsVHAKWBjcgd22iWZXPals9z3","price":93.3957}
```

---

## 検証
すでにコンパイル済みの `auction-worker` か実行可能なプログラムがあると想定して検証プログラムを走らせます。起動は検証バッチが担います。コマンドは以下です。
```shell
user@mac$ make check
```
以下のような出力がされ、テスト結果が表示されます。詳しくは評価をご覧ください。
```
docker exec -it geniee-speedup-2025-app make -C /tmp/intern inner-check
make: Entering directory '/tmp/intern'
./test/check_performance_all.sh 123
seed = 123
--------------------small--------------------
init DB data...   done.
run auction-worker...
wait 5 second for setup auction worker...
[####################] 10/10 (0.49 req/sec)
response check PASSED
elapsed time: 20.2705 sec. performance check Time Limit Exceeded...
FAILED...
make: *** [Makefile:54: inner-check] Error 1
make: Leaving directory '/tmp/intern'
make: *** [check] Error 2
```

検証時に以下のコマンドが出た際には
- サーバが起動していない (実行時エラーなどでサーバが終了している)
- 5秒の準備期間内にサーバが受付可能状態になっていない

などが考えられます。

```
[--------------------] 0/10 (0.0 req/sec)
Server down detected. Check your log files (test/stdout.log, test/stderr.log)
```

具体的なエラーはサーバ起動画面や、 [`test/stdout.log`](../test/stdout.log) 、 [`test/stderr.log`](../test/stderr.log) に表示されるログを確認してください。

---

## DBデータの初期化
特定のデータをDBにセットしたい場合、以下のコマンドで初期化できます。
```shell
user@mac$ make setup-db SEED={seed} DATASIZE={datasize}
```
`seed` はデータ生成に用いる乱数のシード値、 `{datasize}` は生成するデータの規模を表します。
`datasize` では `small` 、 `medium` 、 `large` の3つが使用できます。

検証用データのseedは `123` で統一していますから、
```shell
user@mac$ make setup-db SEED=123 DATASIZE=small
```
のように実行を行うと初期化可能です。

ちなみに `make check` を行うと、最後に実行された規模のデータセットがセットされます。

---

## 単体の検証
`make check` は規模の小さいデータセットから評価を順に行います。  
もし規模の大きいものを試したい場合、検証バッチが叩いているプログラムを直接実行することで可能です。  
ただしあらかじめ手動で `auction-worker` を起動しておく必要があることに注意してください。  
コマンドは以下の通りです。
```shell
user@mac$ make calc-perf SEED={seed} DATASIZE={datasize}
```
`seed` は同じくシード、 `dataset` はリクエスト送信の規模を表します。
`datasize` として `small` 、 `medium` 、 `large` 、 `challenge` の4つが使用できます。

同じく検証用データのseedは `123` で統一しているため、
```shell
user@mac$ make calc-perf SEED=123 DATASIZE=small
```
のように実行を行うと個別評価が行えます。

別シードで検証を行うと、該当 `seed` における正解ファイルが存在しないため、レスポンスの確認で失敗します。
そのため
```shell
user@mac$ make calc-perf SEED=123 DATASIZE=small OPTION=--skip-validation
```
のように `OPTION=--skip-validation` オプションをつけるとレスポンスの正当性確認を行わず、速度のみを測ることができます。

--- 

## MySQLサーバログイン
データベースの情報を見たい場合は以下のコマンドでログインすることができます。
```shell
user@mac$ make login-mysql
```
データセット名は `geniee` を使っています。ログイン後任意のコマンドを実行することが可能です。

--- 

## コンテナにログイン
Dockerコンテナに入る場合は以下のコマンドでログインすることができます
```shell
user@mac$ make login-container
```
これによりコンテナ中の`/tmp/intern`配下にアクセスすることができます。

ログアウトする際には`Ctrl + D`を押す、もしくは`exit`と入力してください。
