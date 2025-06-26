# スコア情報

## 初期実装

参考までに小規模データセットを Writer の PC 上で実行した時のスコアを残します。0.51 req/sec 程度。(C++)

```
--------------------small--------------------
init DB data...   done.
run auction-worker...
wait 5 second for setup auction worker...
[####################] 10/10 (0.51 req/sec)
response check PASSED
elapsed time: 19.4622 sec. performance check Time Limit Exceeded...
FAILED...
make: *** [Makefile:60: inner-check] Error 1
make: Leaving directory '/tmp/intern'
make: *** [check] Error 2
```

---

## Write の結果

チャレンジデータセットの結果。2721 req/sec 程度。(C++)

```
--------------------challenge--------------------
run auction-worker...
wait 5 second for setup auction worker...
[####################] 5000/5000 (2721.28 req/sec)
response check PASSED
elapsed time: 1.8755 sec. performance check ACCEPTED
PASSED
ALL TEST PASSED
```
