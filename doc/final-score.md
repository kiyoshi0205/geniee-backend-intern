# スコア情報

## 初期実装

参考までに小規模データセットを Writer の PC 上で実行した時のスコアを残します。0.26 req/sec 程度。(C++)

```
--------------------small--------------------
init DB data...   done.
run auction-worker...
wait 5 second for setup auction worker...
[####################] 10/10 (0.26 req/sec)
response check PASSED
elapsed time: 39.2654 sec. performance check Time Limit Exceeded...
FAILED...
```

---

## Write の結果

チャレンジデータセットの結果。1923 req/sec 程度。(C++)

```
--------------------challenge--------------------
run auction-worker...
wait 5 second for setup auction worker...
[####################] 5000/5000 (1923.79 req/sec)
response check PASSED
elapsed time: 2.6622 sec. performance check ACCEPTED
PASSED
```
