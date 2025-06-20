from model.advertiser import CategoryBlockingType, Advertiser
from model.category_blocking import CategoryBlocking
from typing import List


def categoryBlocking(advertiser: Advertiser, category_blocking: CategoryBlocking, request_category: List[int]) -> bool:
    """広告とリクエストにあるカテゴリをもとに、配信許可/拒否制御を行う

    Args:
        advertiser (Advertiser): 広告主クラス。配信許可/拒否の情報を所持
        category_blocking (CategoryBlocking): 広告主に紐付いているカテゴリ情報
        request_category (List[int]): リクエストに付与されているカテゴリ情報

    Returns:
        bool: カテゴリ制御によってブロックされるときにtrueを返す
    """
    including_ads = False
    for category_id in category_blocking.category_ids:
        including_ads = including_ads or category_id in request_category

    if advertiser.category_blocking_type_id == CategoryBlockingType.ALLOW:
        return not including_ads
    else:
        return including_ads
