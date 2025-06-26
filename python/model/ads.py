from dataclasses import dataclass


@dataclass(frozen=True)
class Ads:
    """広告案件クラス。広告の価格や広告本体のテキストが格納されている"""

    ads_id: int  # 広告案件ID
    advertiser_id: int  # 広告主ID
    price: float  # 広告の価格
    ads_txt: str  # 広告本体 (文字列)
    size_id: int  # 広告サイズID
