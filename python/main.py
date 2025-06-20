from server.server import HttpServer
from server.auction import auction_handler
from connector.mysql_connector import MySQLConnector

import os

if __name__ == "__main__":
    pid = os.getpid()
    print(f"pid = {pid}", flush=True)

    mysql_connector = MySQLConnector()
    server = HttpServer()
    server.setup_cb(endpoint="/auction",
                    handler=auction_handler, arg=mysql_connector)
    server.run()
