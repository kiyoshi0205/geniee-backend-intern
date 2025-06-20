package model

// サーバに来たオークションリクエストのうち、必要なものを取り出して保存したクラス。
type Request struct {
	AuctionID    string   `json:"auction_id"`    // オークションID。オークションを識別するために使用。
	CategoryIds  []uint   `json:"category_ids"`  // 配信枠のカテゴリ属性。広告のカテゴリとマッチングさせる
	Feature      int      `json:"zone_feature"`  // 配信枠の特徴量。各広告主ごとの最もこの値に近い広告1つのみが配信候補となる
	ZoneWidth    uint     `json:"zone_width"`    // 配信枠の幅
	ZoneHeight   uint     `json:"zone_height"`   // 配信枠の高さ
	BlockDomains []string `json:"block_domains"` // 配信しないドメインのリスト
}
