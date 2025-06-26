import random
import argparse
import string

from sample_data import *

ADVERTISER_TEMPLATE = "{advertiser_id},{advertiser_name},{category_blocking_type},{advertiser_domain}\n"
ADSES_TEMPLATE = "{ads_id},{advertiser_id},{price},{ads_txt},{size_id}\n"
CATEGORY_BLOCKINGS_TEMPLATE = "{advertiser_id},{category_id}\n"
ADS_FEATURES_TEMPLATE = "{ads_id},{feature}\n"
ADS_SIZE_TEMPLATE = "{size_id},{ads_width},{ads_height}\n"


def random_prohibit_string(length: int) -> str:
    result = list[str]()
    remaining_len = length

    while remaining_len > 0:
        prohibit_string = random.choice(PROHIBIT_STRINGS)
        if len(prohibit_string) <= remaining_len:
            result.append(prohibit_string)
            remaining_len -= len(prohibit_string)
        else:
            break

    if remaining_len > 0:
        filler = "".join(random.choices(
            string.ascii_letters + string.digits, k=remaining_len))
        result.append(filler)

    random.shuffle(result)
    return "".join(result)


def random_string(length: int) -> str:
    return "".join(random.choices(string.ascii_letters + string.digits, k=length))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="seed値を受け取りランダムなSQLデータを生成")
    parser.add_argument("seed", help="乱数のシード。シードが一致していれば生成されるデータは同一")
    parser.add_argument("datasize", choices=[
                        "small", "medium", "large"], help="生成されるデータ規模")
    args = parser.parse_args()
    seed = args.seed
    datasize = args.datasize

    random.seed(seed)
    coff = 1
    if datasize == "small":
        coff = 2
    elif datasize == "medium":
        coff = 30
    elif datasize == "large":
        coff = 60
    else:
        raise ValueError("invalid datasize")

    with open("/var/lib/mysql-files/advertisers.csv", mode="w") as f_advertisers, open("/var/lib/mysql-files/adses.csv", mode="w") as f_adses, open(
        "/var/lib/mysql-files/category_blockings.csv", mode="w"
    ) as f_category_blockings, open("/var/lib/mysql-files/ads_features.csv", mode="w") as f_ads_features, open(
        "/var/lib/mysql-files/ads_sizes.csv", mode="w"
    ) as f_ads_sizes:

        for i, size in enumerate(CANDIDATE_ZONE_SIZES):
            sql = ADS_SIZE_TEMPLATE.format(
                size_id=i + 1, ads_width=size[0], ads_height=size[1])
            f_ads_sizes.write(sql)

        sum_ads = 0
        n_advertisers = random.randint(10 * coff, 15 * coff)
        for advertiser_id in range(1, n_advertisers + 1):
            advertiser_name = random_string(16)
            category_blocking_type = random.randint(0, 1)
            advertiser_domain = random_prohibit_string(32) + ".com"
            sql = ADVERTISER_TEMPLATE.format(
                advertiser_id=advertiser_id, advertiser_name=advertiser_name, category_blocking_type=category_blocking_type, advertiser_domain=advertiser_domain
            )
            f_advertisers.write(sql)

            n_category_blockings = random.randint(1, 10)
            for category_id in random.sample(range(1, 21), k=n_category_blockings):
                sql = CATEGORY_BLOCKINGS_TEMPLATE.format(
                    advertiser_id=advertiser_id, category_id=category_id)
                f_category_blockings.write(sql)

            n_adses = random.randint(10 * coff, 15 * coff)
            for id in range(1, n_adses + 1):
                ads_id = id + sum_ads
                price = round(random.uniform(0.01, 100.0), 4)
                ads_txt = random_string(64)
                size_id = random.choice(ADS_SIZE_IDS)
                sql = ADSES_TEMPLATE.format(
                    ads_id=ads_id, advertiser_id=advertiser_id, price=price, ads_txt=ads_txt, size_id=size_id)
                f_adses.write(sql)

                feature = random.randint(-100000, 100000)
                sql = ADS_FEATURES_TEMPLATE.format(
                    ads_id=ads_id, feature=feature)
                f_ads_features.write(sql)

            sum_ads += n_adses
