package model

type CategoryBlockingType int

const (
	ALLOW CategoryBlockingType = iota
	DENY
)

// 広告主情報が格納されているクラス。
type Advertiser struct {
	AdvertiserId           uint                 // 広告主ID
	AdvertiserName         string               // 広告主名
	CategoryBlockingTypeId CategoryBlockingType // カテゴリ配信制御の許可(0)/拒否(1)フラグ。
	AdvertiserDomain       string               // 広告主のドメイン名
}
