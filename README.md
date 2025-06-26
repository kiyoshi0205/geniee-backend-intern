# Geniee Speedup Challenge
広告配信の流れを単純化した簡易プログラムを高速化して速度を競いましょう！

コマンドの実行において
```shell
user@mac$ command
```
は `command` をMacのターミナルで実行されることを意味します。 `@mac` をつけています。

---
## 環境整備
`setup/setup-server.sh`を実行してください

```shell
user@mac$ ~/geniee-backend-intern/setup/setup-server.sh
```

vscode上でファイルを編集したい場合は以下のコマンドを実行してください(推奨)
```shell
user@mac$ code ~/geniee-backend-intern
```

## 操作方法
* [コマンドについて](./doc/command.md)

## 仕様
* [DBについて](./doc/db.md)
* [サイズ抽出について](./doc/size-scan.md)
* [ドメインブロック制御について](./doc/domain-blocking.md)
* [categotyBlockingsについて](./doc/category-blocking.md)
* [featureTargetingについて](./doc/feature-targeting.md)
* [最終オークションについて](./doc/final-auction.md)
* [リクエストについて](./doc/request.md)
* [レスポンスについて](./doc/response.md)

## 初期実装について
* [使用している外部ライブラリについて](./doc/dependency-lib.md)
* [各種ファイルの意味について](./doc/impl.md)

## 評価
* [データセットの種類について](./doc/dataset.md)
* [評価方法について](./doc/test.md)
* [提出について](./doc/submit.md)


## その他
* [初期実装 & Write解スコア](./doc/final-score.md)
* [FAQ](./doc/faq.md)

---

## ファイル/ディレクトリ詳細
| ファイル/ディレクトリ名称 | 説明 |
|-----------------------|-----|
| cpp/ | C++プログラムで記載されている処理本体が格納されています |
| golang/ | Golangプログラムで記載されている処理本体が格納されています |
| python/ | Pythonプログラムで記載されている処理本体が格納されています |
| doc/ | セットアップの方法や仕様、既存コードの説明など細かいドキュメント置き場 | 
| data/ | 配信の検証に用いるサンプルデータ。 | 
| setup/ | セットアップや再起動時に実行するshellファイルを置いたディレクトリ |
| sql/ | 配信に用いるスキーマ定義とSQLサンプルデータ | 
| test/ | スコア計算用プログラム。作成されたサーバが条件を満たしているかを検証しスコアを計算します。 **このディレクトリは変更してはいけません**| 
| docker-compose.yaml Dockerfile | 開発環境 兼 スコア検証環境 |
|prometheus.yml|ダッシュボード作成環境  |
| Makefile | コンパイルやスコア検証などコマンドが定義されています |
| git-web.webloc | このgitをブラウザで表示するためのwebloc |
