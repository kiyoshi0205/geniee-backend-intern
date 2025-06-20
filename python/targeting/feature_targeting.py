from model.ads_feature import AdsFeature
from typing import List

import sys


def featureTargeting(ads_features: List[AdsFeature], request_feature: int) -> AdsFeature:
    """広告と配信枠の特徴量を用いて、広告のターゲティングを行う

    Args:
        ads_feature (List[AdsFeature]): ある広告主が所持している広告特徴量配列
        request_feture (int): 配信枠に付与されている特徴量

    Returns:
        AdsFeature: 配信枠の特徴量に最も近い広告特徴量
    """
    nearest_id = 0
    best = sys.maxsize

    for idx, ads_feature in enumerate(ads_features):
        if (sub := abs(request_feature - ads_feature.feature)) < best:
            best = sub
            nearest_id = idx

    return ads_features[nearest_id]
