from config.config import port, n_thread

from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ForkingMixIn, ThreadingMixIn
from concurrent.futures import ThreadPoolExecutor
from urllib.parse import urlparse
from typing import Callable, Any


class HTTPHandler(BaseHTTPRequestHandler):
    """Httpリクエストを処理するクラス。内部で登録したエンドポイントを呼び出している"""

    endpoint_mapping = {}

    def do_POST(self):
        url = urlparse(self.path)
        if url.path in HTTPHandler.endpoint_mapping:
            handler, arg = HTTPHandler.endpoint_mapping[url.path]
            handler(self, arg)

    def log_message(self, format: str, *args: Any):
        pass


class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
    """http requestを上限 `n_thread` スレッドで処理するHTTP Server"""

    def __init__(self, server_address, RequestHandlerClass, bind_and_activate=True):
        super().__init__(server_address, RequestHandlerClass, bind_and_activate)
        self.executor = ThreadPoolExecutor(max_workers=n_thread)

    def process_request(self, request, client_address):
        self.executor.submit(self.process_request_thread, request, client_address)

    def server_close(self):
        super().server_close()
        self.executor.shutdown(wait=True)


class ProcessHTTPServer(ForkingMixIn, HTTPServer):
    """http requestを上限 `n_thread` プロセスで処理するHTTP Server"""

    max_children = n_thread


class HttpServer:
    """Httpプロセスをまとめて複数リクエスト処理を受けられるようにしたサーバ"""

    def __init__(self):
        self.server = ThreadingHTTPServer(("", port), HTTPHandler)

    def setup_cb(self, endpoint: str, handler: Callable[[BaseHTTPRequestHandler, Any], None], arg: Any):
        """エンドポイントを登録する

        Args:
            endpoint (str): エンドポイント名。
            handler (Callable[[BaseHTTPRequestHandler, Any]]): 呼び出す関数
            arg (Any): エンドポインタに渡すデータ。
        """

        HTTPHandler.endpoint_mapping[endpoint] = (handler, arg)

    def run(self):
        """サーバを実行し、リクエストを持つ"""
        print("listening...")
        self.server.serve_forever()
