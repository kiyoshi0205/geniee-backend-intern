package connector

import (
	"auction-worker/model"
)

// 広告案件が格納されている `Adses` 全件をMySQLサーバから取得する関数。
func FetchAdses(mysql_connector *MySQLConnector) ([]model.Ads, error) {
	var adses []model.Ads
	err := mysql_connector.fetchModels("SELECT ads_id, advertiser_id, price, ads_txt, size_id FROM adses ORDER BY ads_id;", &adses)
	return adses, err
}

// 広告サイズが格納されている `AdSize` 広告サイズIDをもとに対応するデータをMySQLサーバから取得する関数。
func FetchAdSize(mysql_connector *MySQLConnector, size_id uint) (model.AdSize, error) {
	var res struct {
		AdsWidth  uint `db:"ads_width"`
		AdsHeight uint `db:"ads_height"`
	}
	err := mysql_connector.fetchModelByID("SELECT ads_width, ads_height FROM ads_sizes WHERE size_id = ? LIMIT 1;", size_id, &res)
	if err != nil {
		return model.AdSize{}, err
	}

	return model.AdSize{
		SizeId:    size_id,
		AdsWidth:  res.AdsWidth,
		AdsHeight: res.AdsHeight,
	}, nil
}

// 特定の広告主のカテゴリ情報が記録されている `CategoryBlocking` をMySQLサーバから取得する関数。
func FetchCategoryBlocking(mysql_connector *MySQLConnector, advertiser_id uint) (model.CategoryBlocking, error) {
	var category_ids []uint
	err := mysql_connector.fetchModelByID("SELECT category_id FROM category_blockings WHERE advertiser_id = ?;", advertiser_id, &category_ids)
	if err != nil {
		return model.CategoryBlocking{}, err
	}
	return model.CategoryBlocking{
		AdvertiserId: advertiser_id,
		CategoryIds:  category_ids,
	}, nil
}

// 特定広告主 `Advertiser` をMySQLサーバから取得する関数。
func FetchAdvertiser(mysql_connector *MySQLConnector, advertiser_id uint) (model.Advertiser, error) {
	var res struct {
		AdvertiserName         string                     `db:"advertiser_name"`
		CategoryBlockingTypeId model.CategoryBlockingType `db:"category_blocking_type"`
		AdvertiserDomain       string                     `db:"domain"`
	}
	err := mysql_connector.fetchModelByID("SELECT advertiser_name, category_blocking_type, domain FROM advertisers WHERE advertiser_id = ? LIMIT 1;", advertiser_id, &res)
	if err != nil {
		return model.Advertiser{}, err
	}
	return model.Advertiser{
		AdvertiserId:           advertiser_id,
		AdvertiserName:         res.AdvertiserName,
		CategoryBlockingTypeId: res.CategoryBlockingTypeId,
		AdvertiserDomain:       res.AdvertiserDomain,
	}, nil
}

// 特定広告案件の `AdsFeature` をMySQLサーバから取得する関数。
func FetchAdsFeature(mysql_connector *MySQLConnector, ads_id uint) (model.AdsFeature, error) {
	var ads_feature int
	err := mysql_connector.fetchModelByID("SELECT feature FROM ads_features WHERE ads_id = ? LIMIT 1;", ads_id, &ads_feature)
	if err != nil {
		return model.AdsFeature{}, err
	}
	return model.AdsFeature{
		AdsId:   ads_id,
		Feature: ads_feature,
	}, nil
}
