#pragma once

#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>

#include <memory>
#include <mutex>
#include <string>

#include "../config/config.cpp"

namespace geniee::connector::mysql {
    /// @brief
    /// MySQLに接続しデータを取得する関数群が定義されているクラス。使用ライブラリの都合上MySQLの接続はスレッドセーフではないため、こちらのクラスで排他制御。
    class MySQLConnector {
    public:
        /// @brief SQLクエリを実行しデータ全てを返す関数
        /// @param query SQLクエリ。データ取得クエリ (Selectなど) を想定
        /// @return データアクセスのためのクラス。基本的には `next()`　でCursorを動かし `getXXX()` でデータを取得する。詳しくは
        /// `fetch_model_from_db.cpp` か公式リファレンスを参照のこと。sql::ResultSetはdeleteする必要があるがそちらはunique_ptrが面倒を見る。
        std::unique_ptr<sql::ResultSet> fetchModels(std::string query) {
            std::lock_guard<std::mutex> lock(mut);
            sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();  // この関数はthread safe *ではないので* 排他制御
            std::unique_ptr<sql::Connection> con(driver->connect(geniee::config::db_uri, geniee::config::db_user, geniee::config::db_pass));
            std::unique_ptr<sql::Statement> stmt(con->createStatement());
            return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
            /* NOTE: But do not explicitly free driver, the connector object. Connector/C++ takes care of freeing that.
             * driverを呼び出し側がdeleteしてはいけない。mysql側が面倒を見ている。
             * https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-connecting.html
             */
        }

        /// @brief SQLクエリを実行し指定したIDデータを返す関数。
        /// @param query SQLクエリ。データ取得クエリ (Selectなど) を想定
        /// @param id SQLクエリに渡すID。
        /// @return データアクセスのためのクラス。使い方は `geniee::connector::mysql::fetchModels` のドキュメント参照。
        std::unique_ptr<sql::ResultSet> fetchModelByID(std::string query, unsigned int id) {
            std::lock_guard<std::mutex> lock(mut);
            sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();  // この関数はthread safe *ではないので* 排他制御
            std::unique_ptr<sql::Connection> con(driver->connect(geniee::config::db_uri, geniee::config::db_user, geniee::config::db_pass));
            std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(query));
            stmt->setUInt(1, id);  // ここでIDをセット。
            return std::unique_ptr<sql::ResultSet>(stmt->executeQuery());
        }

    private:
        std::mutex mut;
    };
}  // namespace geniee::connector::mysql
