package server

import (
	"auction-worker/model"
	"encoding/json"
	"net/http"
)

// リクエスト元にオークション情報をJSON形式にして返す
func sendWinnerResponse(w http.ResponseWriter, response model.Response) {
	raw, err := json.Marshal(response)
	if err != nil {
		sendNobidResponse(w)
	} else {
		w.WriteHeader(http.StatusOK)
		w.Write(raw)
	}
}

// 該当する広告がない場合に `204 No Contents` を返す
func sendNobidResponse(w http.ResponseWriter) {
	w.WriteHeader(http.StatusNoContent)
}
