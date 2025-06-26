from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class Request:
    """サーバに来たオークションリクエストのうち、必要なものを取り出して保存したクラス"""

    auction_id: str  # オークションID。オークションを識別するために使用
    category_ids: List[int]  # 配信枠のカテゴリ属性。広告のカテゴリとマッチングさせる
    feature: int  # 配信枠の特徴量。各広告主ごとの最もこの値に近い広告1つのみが配信候補となる
    zone_width: int  # 配信枠の幅
    zone_height: int  # 配信枠の高さ
    block_domains: List[str]  # 配信しないドメインのリスト
