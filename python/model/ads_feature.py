from dataclasses import dataclass


@dataclass(frozen=True)
class AdsFeature:
    """広告案件に紐づく特徴量を保存するクラス"""

    ads_id: int  # 広告案件ID
    feature: int  # 広告に紐づく特徴量
