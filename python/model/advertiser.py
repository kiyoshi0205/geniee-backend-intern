from enum import Enum
from dataclasses import dataclass


class CategoryBlockingType(Enum):
    ALLOW = 0
    DENY = 1


@dataclass(frozen=True)
class Advertiser:
    """広告主情報が格納されているクラス"""

    advertiser_id: int  # 広告主ID
    advertiser_name: str  # 広告主名
    category_blocking_type_id: CategoryBlockingType  # カテゴリ配信制御の許可(0)/拒否(1)フラグ
    domain: str  # 広告主のドメイン
