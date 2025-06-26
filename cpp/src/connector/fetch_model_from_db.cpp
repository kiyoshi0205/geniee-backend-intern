#pragma once

#include <optional>
#include <vector>

#include "../model/ads.cpp"
#include "../model/ads_feature.cpp"
#include "../model/advertiser.cpp"
#include "../model/category_blocking.cpp"
#include "./mysql_connector.cpp"
#include "../model/ads_size.cpp"

namespace geniee::connector::mysql {
    /// @brief 広告案件が格納されている `Adses` 全件をMySQLサーバから取得する関数。
    /// @param mysql_connector MySQLに接続しクエリ実行を行うクラス。
    /// @return geniee::model::Adsの配列。
    std::vector<geniee::model::Ads> fetchAdses(geniee::connector::mysql::MySQLConnector *mysql_connector) {
        auto res = mysql_connector->fetchModels("SELECT ads_id, advertiser_id, price, ads_txt, size_id FROM adses ORDER BY ads_id;");
        std::vector<geniee::model::Ads> adses;
        while (res->next()) {
            unsigned int ads_id = res->getUInt("ads_id");
            unsigned int advertiser_id = res->getUInt("advertiser_id");
            double price = res->getDouble("price");
            std::string ads_txt = res->getString("ads_txt");
            unsigned int size_id = res->getUInt("size_id");
            adses.emplace_back(ads_id, advertiser_id, price, ads_txt, size_id);
        }
        return adses;
    }
    /// @brief 広告サイズIDから紐づいている広告サイズが格納されている `ads_sizes` をMySQLサーバから取得する関数。
    /// @param mysql_connector MySQLに接続しクエリ実行を行うクラス。
    /// @return (幅,高さ)のペア `std::nullopt`。基本は存在するが念の為バリデーション。
    std::optional<geniee::model::AdSize> fetchAdSize(geniee::connector::mysql::MySQLConnector *mysql_connector, unsigned int size_id) {
        auto res = mysql_connector->fetchModelByID("SELECT ads_width, ads_height FROM ads_sizes WHERE size_id = ? LIMIT 1;", size_id);
        std::optional<geniee::model::AdSize> ad_size;
        while (res->next()) {
            unsigned int ads_width = res->getUInt("ads_width");
            unsigned int ads_height = res->getUInt("ads_height");
            ad_size.emplace(size_id, ads_width, ads_height);
            break;
        }
        return ad_size;
    }

    /// @brief 特定の広告主のカテゴリ情報が記録されている `CategoryBlocking` をMySQLサーバから取得する関数。
    /// @param mysql_connector  MySQLに接続しクエリ実行を行うクラス。
    /// @param advertiser_id カテゴリ情報を取得したい広告主ID
    /// @return 該当広告主の `geniee::model::CategoryBlocking` が存在すればそれを、存在しない場合は
    /// `std::nullopt`。基本は存在するが念の為バリデーション。
    std::optional<geniee::model::CategoryBlocking> fetchCategoryBlocking(geniee::connector::mysql::MySQLConnector *mysql_connector,
                                                                         unsigned int advertiser_id) {
        auto res = mysql_connector->fetchModelByID("SELECT category_id FROM category_blockings WHERE advertiser_id = ?;", advertiser_id);
        std::vector<unsigned int> category_ids;
        std::optional<geniee::model::CategoryBlocking> category_blocking;
        while (res->next()) {
            unsigned int category_id = res->getUInt("category_id");
            category_ids.emplace_back(category_id);
        }
        if (!category_ids.empty()) {
            category_blocking.emplace(advertiser_id, category_ids);
        }
        return category_blocking;
    }

    /// @brief 特定広告主 `Advertiser` をMySQLサーバから取得する関数。
    /// @param mysql_connector MySQLに接続しクエリ実行を行うクラス。
    /// @param advertiser_id 取得したい広告主ID
    /// @return 該当広告主の `geniee::model::Advertiser` が存在すればそれを、存在しない場合は `std::nullopt`。基本は存在するが念の為バリデーション。
    std::optional<geniee::model::Advertiser> fetchAdvertiser(geniee::connector::mysql::MySQLConnector *mysql_connector, unsigned int advertiser_id) {
        auto res = mysql_connector->fetchModelByID(
            "SELECT advertiser_name, category_blocking_type, domain FROM advertisers WHERE advertiser_id = ? LIMIT 1;", advertiser_id);
        std::optional<geniee::model::Advertiser> advertiser;
        while (res->next()) {
            std::string advertiser_name = res->getString("advertiser_name");
            geniee::model::Advertiser::CategoryBlockingType category_blocking_type =
                static_cast<geniee::model::Advertiser::CategoryBlockingType>(res->getInt("category_blocking_type"));
            std::string advertiser_domain = res->getString("domain");
            advertiser.emplace(advertiser_id, advertiser_name, category_blocking_type, advertiser_domain);
            break;
        }
        return advertiser;
    }

    /// @brief 特定広告案件の `AdsFeature` をMySQLサーバから取得する関数。
    /// @param mysql_connector MySQLに接続しクエリ実行を行うクラス。
    /// @param ads_id 取得したい広告案件ID
    /// @return 該当広告主の `geniee::model::AdsFeature` が存在すればそれを、存在しない場合は `std::nullopt`。基本は存在するが念の為バリデーション。
    std::optional<geniee::model::AdsFeature> fetchAdsFeature(geniee::connector::mysql::MySQLConnector *mysql_connector, unsigned int ads_id) {
        auto res = mysql_connector->fetchModelByID("SELECT feature FROM ads_features WHERE ads_id = ? LIMIT 1;", ads_id);
        std::optional<geniee::model::AdsFeature> ads_feature;
        while (res->next()) {
            int feature = res->getInt("feature");
            ads_feature.emplace(ads_id, feature);
            break;
        }
        return ads_feature;
    };
}  // namespace geniee::connector::mysql
