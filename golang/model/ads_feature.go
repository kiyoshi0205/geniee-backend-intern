package model

// 広告案件に紐づく特徴量を保存するクラス。
type AdsFeature struct {
	AdsId   uint `db:"ads_id"`  // 広告案件ID
	Feature int  `db:"feature"` // 広告に紐づく特徴量
}
