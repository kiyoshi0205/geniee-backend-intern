from model.response import Response

import json as json_lib
from http.server import BaseHTTPRequestHandler
from http import HTTPStatus


def sendWinnerResponse(r: BaseHTTPRequestHandler, response: Response):
    """リクエスト元にオークション情報をJSON形式にして返す

    Args:
        r (BaseHTTPRequestHandler): http.serverのリクエスト情報
        response (Response): オークションに勝った広告情報などが保存されているモデル
    """
    json = {"auction_id": response.auction_id, "ads": response.ads, "price": response.price}
    data = json_lib.dumps(json, separators=(",", ":"))
    r.send_response(HTTPStatus.OK)
    r.end_headers()
    r.wfile.write(data.encode())


def sendNobidResponse(r: BaseHTTPRequestHandler):
    """該当する広告がない場合に `204 No Contents` を返す

    Args:
        r (BaseHTTPRequestHandler): http.serverのリクエスト情報
    """
    r.send_response(HTTPStatus.NO_CONTENT)
    r.end_headers()
