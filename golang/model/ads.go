package model

// 広告案件クラス。広告の価格や広告本体のテキストが格納されている。
type Ads struct {
	AdsId        uint    `db:"ads_id"`        // 広告案件ID
	AdvertiserId uint    `db:"advertiser_id"` // 広告主ID
	Price        float64 `db:"price"`         // 広告の価格
	AdsTxt       string  `db:"ads_txt"`       // 広告本体 (文字列)
	SizeId       uint    `db:"size_id"`       // 広告サイズID
}
