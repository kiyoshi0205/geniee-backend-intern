#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include "./config/config.cpp"
#include "./connector/mysql_connector.cpp"
#include "./server/auction.cpp"
#include "./server/server.cpp"

int main() {
    pid_t pid = getpid();
    std::cout << "pid = " << pid << std::endl;
    geniee::connector::mysql::MySQLConnector mysql_connector;
    geniee::server::HttpServer server(geniee::config::n_thread);
    server.setup_cb("/auction", geniee::server::auction_handler, &mysql_connector);
    server.run();
}
