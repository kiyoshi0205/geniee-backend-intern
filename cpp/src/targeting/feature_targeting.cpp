#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "../model/ads_feature.cpp"

namespace geniee::targeting {
    /// @brief 広告と配信枠の特徴量を用いて、広告のターゲティングを行う
    /// @param ads_features ある広告主が所持している広告特徴量配列
    /// @param request_feature 配信枠に付与されている特徴量
    /// @return 配信枠の特徴量に最も近い広告特徴量
    geniee::model::AdsFeature featureTargeting(const std::vector<geniee::model::AdsFeature> ads_features, const int request_feature) {
        return std::ranges::min(ads_features, std::less<>(), [request_feature](geniee::model::AdsFeature ads_feature) {
            return std::abs(request_feature - ads_feature.getFeature());
        });
    }
}  // namespace geniee::targeting
