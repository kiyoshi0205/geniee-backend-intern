#pragma once

#include <event2/event.h>
#include <event2/http.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

#include "../config/config.cpp"

namespace geniee::server {
    /// @brief Httpリクエストを処理するスレッド。内部で登録したエンドポイントを呼び出している
    class HttpThread {
    public:
        HttpThread() : ev_base(event_base_new(), &event_base_free), http(evhttp_new(ev_base.get()), &evhttp_free), sock(create_socket()) {
            if (evhttp_accept_socket(http.get(), sock)) {
                fprintf(stderr, "failed to accept socket\n");
                exit(1);
            }
        }
        /// @brief エンドポイントを登録する
        /// @param endpoint エンドポイント名。
        /// @param cb 呼び出す関数。libeventのリクエストと任意の引数を1つとる関数を指定
        /// @param arg エンドポイントに渡すデータ。void*にして渡す
        void setup_cb(const std::string &endpoint, void (*cb)(evhttp_request *, void *), void *arg) {
            evhttp_set_cb(http.get(), endpoint.c_str(), cb, arg);
        }
        /// @brief スレッドを実行しエンドポイントにくるリクエストを待つ。
        void run() {
            worker = std::jthread([this]() { event_base_dispatch(ev_base.get()); });
        }
        ~HttpThread() {
            close(sock);
        }

    private:
        /// @brief リクエストを待つソケットを作成する。同一ポートをリッスンできるような設定を行う
        /// @return 作成したソケット
        int create_socket() {
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if (fd < 0) {
                fprintf(stderr, "failed to socket\n");
                exit(1);
            }
            evutil_make_socket_nonblocking(fd);
            evutil_make_listen_socket_reuseable_port(fd);
            sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(geniee::config::port);
            if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr))) {
                fprintf(stderr, "failed to bind ip to socket\n");
                exit(1);
            }
            if (listen(fd, geniee::config::backlog)) {
                fprintf(stderr, "failed to listen port\n");
                exit(1);
            }
            return fd;
        }

        std::jthread worker;
        std::unique_ptr<event_base, decltype(&event_base_free)> ev_base;
        std::unique_ptr<evhttp, decltype(&evhttp_free)> http;
        int sock;
    };

    /// @brief Httpスレッドをまとめて複数リクエスト処理を受けられるようにしたサーバ。
    class HttpServer {
    public:
        HttpServer(const int n_thread) : threads(n_thread) {}
        /// @brief エンドポイントを登録する
        /// @param endpoint エンドポイント名。
        /// @param cb 呼び出す関数。libeventのリクエストと任意の引数を1つとる関数を指定
        /// @param arg エンドポイントに渡すデータ。void*にして渡す
        void setup_cb(const std::string &endpoint, void (*cb)(evhttp_request *, void *), void *arg) {
            for (auto &&thread : threads) {
                thread.setup_cb(endpoint, cb, arg);
            }
        }
        /// @brief サーバを実行し、リクエストを待つ
        void run() {
            for (auto &&thread : threads) {
                thread.run();
            }
            printf("listening...\n");
        }

    private:
        std::vector<geniee::server::HttpThread> threads;
    };
}  // namespace geniee::server
