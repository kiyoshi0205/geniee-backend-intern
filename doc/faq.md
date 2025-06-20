# FAQ

## 検証テストのレスポンスチェックが失敗した
検証バッチはレスポンスを [`./test/result.json`](../test/result.json) に吐き出したのち、`auction_id` でソートします。  
そのため例えばsmallデータセットを用いた場合、
```shell
user@mac$ diff ~/geniee-backend-intern/data/response_123_small.json ~/geniee-backend-intern/test/result.json
```
のように実行することで、どのデータがずれていたか確認することができます。

## 仮想環境が固まった
仮想環境をリスタートします。

[`./setup/restart-server.sh`](../setup/restart-server.sh) を実行してください。
```shell
user@mac$ ~/geniee-backend-intern/setup/restart-server.sh
```  
上記コマンドを実行すると再度仮想環境に入った状態でvscodeの画面が起動します。

各種インストールしたライブラリや設定は基本的には保存されたままです。
