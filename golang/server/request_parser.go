package server

import (
	"auction-worker/model"
	"encoding/json"
	"net/http"
)

// http.Requestを自作モデルのリクエストに変換
func parseRequest(r *http.Request) model.Request {
	request := model.Request{}
	json.NewDecoder(r.Body).Decode(&request)
	return request
}
