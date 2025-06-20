from dataclasses import dataclass


@dataclass(frozen=True)
class Response:
    """オークション結果を保存するクラス"""

    auction_id: str  # 対応するオークションID
    ads: str  # 広告本体の文字列
    price: float  # 広告の価格
