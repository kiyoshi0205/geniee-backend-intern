package server

import (
	"auction-worker/config"
	"fmt"
	"net/http"
)

// Httpスレッドをまとめて複数リクエスト処理を受けられるようにしたサーバ
type HttpServer struct {
	mux *http.ServeMux
}

// HttpServerのコンストラクタ
func NewHttpServer() HttpServer {
	mux := http.NewServeMux()
	return HttpServer{
		mux: mux,
	}
}

// エンドポイントを登録する。handlerにデータを渡したいときはarg経由でやり取りすることが可能
func (server *HttpServer) SetupCb(endpoint string, cb func(http.ResponseWriter, *http.Request, any), arg any) {
	threads := make(chan struct{}, config.N_Thread)
	server.mux.HandleFunc(endpoint, func(w http.ResponseWriter, r *http.Request) {
		// 同時スレッド数を制限するための処理
		threads <- struct{}{}
		cb(w, r, arg)
		<-threads
	})
}

// Httpサーバを起動する
func (server *HttpServer) Run() {
	srv := &http.Server{
		Addr:    fmt.Sprintf(":%d", config.Port),
		Handler: server.mux,
	}
	fmt.Println("listening...")
	srv.ListenAndServe()
}
