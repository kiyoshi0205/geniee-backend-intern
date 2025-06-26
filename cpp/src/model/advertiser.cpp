#pragma once

#include <fmt/format.h>

#include <ostream>
#include <string>

namespace geniee::model {
    /// @brief 広告主情報が格納されているクラス。
    class Advertiser {
    public:
        enum class CategoryBlockingType { ALLOW, DENY };

        Advertiser(const unsigned int m_advertiser_id, const std::string m_advertiser_name, const CategoryBlockingType m_category_blocking_type,
                   const std::string m_domain) :
            advertiser_id(m_advertiser_id),
            advertiser_name(m_advertiser_name),
            category_blocking_type(m_category_blocking_type),
            advertiser_domain(m_domain) {}
        Advertiser() : Advertiser(0, "", Advertiser::CategoryBlockingType::ALLOW, "") {}

        std::string getAdvertiserName() const {
            return advertiser_name;
        }

        unsigned int getAdvertiserID() const {
            return advertiser_id;
        }

        CategoryBlockingType getCategoryBlockingType() const {
            return category_blocking_type;
        }

        std::string getDomain() const {
            return advertiser_domain;
        }

        friend std::ostream& operator<<(std::ostream& os, const Advertiser& advertiser) {
            return os << fmt::format("Advertiser (advertiser_id = {}, advertiser_name = {}, category_blocking_type = {}, domain = {})",
                                     advertiser.getAdvertiserID(), advertiser.getAdvertiserName(),
                                     static_cast<std::underlying_type_t<CategoryBlockingType>>(advertiser.getCategoryBlockingType()),
                                     advertiser.getDomain());
        }

    private:
        unsigned int advertiser_id;                   // 広告主ID
        std::string advertiser_name;                  // 広告主名
        CategoryBlockingType category_blocking_type;  // カテゴリ配信制御の許可(0)/拒否(1)フラグ。
        std::string advertiser_domain;                // 広告主のドメイン
    };
}  // namespace geniee::model

/// @brief Advertiserのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::Advertiser> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::Advertiser advertiser, fmt::format_context& ctx) const {
        return fmt::format_to(
            ctx.out(), "Advertiser (advertiser_id = {}, advertiser_name = {}, category_blocking_type = {})", advertiser.getAdvertiserID(),
            advertiser.getAdvertiserName(),
            static_cast<std::underlying_type_t<geniee::model::Advertiser::CategoryBlockingType>>(advertiser.getCategoryBlockingType()));
    }
};
