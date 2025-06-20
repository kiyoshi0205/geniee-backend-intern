#pragma once

#include <event2/buffer.h>
#include <event2/http.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <memory>

#include "../model/response.cpp"

namespace geniee::server {
    /// @brief リクエスト元にオークション情報をJSON形式にして返す
    /// @param r libeventのリクエスト情報
    /// @param response オークションに勝った広告情報などが保存されているモデル
    void sendWinnerResponse(evhttp_request *r, const geniee::model::Response response) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<decltype(buffer)> writer(buffer);
        writer.StartObject();
        writer.Key("auction_id");
        writer.String(response.getAuctionID());

        writer.Key("ads");
        writer.String(response.getAds());

        writer.Key("price");
        writer.Double(response.getPrice());

        writer.EndObject();

        std::unique_ptr<evbuffer, decltype(&evbuffer_free)> buf(evbuffer_new(), &evbuffer_free);
        if (!buf) {
            evhttp_send_error(r, HTTP_SERVUNAVAIL, "Internal Server Error");
            return;
        }

        evbuffer_add(buf.get(), buffer.GetString(), buffer.GetSize());
        evhttp_send_reply(r, HTTP_OK, "OK", buf.get());
    }

    /// @brief 該当する広告がない場合に 204 No contents を返す
    /// @param r libeventのリクエスト情報
    void sendNobidResponse(evhttp_request *r) {
        evhttp_send_reply(r, HTTP_NOCONTENT, nullptr, nullptr);
    }

}  // namespace geniee::server
