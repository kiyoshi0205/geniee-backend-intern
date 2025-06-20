package model

// オークション結果を保存するクラス。
type Response struct {
	AuctionID string  `json:"auction_id"` // 対応するオークションID
	Ads       string  `json:"ads"`        // 広告本体の文字列
	Price     float64 `json:"price"`      // 広告の価格
}
