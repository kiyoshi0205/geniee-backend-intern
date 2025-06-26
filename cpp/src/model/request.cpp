#pragma once

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <ostream>
#include <string>
#include <vector>

namespace geniee::model {
    /// @brief サーバに来たオークションリクエストのうち、必要なものを取り出して保存したクラス。
    class Request {
    public:
        Request(const std::string m_auction_id, const std::vector<unsigned int> m_category_ids, const int m_feature, const unsigned int m_zone_width,
                const unsigned int m_zone_height, const std::vector<std::string> m_block_domains) :
            auction_id(m_auction_id),
            category_ids(m_category_ids),
            feature(m_feature),
            zone_width(m_zone_width),
            zone_height(m_zone_height),
            block_domains(m_block_domains) {}

        std::string getAuctionID() const {
            return auction_id;
        }

        std::vector<unsigned int> getCategoryIDs() const {
            return category_ids;
        }

        int getFeature() const {
            return feature;
        }

        unsigned int getZoneWidth() const {
            return zone_width;
        }

        unsigned int getZoneHeight() const {
            return zone_height;
        }

        std::vector<std::string> getBlockDomains() const {
            return block_domains;
        }

        friend std::ostream& operator<<(std::ostream& os, const Request& request) {
            return os << fmt::format(
                       "Request (auction_id = {}, category_ids = {}, feature = {}, zone_width = {}, zone_height = {}, block_domains = {})",
                       request.getAuctionID(), request.getCategoryIDs(), request.getFeature(), request.getZoneWidth(), request.getZoneHeight(),
                       request.getBlockDomains());
        }

    private:
        std::string auction_id;                  // オークションID。オークションを識別するために使用。
        std::vector<unsigned int> category_ids;  // 配信枠のカテゴリ属性。広告のカテゴリとマッチングさせる
        int feature;                             // 配信枠の特徴量。各広告主ごとの最もこの値に近い広告1つのみが配信候補となる
        unsigned int zone_width;                 // 要求広告サイズ幅
        unsigned int zone_height;                // 要求広告サイズ高さ
        std::vector<std::string> block_domains;  // 配信しないドメインのリスト
    };
}  // namespace geniee::model

/// @brief Requestのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::Request> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::Request request, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(),
                              "Request (auction_id = {}, category_ids = {}, feature = {}, zone_width = {}, zone_height = {}, block_domains = {})",
                              request.getAuctionID(), request.getCategoryIDs(), request.getFeature(), request.getZoneWidth(), request.getZoneHeight(),
                              request.getBlockDomains());
    }
};
