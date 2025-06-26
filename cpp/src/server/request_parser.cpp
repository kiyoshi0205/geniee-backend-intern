#pragma once

#include <event2/buffer.h>
#include <event2/http.h>
#include <rapidjson/document.h>

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "../model/request.cpp"

namespace geniee::server {
    /// @brief libeventのリクエストを自作モデルのリクエストに変換
    /// @param r libeventのリクエスト
    /// @return 情報が格納された自作の `geniee::model::Request`
    geniee::model::Request parseRequest(evhttp_request *r) {
        evbuffer *input_buffer = evhttp_request_get_input_buffer(r);
        const size_t len = evbuffer_get_length(input_buffer);
        std::string data(len, '\0');
        evbuffer_remove(input_buffer, data.data(), len);

        rapidjson::Document json;
        json.Parse(data);

        std::string auction_id;
        std::vector<unsigned int> category_ids;
        int feature = 0;
        unsigned int zone_width = 0;
        unsigned int zone_height = 0;
        std::vector<std::string> block_domains;

        if (json.HasMember("auction_id") && json["auction_id"].IsString()) {
            auction_id = json["auction_id"].GetString();
        }

        if (json.HasMember("category_ids") && json["category_ids"].IsArray()) {
            std::ranges::transform(json["category_ids"].GetArray() | std::views::filter([](const auto &v) { return v.IsUint(); }),
                                   std::back_inserter(category_ids), [](const auto &v) { return v.GetUint(); });
        }

        if (json.HasMember("zone_feature") && json["zone_feature"].IsInt()) {
            feature = json["zone_feature"].GetInt();
        }

        if (json.HasMember("zone_width") && json["zone_width"].IsUint()) {
            zone_width = json["zone_width"].GetUint();
        }
        if (json.HasMember("zone_height") && json["zone_height"].IsUint()) {
            zone_height = json["zone_height"].GetUint();
        }

        if (json.HasMember("block_domains") && json["block_domains"].IsArray()) {
            std::ranges::transform(json["block_domains"].GetArray() | std::views::filter([](const auto &v) { return v.IsString(); }),
                                   std::back_inserter(block_domains), [](const auto &v) { return v.GetString(); });
        }

        return geniee::model::Request(auction_id, category_ids, feature, zone_width, zone_height, block_domains);
    }
}  // namespace geniee::server
