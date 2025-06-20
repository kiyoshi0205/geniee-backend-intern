from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class CategoryBlocking:
    """配信制御情報が格納されているクラス"""

    advertiser_id: int  # 広告主ID
    category_ids: List[int]  # カテゴリIDの配列。配信タイプの許可/拒否情報は対応する広告主に格納
