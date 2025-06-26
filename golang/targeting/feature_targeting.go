package targeting

import (
	"auction-worker/model"
	"math"
)

// 広告と配信枠の特徴量を用いて、広告のターゲティングを行う
func FeatureTargeting(ads_features []model.AdsFeature, request_feature int) model.AdsFeature {
	nearest_id := 0
	best := math.MaxFloat64
	for id, ads_feature := range ads_features {
		if sub := math.Abs(float64(ads_feature.Feature - request_feature)); sub < best {
			best = sub
			nearest_id = id
		}
	}
	return ads_features[nearest_id]
}
