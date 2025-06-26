package server

import (
	"auction-worker/blocking"
	"auction-worker/connector"
	"auction-worker/model"
	"auction-worker/targeting"
	"fmt"
	"net/http"
	"sort"
	"strings"
)

// 処理の中心。リクエストや広告情報を元にオークションを行い、適切な広告を選択する。
func auction(request model.Request, arg any) (model.Response, error) {
	// anyのままだと扱えないのでキャスト
	mysql_connector, ok := arg.(*connector.MySQLConnector)
	if !ok {
		panic("convert argument to mysql connector failed")
	}

	// 広告主 -> 広告の対応を保存するための変数
	advertiser_adses := make(map[uint][]model.Ads)

	// hint:ループの中でデータベースからデータを取得する処理が行われている(N+1問題)
	// 全てのデータを一括で取得するとSQLとの接続が少なくて高速化できます
	{
		adses, err := connector.FetchAdses(mysql_connector)
		if err != nil {
			return model.Response{}, err
		}
		var advertiser_satisfy_size []model.Ads

		// まずは広告サイズを確認し、リクエストで要求されているサイズと一致しているものを保存する
		for _, ads := range adses {
			ad_size, err := connector.FetchAdSize(mysql_connector, ads.SizeId)
			if err == nil {
				if ad_size.AdsWidth == request.ZoneWidth && ad_size.AdsHeight == request.ZoneHeight {
					advertiser_satisfy_size = append(advertiser_satisfy_size, ads)
				}
			}
		}

		var advertiser_satisfy_domain []model.Ads
		// 次に広告主ドメインを確認し、リクエストで指定されたドメイン名が含まれていればブロックする
		for _, ads := range advertiser_satisfy_size {

			advertiser, err := connector.FetchAdvertiser(mysql_connector, ads.AdvertiserId)
			if err == nil {
				contain_block_domain := false
				for _, block_domain := range request.BlockDomains {
					if strings.Contains(advertiser.AdvertiserDomain, block_domain) {
						contain_block_domain = true
					}
				}
				if !contain_block_domain {
					advertiser_satisfy_domain = append(advertiser_satisfy_domain, ads)
				}
			}
		}

		// 次にカテゴリ制御を行い、有効な広告主の広告のみ取り出して保存する
		for _, ads := range advertiser_satisfy_domain {
			advertiser, err1 := connector.FetchAdvertiser(mysql_connector, ads.AdvertiserId)
			category_blocking, err2 := connector.FetchCategoryBlocking(mysql_connector, ads.AdvertiserId)
			if err1 == nil && err2 == nil {
				blocking := blocking.CategoryBlocking(advertiser, category_blocking, request.CategoryIds)
				if blocking {
					continue
				}
			}
			advertiser_adses[ads.AdvertiserId] = append(advertiser_adses[ads.AdvertiserId], ads)
		}
	}

	// 特徴量でターゲティングした後の結果を保存
	var ads_targetted []model.Ads
	// 特徴量でターゲティングを行い、広告主ごとに最適な広告を選択
	for _, adv_adses := range advertiser_adses {
		var ads_features []model.AdsFeature
		for _, ads := range adv_adses {
			if ads_feature, err := connector.FetchAdsFeature(mysql_connector, ads.AdsId); err == nil {
				ads_features = append(ads_features, ads_feature)
			}
		}
		if len(ads_features) > 0 {
			best_ads := targeting.FeatureTargeting(ads_features, request.Feature)
			for _, ads := range adv_adses {
				if ads.AdsId == best_ads.AdsId {
					ads_targetted = append(ads_targetted, ads)
				}
			}
		}
	}

	// 最も価格が高い広告を取得するために逆順ソート
	sort.Slice(ads_targetted, func(i int, j int) bool {
		if ads_targetted[i].Price != ads_targetted[j].Price {
			return ads_targetted[i].Price > ads_targetted[j].Price
		}
		return ads_targetted[i].AdsId < ads_targetted[j].AdsId
	})

	if len(ads_targetted) == 0 {
		return model.Response{}, fmt.Errorf("No appropriate ads found")
	} else {
		// 先頭の広告が最も高価
		winner := ads_targetted[0]
		response := model.Response{
			AuctionID: request.AuctionID,
			Ads:       winner.AdsTxt,
			Price:     winner.Price,
		}
		return response, nil
	}
}

// サーバから叩かれるエンドポイント。処理しやすいように加工を行いメイン処理を呼び出す
func AuctionHandler(w http.ResponseWriter, r *http.Request, arg any) {
	// そのままだと使い勝手が悪いので自作モデルに変換
	request := parseRequest(r)
	// リクエストを処理する関数を呼ぶ
	response, err := auction(request, arg)
	if err != nil {
		fmt.Println(err)
		sendNobidResponse(w)
	} else {
		sendWinnerResponse(w, response)
	}
}
