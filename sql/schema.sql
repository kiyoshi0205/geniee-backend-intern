DROP DATABASE IF EXISTS `geniee`;

CREATE DATABASE `geniee`;

USE `geniee`;

CREATE TABLE `advertisers` (
    `advertiser_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `advertiser_name` varchar(255) NOT NULL,
    `category_blocking_type` tinyint(1) unsigned NULL,
    `domain` varchar(255) NOT NULL,
    PRIMARY KEY (`advertiser_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;



CREATE TABLE `ads_sizes` (
    `size_id` int(10) unsigned NOT NULL,
    `ads_width` int(10) unsigned NOT NULL,
    `ads_height` int(10) unsigned NOT NULL, 
    PRIMARY KEY (`size_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


CREATE TABLE `adses` (
    `ads_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
    `advertiser_id` int(10) unsigned NOT NULL,
    `price` decimal(15, 4) unsigned NOT NULL,
    `ads_txt` varchar(255) NOT NULL,
    `size_id` int(10) unsigned NOT NULL,
    PRIMARY KEY (`ads_id`),
    FOREIGN KEY `fk_advertiser_id` (`advertiser_id`) REFERENCES `advertisers` (`advertiser_id`),
    FOREIGN KEY `fk_size_id` (`size_id`) REFERENCES `ads_sizes` (`size_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


CREATE TABLE `category_blockings` (
    `advertiser_id` int(10) unsigned NOT NULL,
    `category_id` int(10) unsigned NOT NULL,
    PRIMARY KEY (`advertiser_id`, `category_id`),
    FOREIGN KEY `fk_advertiser_id` (`advertiser_id`) REFERENCES `advertisers` (`advertiser_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


CREATE TABLE `ads_features` (
    `ads_id` int(10) unsigned NOT NULL,
    `feature` int(10) NOT NULL,
    PRIMARY KEY (`ads_id`),
    FOREIGN KEY `fk_ads_id` (`ads_id`) REFERENCES `adses` (`ads_id`)    
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
