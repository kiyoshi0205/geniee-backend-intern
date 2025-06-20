from config.config import db_user, db_pass, db_host, db_port, db_database

import mysql.connector


class MySQLResults:
    def __init__(self, connector, cursor):
        self.connector = connector
        self.cursor = cursor

    def __enter__(self):
        return self.cursor

    def __exit__(self, exception_type, exception_value, traceback):
        self.cursor.close()
        self.connector.close()


class MySQLConnector:
    """MySQLに接続しデータを取得する関数群が定義されているクラス"""

    def fetchModels(self, query: str) -> MySQLResults:
        """SQLクエリを実行しデータ全てを返す関数

        Args:
            query (str): データ取得クエリ (Selectなど) を想定

        Returns:
            MySQLResults: データアクセスのためのクラス。基本的にはwith句と共に使い、for文で内部データを取得できる
        """
        conn = mysql.connector.connect(user=db_user, password=db_pass, host=db_host, port=db_port, database=db_database)
        cursor = conn.cursor()
        cursor.execute(query)

        return MySQLResults(conn, cursor)

    def fetchModelByID(self, query: str, id: int) -> MySQLResults:
        """SQLクエリを実行し指定したIDデータを返す関数

        Args:
            query (str): データ取得クエリ (Selectなど) を想定
            id (int): SQLクエリに渡すID

        Returns:
            MySQLResults: データアクセスのためのクラス。詳しくは `fetchModels` のドキュメント参照
        """
        conn = mysql.connector.connect(user=db_user, password=db_pass, host=db_host, port=db_port, database=db_database)
        cursor = conn.cursor()
        cursor.execute(query, (id,))

        return MySQLResults(conn, cursor)
