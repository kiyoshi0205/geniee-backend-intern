from model.response import Response
from model.ads import Ads
from model.ads_feature import AdsFeature
from model.request import Request

from connector.mysql_connector import MySQLConnector
from connector.fetch_model_from_db import fetchAdses, fetchAdsFeature, fetchAdvertiser, fetchCategoryBlocking, fetchAdSize
from blocking.category_blocking import categoryBlocking
from targeting.feature_targeting import featureTargeting
from server.request_parser import parseRequest
from server.response_sender import sendNobidResponse, sendWinnerResponse

from collections import defaultdict
from typing import Optional
from http.server import BaseHTTPRequestHandler


def auction(request: Request, mysql_connector: MySQLConnector) -> Optional[Response]:
    """処理の中心。リクエストや広告情報を元にオークションを行い、適切な広告を選択する。

    Args:
        request (Request): リクエストに必要な情報を格納した自作クラス
        mysql_connector (MySQLConnector): エンドポイント設定時にあらかじめ指定したオブジェクトを渡すことができる。今回はMySQLのコネクタを使いやすくしたクラスを共有。
    """

    # 広告主 -> 広告の対応を保存するための変数
    advertiser_adses: defaultdict[int, list[Ads]] = defaultdict(list)
    adses = fetchAdses(mysql_connector=mysql_connector)
    # hint:ループの中でデータベースからデータを取得する処理が行われている(N+1問題)
    # 全てのデータを一括で取得するとSQLとの接続が少なくて高速化できます
    adses_satisfy_size: list[Ads] = []
    # まずは広告のサイズを確認し、リクエストで要求されているサイズと一致するものを保存する
    for ads in adses:
        ad_size = fetchAdSize(
            mysql_connector=mysql_connector, size_id=ads.size_id)
        if ad_size is not None:
            if ad_size.ads_width == request.zone_width and ad_size.ads_height == request.zone_height:
                adses_satisfy_size.append(ads)

    # 次に広告主ドメインを確認し、リクエストで指定されたドメイン名が含まれていればブロックする
    ads_satisfy_domain: list[Ads] = []
    for ads in adses_satisfy_size:
        advertiser = fetchAdvertiser(
            mysql_connector=mysql_connector, advertiser_id=ads.advertiser_id)
        if advertiser is None:
            continue

        is_contained_block_domain = False
        advertiser_domain = advertiser.domain
        for domain in request.block_domains:
            if domain in advertiser_domain:
                is_contained_block_domain = True

        if not is_contained_block_domain:
            ads_satisfy_domain.append(ads)

    # 次にカテゴリ制御を行い、有効な広告主のみを取り出して保存する
    for ads in ads_satisfy_domain:
        advertiser = fetchAdvertiser(
            mysql_connector=mysql_connector, advertiser_id=ads.advertiser_id)
        category_blocking = fetchCategoryBlocking(
            mysql_connector=mysql_connector, advertiser_id=ads.advertiser_id)
        if advertiser is not None and category_blocking is not None:
            blocked = categoryBlocking(
                advertiser=advertiser, category_blocking=category_blocking, request_category=request.category_ids)
            if blocked:
                continue
        advertiser_adses[ads.advertiser_id].append(ads)

    # 特徴量でターゲティングした後の結果を保存
    ads_targetted: list[Ads] = []
    # 特徴量でターゲティングを行い、広告主ごとに最適な広告を選択
    for _, adv_adses in advertiser_adses.items():
        ads_features: list[AdsFeature] = []
        for ads in adv_adses:
            ads_feature = fetchAdsFeature(
                mysql_connector=mysql_connector, ads_id=ads.ads_id)
            if ads_feature is not None:
                ads_features.append(ads_feature)

        if len(ads_features) > 0:
            best_ads = featureTargeting(
                ads_features=ads_features, request_feature=request.feature)

            for ads in adv_adses:
                if ads.ads_id == best_ads.ads_id:
                    ads_targetted.append(ads)

    # 最も価格が高い広告を選択するために逆順ソート
    ads_targetted.sort(key=lambda ads: (ads.price, -ads.ads_id), reverse=True)

    if len(ads_targetted) == 0:
        return None
    else:
        winner = ads_targetted[0]  # 先頭の広告が最も高価
        response = Response(request.auction_id, winner.ads_txt, winner.price)
        return response


def auction_handler(http_request: BaseHTTPRequestHandler, arg):
    """サーバから叩かれるエンドポイント。処理しやすいように加工を行いメイン処理を呼び出す

    Args:
        http_request (BaseHTTPRequestHandler):
        arg: handlerに渡したい引数
    """

    # request handlerだと使い勝手が悪いので自作モデルに移動
    request = parseRequest(r=http_request)
    response = auction(request, arg)
    if response is None:
        sendNobidResponse(r=http_request)
    else:
        sendWinnerResponse(r=http_request, response=response)
