#pragma once

#include <fmt/format.h>

#include <ostream>

namespace geniee::model {
    /// @brief 広告サイズクラス。広告のサイズIDに基づいて大きさを管理する。
    class AdSize {
    public:
        AdSize(const unsigned int m_size_id, const unsigned int m_ads_width, const unsigned int m_ads_height) :
            size_id(m_size_id),
            ads_width(m_ads_width),
            ads_height(m_ads_height) {}
        AdSize() : AdSize(0, 0, 0) {}

        unsigned int getSizeID() const {
            return size_id;
        }

        unsigned int getWidth() const {
            return ads_width;
        }

        unsigned int getHeight() const {
            return ads_height;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdSize& adsize) {
            return os << fmt::format("AdSize (size_id = {}, ads_width = {}, ads_height = {})", adsize.getSizeID(), adsize.getWidth(),
                                     adsize.getHeight());
        }

    private:
        unsigned int size_id;     // サイズID
        unsigned int ads_width;   // 広告幅
        unsigned int ads_height;  // 広告高さ
    };
}  // namespace geniee::model

/// @brief AdSizeのフォーマットを定義する。(fmt::format用)
template<>
struct fmt::formatter<geniee::model::AdSize> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(geniee::model::AdSize adsize, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "AdSize (size_id = {}, ads_width = {}, ads_height = {})", adsize.getSizeID(), adsize.getWidth(),
                              adsize.getHeight());
    }
};
