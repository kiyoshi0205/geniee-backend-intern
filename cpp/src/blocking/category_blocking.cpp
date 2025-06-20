#pragma once

#include <algorithm>
#include <vector>

#include "../model/advertiser.cpp"
#include "../model/category_blocking.cpp"

namespace geniee::blocking {
    /// @brief 広告とリクエストにあるカテゴリをもとに、配信許可/拒否制御を行う
    /// @param advertiser 広告主クラス。配信許可/拒否の情報を所持
    /// @param category_blocking　広告主に紐付いているカテゴリ情報
    /// @param request_category リクエストに付与されているカテゴリ情報
    /// @return カテゴリ制御によってブロックされるときにtrueを返す
    bool categoryBlocking(const geniee::model::Advertiser advertiser, const geniee::model::CategoryBlocking category_blocking,
                          const std::vector<unsigned int> request_category) {
        // request_categoryの中に対象のカテゴリが含まれているかどうか
        auto find_category = [request_category](unsigned int category_id) -> bool {
            return std::ranges::find(request_category, category_id) != std::end(request_category);
        };
        const bool including_ids = std::ranges::any_of(category_blocking.getCategoryIDs(), find_category);

        switch (advertiser.getCategoryBlockingType()) {
            case geniee::model::Advertiser::CategoryBlockingType::ALLOW:
                return !including_ids;
            case geniee::model::Advertiser::CategoryBlockingType::DENY:
                return including_ids;
        }
        std::terminate();
    }
}  // namespace geniee::blocking
