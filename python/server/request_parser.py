import json as json_lib
from model.request import Request
from http.server import BaseHTTPRequestHandler


def parseRequest(r: BaseHTTPRequestHandler) -> Request:
    """http.serverのrequest handlerを自作モデルのリクエストに変換

    Args:
        r (BaseHTTPRequestHandler): http.serverのリクエスト

    Returns:
        Request: 情報が格納された自作の `model.request.Request`
    """
    len = int(r.headers.get("content-length"))
    data = r.rfile.read(len).decode("utf-8")
    json = json_lib.loads(data)

    auction_id = ""
    category_ids: list[int] = []
    feature = 0.0
    zone_width = 0
    zone_height = 0
    block_domains: list[str] = []

    if "auction_id" in json:
        auction_id = json["auction_id"]

    if "category_ids" in json:
        category_ids = json["category_ids"]

    if "zone_feature" in json:
        feature = json["zone_feature"]

    if "zone_width" in json:
        zone_width = json["zone_width"]

    if "zone_height" in json:
        zone_height = json["zone_height"]

    if "block_domains" in json:
        block_domains = json["block_domains"]

    return Request(auction_id=auction_id, category_ids=category_ids, feature=feature, zone_width=zone_width, zone_height=zone_height, block_domains=block_domains)
