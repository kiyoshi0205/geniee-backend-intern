#pragma once

#include <string>

namespace geniee::config {
    // db setting
    const std::string db_uri = "tcp://127.0.0.1:3306/geniee";
    const std::string db_user = "root";
    const std::string db_pass = "r4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ";

    // port
    const int port = 6562;

    // thread数
    const int n_thread = 3;

    // リクエストが許容できる長さ
    const int backlog = 1000;

}  // namespace geniee::config
