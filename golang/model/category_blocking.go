package model

// 配信制御情報が格納されているクラス。
type CategoryBlocking struct {
	AdvertiserId uint   // 広告主ID
	CategoryIds  []uint // カテゴリIDの配列。配信タイプの許可/拒否情報は対応する広告主に格納
}
