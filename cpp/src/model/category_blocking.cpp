#pragma once

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <ostream>
#include <vector>

namespace geniee::model {
    /// @brief 配信制御情報が格納されているクラス。
    class CategoryBlocking {
    public:
        CategoryBlocking(const unsigned int m_advertiser_id, const std::vector<unsigned int> m_category_ids) :
            advertiser_id(m_advertiser_id),
            category_ids(m_category_ids) {}
        CategoryBlocking() : CategoryBlocking(0, {}) {}

        unsigned int getAdvertiserID() const {
            return advertiser_id;
        }

        std::vector<unsigned int> getCategoryIDs() const {
            return category_ids;
        }

        friend std::ostream& operator<<(std::ostream& os, const CategoryBlocking& category_blocking) {
            return os << fmt::format("CategoryBlocking (advertiser_id = {}, category_ids = {})", category_blocking.getAdvertiserID(),
                                     category_blocking.getCategoryIDs());
        }

    private:
        unsigned int advertiser_id;              // 広告主ID
        std::vector<unsigned int> category_ids;  // カテゴリIDの配列。配信タイプの許可/拒否情報は対応する広告主に格納
    };
}  // namespace geniee::model

/// @brief CategoryBlockingのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::CategoryBlocking> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::CategoryBlocking category_blocking, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "CategoryBlocking (advertiser_id = {}, category_ids = {})", category_blocking.getAdvertiserID(),
                              category_blocking.getCategoryIDs());
    }
};
