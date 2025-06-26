# レスポンス (json)

```json
{
  "auction_id": "xxxxxxxxxxx",
  "ads": "yyyyyy",
  "price": 2.7182
}
```

| json キー  | 説明                                                                        |
| ---------- | --------------------------------------------------------------------------- |
| auction_id | request.json に含まれている auction_id を返します。                           |
| ads        | 広告案件です。DB に保存されている値を返します。検証のために文字列が設定されています。 |
| price      | この広告の価格です。同じく DB に保存されています。                                |

要求に合致する広告が存在しない場合は以下のようなメッセージが返ってきます。

```json
{
  "auction_id": "xxxxxxxxxxx",
  "message": "No Contents"
}
```
