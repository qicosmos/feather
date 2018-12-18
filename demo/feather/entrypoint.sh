#!/bin/bash

mysql -h mariadb --user=root --password=secret -e 'select 1 from feather.article' 2> /dev/null

if [ $? = 1 ]; then
	mysql -h mariadb --user=root --password=secret -e "CREATE USER 'feather'@'%' IDENTIFIED BY '123456';" 2> /dev/null
	mysql -h mariadb --user=root --password=secret -e "GRANT ALL ON *.* TO 'feather'@'%';" 2> /dev/null
	mysql -h mariadb --user=feather --password=123456 -e "CREATE DATABASE IF NOT EXISTS \`feather\` DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_unicode_ci" 2> /dev/null
	mysql -h mariadb --user=feather --password=123456 feather < feather.sql 2> /dev/null
fi

exec ./feather
