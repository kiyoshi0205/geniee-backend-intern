from model.ads import Ads
from model.category_blocking import CategoryBlocking
from model.advertiser import Advertiser, CategoryBlockingType
from model.ads_feature import AdsFeature
from model.ads_size import AdSize
from connector.mysql_connector import MySQLConnector

from typing import List, Optional


def fetchAdses(mysql_connector: MySQLConnector) -> List[Ads]:
    """広告案件が格納されている `Adses` 全件をMySQLサーバから取得する関数

    Args:
        mysql_connector (MySQLConnector): MySQLに接続しクエリ実行を行うクラス

    Returns:
        List[Ads]: Adsの配列
    """
    adses = []
    with mysql_connector.fetchModels(query="SELECT ads_id, advertiser_id, price, ads_txt, size_id FROM adses ORDER BY ads_id;") as cursor:
        data = cursor.fetchall()
        adses = [Ads(ads_id=ads_id, advertiser_id=advertiser_id, price=float(
            price), ads_txt=ads_txt, size_id=size_id) for ads_id, advertiser_id, price, ads_txt, size_id in data]

    return adses


def fetchAdSize(mysql_connector: MySQLConnector, size_id: int) -> Optional[AdSize]:
    """
    特定の広告サイズ `AdSize` をMySQLサーバから取得する関数
    Args:
        mysql_connector (MySQLConnector): MySQLに接続しクエリ実行を行うクラス
        size_id (int): 取得したい広告サイズID

    Returns:
        Optional[AdSize]: 該当広告サイズが存在すればそれを、存在しない場合はNoneを返す
    """
    with mysql_connector.fetchModelByID(query="SELECT ads_width, ads_height FROM ads_sizes WHERE size_id = %s LIMIT 1; ", id=size_id) as cursor:
        data = cursor.fetchone()
        if data:
            ads_width, ads_height = data
            return AdSize(size_id=size_id, ads_width=ads_width, ads_height=ads_height)
        else:
            return None


def fetchCategoryBlocking(mysql_connector: MySQLConnector, advertiser_id: int) -> Optional[CategoryBlocking]:
    """特定の広告主のカテゴリ情報が記録されている `CategoryBlocking` をMySQLサーバから取得する関数。
    Args:
        mysql_connector (MySQLConnector): MySQLに接続しクエリ実行を行うクラス
        advertiser_id (int): カテゴリ情報を取得したい広告主ID

    Returns:
        Optional[CategoryBlocking]: 該当広告主の `CategoryBlocking` が存在すればそれを、存在しない場合はNoneを返す
    """
    category_ids: List[int] = []
    with mysql_connector.fetchModelByID(query="SELECT category_id FROM category_blockings WHERE advertiser_id = %s;", id=advertiser_id) as cursor:
        data = cursor.fetchall()
        category_ids = [category_id for (category_id,) in data]

    return CategoryBlocking(advertiser_id=advertiser_id, category_ids=category_ids) if category_ids else None


def fetchAdvertiser(mysql_connector: MySQLConnector, advertiser_id: int) -> Optional[Advertiser]:
    """特定広告主 `Advertiser` をMySQLサーバから取得する関数

    Args:
        mysql_connector (MySQLConnector): MySQLに接続しクエリ実行を行うクラス
        advertiser_id (int): 取得したい広告主ID

    Returns:
        Optional[Advertiser]: 該当広告主の `Advertiser` が存在すればそれを、存在しない場合はNoneを返す
    """
    with mysql_connector.fetchModelByID(query="SELECT advertiser_name, category_blocking_type, domain FROM advertisers WHERE advertiser_id = %s LIMIT 1; ", id=advertiser_id) as cursor:
        data = cursor.fetchone()

        if data:
            advertiser_name, category_blocking_type, domain = data
            return Advertiser(advertiser_id=advertiser_id, advertiser_name=advertiser_name, category_blocking_type_id=CategoryBlockingType(category_blocking_type), domain=domain)
        else:
            return None


def fetchAdsFeature(mysql_connector: MySQLConnector, ads_id: int) -> Optional[AdsFeature]:
    """特定広告案件の `AdsFeature` をMySQLサーバから取得する関数

    Args:
        mysql_connector (MySQLConnector): MySQLに接続しクエリ実行を行うクラス
        ads_id (int): 取得したい広告案件ID

    Returns:
        Optional[AdsFeature]: 該当広告主の `AdsFeature` が存在すればそれを、存在しない場合はNoneを返す
    """
    with mysql_connector.fetchModelByID(query="SELECT feature FROM ads_features WHERE ads_id = %s LIMIT 1;", id=ads_id) as cursor:
        data, = cursor.fetchone()
        return AdsFeature(ads_id=ads_id, feature=data) if data else None
