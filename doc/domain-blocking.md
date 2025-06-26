# ドメインブロック制御

広告主はドメインを保有しています(`advertisers.domain` )。

リクエストに含まれる文字列配列 `block_domains`の文字列達のうち、少なくとも一つが連続部分文字列として広告主ドメインに含まれていた場合、その広告は配信候補から除外されます。

ここで、ある文字列Sの連続部分文字列とは、Sが空文字にならない限り任意の文字数を先頭と末尾から取り除いてできる文字列とします。


## 具体例

リクエスト中の`block_domains`に含まれる禁止文字列達が`prohibit,tet,hoge`の場合、

以下の広告主ドメインはそれぞれブロック対象となり、配信候補から除外されます。

`hogefuga.com` 、`googletest.com`、`dprohibitptest.com`


それぞれ理由は

- `hogefuga.com`の場合、1文字目から4文字目に`hoge`が含まれる。

- `googletest.com`の場合、7文字目から10文字目に`test`が含まれる。

- `dprohibitptest.com`の場合、2文字目から9文字目に`prohibit`が含まれ、11文字目から14文字目には`test`が含まれる。


一方で次の広告主ドメインの場合はリクエスト中の禁止文字列が含まれていないので配信候補になることができます。

`googletes.com`、`fhoguga.com`、`geniee.com`




