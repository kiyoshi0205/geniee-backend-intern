#!/bin/sh

dir=$(cd $(dirname $0); pwd)

# create init data from seed and dataset size
sudo -umysql python3 $dir/create_data.py $@

# clear old data
echo 'DROP DATABASE geniee;' | mysql -uroot -pr4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ  2> /dev/null

# declare schema
mysql -uroot -pr4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ < $dir/../sql/schema.sql 2> /dev/null

# insert data to DB
for i in advertisers ads_sizes adses category_blockings ads_features
do
    echo "USE geniee; LOAD DATA INFILE '/var/lib/mysql-files/$i.csv' INTO TABLE $i FIELDS TERMINATED BY ',';" | mysql -uroot -pr4RqBb4ZDC6QqVKkuhkQKZZP7zE_9zQ 2> /dev/null
done
