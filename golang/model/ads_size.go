package model

// 広告案件に紐づく特徴量を保存するクラス。
type AdSize struct {
	SizeId    uint `db:"size_id"`    // 広告サイズID
	AdsWidth  uint `db:"ads_width"`  // 広告幅
	AdsHeight uint `db:"ads_height"` // 広告高さ
}
