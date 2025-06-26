USE `geniee`;

INSERT INTO `advertisers` (`advertiser_id`, `advertiser_name`, `category_blocking_type`, `domain`)
VALUES (1, "test_advertiser", 0, "K789YZWFinalCodeXYZ000CXYZ456YZ0.com");

INSERT INTO `ads_sizes` (`size_id`, `ads_width`, `ads_height`)
VALUES (1, 200, 300);

INSERT INTO `adses` (`ads_id`, `advertiser_id`, `price`, `ads_txt`)
VALUES (1, 1, 102.45, "test_ads");

INSERT INTO `category_blocking` (`advertiser_id`, `category_id`)
VALUES (1, 4), (1, 10);

INSERT INTO `ads_features` (`ads_id`, `feature`)
VALUES (1, 41);
