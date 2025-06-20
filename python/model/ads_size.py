from dataclasses import dataclass


@dataclass(frozen=True)
class AdSize:
    """広告サイズクラス。広告のサイズIDに基づいて大きさを管理する"""

    size_id: int  # サイズID
    ads_width: int  # 広告幅
    ads_height: int  # 広告高さ
