#pragma once

#include <event2/http.h>
#include <event2/http_struct.h>
#include <algorithm>
#include <map>
#include <optional>
#include <vector>

#include "../blocking/category_blocking.cpp"
#include "../connector/fetch_model_from_db.cpp"
#include "../connector/mysql_connector.cpp"
#include "../model/ads.cpp"
#include "../model/ads_feature.cpp"
#include "../model/advertiser.cpp"
#include "../model/category_blocking.cpp"
#include "../model/request.cpp"
#include "../model/response.cpp"
#include "../targeting/feature_targeting.cpp"
#include "./request_parser.cpp"
#include "./response_sender.cpp"
#include "../model/ads_size.cpp"

namespace geniee::server {
    /// @brief 処理の中心。リクエストや広告情報を元にオークションを行い、適切な広告を選択する。
    /// @param request 必要な情報が格納された自作クラスの `geniee::model::Request`。
    /// @param mysql_connector MySQLのコネクタ(スレッド間で共有するために引数で渡している)。
    std::optional<geniee::model::Response> auction(geniee::model::Request request, geniee::connector::mysql::MySQLConnector *mysql_connector) {
        // 広告主 -> 広告の対応を保存するための変数
        std::map<unsigned int, std::vector<geniee::model::Ads>> advertiser_adses;

        {
            // hint:ループの中でデータベースからデータを取得する処理が行われている(N+1問題)
            // 全てのデータを一括で取得するとSQLとの接続が少なくて高速化できます
            const std::vector<geniee::model::Ads> adses = geniee::connector::mysql::fetchAdses(mysql_connector);

            std::vector<geniee::model::Ads> adses_satisfy_size;
            // まずは広告のサイズを確認し、リクエストで要求されているサイズと一致するものを保存する
            for (auto ads : adses) {
                std::optional<geniee::model::AdSize> ad_size = geniee::connector::mysql::fetchAdSize(mysql_connector, ads.getSizeID());
                if (!ad_size)
                    continue;
                unsigned int ad_width = ad_size.value().getWidth();
                unsigned int ad_height = ad_size.value().getHeight();
                if (ad_width == request.getZoneWidth() && ad_height == request.getZoneHeight()) {
                    adses_satisfy_size.push_back(ads);
                }
            }
            std::vector<geniee::model::Ads> ads_satisfy_domain;
            // 次に広告主ドメインを確認し、リクエストで指定されたドメイン名が含まれていればブロックする
            for (auto ads : adses_satisfy_size) {
                std::optional<geniee::model::Advertiser> advertiser =
                    geniee::connector::mysql::fetchAdvertiser(mysql_connector, ads.getAdvertiserID());
                if (!advertiser.has_value())
                    continue;
                const std::string advertiser_domain = advertiser.value().getDomain();
                bool is_contain_block_domain = false;
                for (std::string target_domain : request.getBlockDomains()) {
                    if (advertiser_domain.find(target_domain) != std::string::npos) {
                        is_contain_block_domain = true;
                    }
                }
                if (!is_contain_block_domain) {
                    ads_satisfy_domain.push_back(ads);
                }
            }

            // 次にカテゴリ制御を行い、有効な広告主の広告のみ取り出して保存する
            for (auto ads : ads_satisfy_domain) {
                std::optional<geniee::model::Advertiser> advertiser =
                    geniee::connector::mysql::fetchAdvertiser(mysql_connector, ads.getAdvertiserID());
                std::optional<geniee::model::CategoryBlocking> category_blocking =
                    geniee::connector::mysql::fetchCategoryBlocking(mysql_connector, ads.getAdvertiserID());
                if (advertiser && category_blocking) {
                    bool blocked = geniee::blocking::categoryBlocking(advertiser.value(), category_blocking.value(), request.getCategoryIDs());
                    if (blocked) {
                        continue;
                    }
                }
                advertiser_adses[ads.getAdvertiserID()].push_back(ads);
            }
        }

        // 特徴量でターゲティングした後の結果を保存
        std::vector<geniee::model::Ads> ads_targetted;
        // 特徴量でターゲティングを行い、広告主ごとに最適な広告を選択
        for (auto [_, adv_adses] : advertiser_adses) {
            std::vector<geniee::model::AdsFeature> ads_features;
            for (auto ads : adv_adses) {
                std::optional<geniee::model::AdsFeature> ads_feature = geniee::connector::mysql::fetchAdsFeature(mysql_connector, ads.getAdsID());
                if (ads_feature) {
                    ads_features.push_back(ads_feature.value());
                }
            }
            if (!ads_features.empty()) {
                geniee::model::AdsFeature best_ads = geniee::targeting::featureTargeting(ads_features, request.getFeature());

                const auto best_itr = std::ranges::find(adv_adses, best_ads.getAdsID(), &geniee::model::Ads::getAdsID);
                assert(best_itr != std::end(adv_adses));

                ads_targetted.push_back(*best_itr);
            }
        }

        // 最も価格が高い広告を取得するために逆順ソート
        std::ranges::stable_sort(ads_targetted, std::greater<>(), &geniee::model::Ads::getPrice);

        if (ads_targetted.empty()) {
            return std::nullopt;
        } else {
            geniee::model::Ads winner = ads_targetted.front();  // 先頭の広告が最も高価。
            geniee::model::Response response(request.getAuctionID(), winner.getAdsTxt(), winner.getPrice());
            return response;
        }
    }

    /// @brief サーバから叩かれるエンドポイント。処理しやすいように加工を行いメイン処理を呼び出す。
    /// @param http_request libeventのリクエストクラス。
    /// @param arg エンドポイント設定時にあらかじめ指定したオブジェクトを渡すことができる。今回はMySQLのコネクタをスレッド間で共有するために使用。
    void auction_handler(evhttp_request *http_request, void *arg) {
        const geniee::model::Request request = geniee::server::parseRequest(http_request);
        const std::optional<geniee::model::Response> response = auction(request, static_cast<geniee::connector::mysql::MySQLConnector *>(arg));
        if (response) {
            geniee::server::sendWinnerResponse(http_request, response.value());
        } else {
            geniee::server::sendNobidResponse(http_request);
        }
    }

}  // namespace geniee::server
