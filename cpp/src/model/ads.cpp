#pragma once

#include <fmt/format.h>

#include <ostream>
#include <string>

namespace geniee::model {
    /// @brief 広告案件クラス。広告の価格や広告本体のテキストが格納されている。
    class Ads {
    public:
        Ads(const unsigned int m_ads_id, const unsigned int m_advertiser_id, const double m_price, const std::string m_ads_txt,
            const unsigned int m_size_id) :
            ads_id(m_ads_id),
            advertiser_id(m_advertiser_id),
            price(m_price),
            ads_txt(m_ads_txt),
            size_id(m_size_id) {}
        Ads() : Ads(0, 0, 0.0, "", 0) {}

        unsigned int getAdsID() const {
            return ads_id;
        }

        unsigned int getAdvertiserID() const {
            return advertiser_id;
        }

        double getPrice() const {
            return price;
        }

        std::string getAdsTxt() const {
            return ads_txt;
        }

        unsigned int getSizeID() const {
            return size_id;
        }

        friend std::ostream& operator<<(std::ostream& os, const Ads& ads) {
            return os << fmt::format("Ads (ads_id = {}, advertiser_id = {}, price = {}, ads_txt = {}, size_id = {})", ads.getAdsID(),
                                     ads.getAdvertiserID(), ads.getPrice(), ads.getAdsTxt(), ads.getSizeID());
        }

    private:
        unsigned int ads_id;         // 広告案件ID
        unsigned int advertiser_id;  // 広告主ID
        double price;                // 広告の価格
        std::string ads_txt;         // 広告本体 (文字列)
        unsigned int size_id;        // 広告のサイズを示すid
    };
}  // namespace geniee::model

/// @brief Adsのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::Ads> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::Ads ads, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "Ads (ads_id = {}, advertiser_id = {}, price = {}, ads_txt = {}, size_id = {})", ads.getAdsID(),
                              ads.getAdvertiserID(), ads.getPrice(), ads.getAdsTxt(), ads.getSizeID());
    }
};
