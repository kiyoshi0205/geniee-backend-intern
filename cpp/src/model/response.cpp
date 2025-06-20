#pragma once

#include <fmt/format.h>

#include <ostream>
#include <string>

namespace geniee::model {
    /// @brief オークション結果を保存するクラス。
    class Response {
    public:
        Response(const std::string m_auction_id, const std::string m_ads, const double m_price) :
            auction_id(m_auction_id),
            ads(m_ads),
            price(m_price) {}

        std::string getAuctionID() const {
            return auction_id;
        }

        std::string getAds() const {
            return ads;
        }

        double getPrice() const {
            return price;
        }

        friend std::ostream& operator<<(std::ostream& os, const Response& response) {
            return os << fmt::format("Response (auction_id = {}, ads = {}, price = {})", response.getAuctionID(), response.getAds(),
                                     response.getPrice());
        }

    private:
        std::string auction_id;  // 対応するオークションID
        std::string ads;         // 広告本体の文字列
        double price;            // 広告の価格
    };
}  // namespace geniee::model

/// @brief Responseのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::Response> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::Response response, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "Response (auction_id = {}, ads = {}, price = {})", response.getAuctionID(), response.getAds(),
                              response.getPrice());
    }
};
