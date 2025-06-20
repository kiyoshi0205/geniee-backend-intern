#pragma once

#include <fmt/format.h>

#include <ostream>

namespace geniee::model {
    /// @brief 広告案件に紐づく特徴量を保存するクラス。
    class AdsFeature {
    public:
        AdsFeature(const unsigned int m_ads_id, const int m_feature) : ads_id(m_ads_id), feature(m_feature) {}
        AdsFeature() : AdsFeature(0, 0.0) {}

        unsigned int getAdsID() const {
            return ads_id;
        }

        int getFeature() const {
            return feature;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdsFeature& ads_feature) {
            return os << fmt::format("AdsFeature (ads_id = {}, feature = {})", ads_feature.getAdsID(), ads_feature.getFeature());
        }

    private:
        unsigned int ads_id;  // 広告案件ID
        int feature;          // 広告に紐づく特徴量
    };
}  // namespace geniee::model

/// @brief AdsFeatureのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::AdsFeature> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::AdsFeature ads_feature, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "AdsFeature (ads_id = {}, feature = {})", ads_feature.getAdsID(), ads_feature.getFeature());
    }
};
