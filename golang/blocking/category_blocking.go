package blocking

import (
	"auction-worker/model"

	"golang.org/x/exp/slices"
)

// 広告とリクエストにあるカテゴリをもとに、配信許可/拒否制御を行う
func CategoryBlocking(advertiser model.Advertiser, category_blocking model.CategoryBlocking, request_category []uint) bool {
	include_ids := false
	for _, category_id := range category_blocking.CategoryIds {
		include_ids = include_ids || slices.Contains(request_category, category_id)
	}
	switch advertiser.CategoryBlockingTypeId {
	case model.ALLOW:
		return !include_ids
	case model.DENY:
		return include_ids
	}
	return false
}
