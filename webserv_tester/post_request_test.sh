#!/bin/bash

echo "creating test files"
echo "file" > ./html/tests/post_request_test/file
echo "file.php" > ./html/tests/post_request_test/file.php
echo "read_only" > ./html/tests/post_request_test/read_only
chmod 444 ./html/tests/post_request_test/no_permission
echo "no_permission" > ./html/tests/post_request_test/no_permission
chmod 333 ./html/tests/post_request_test/no_permission
echo "press enter key to continue"
cp ./cgi-bin/php-cgi ./cgi-bin/php-cgi-no-permission
chmod 000 ./cgi-bin/php-cgi-no-permission
read -n 1

set -e

echo "--------------------------------------------------------------------------------"
echo "expected: 200 OK"
echo "curl -X POST -4 --data "0123456789" --resolve server1:8081:127.0.0.1 http://server1:8081/file"
curl -vX POST -4 --data "0123456789" --resolve server1:8081:127.0.0.1 http://server1:8081/file
echo "press enter key to continue"
read -n 1

# echo "--------------------------------------------------------------------------------"
# echo "expected: 204 No Content"
# echo "curl -X POST -4 --resolve server1:8081:127.0.0.1 http://server1:8081/read_only"
# curl -vX POST -4 --resolve server1:8081:127.0.0.1 http://server1:8081/read_only
# echo "press enter key to continue"
# read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 403 Forbidden"
echo "curl -X POST -4 --data "0123456789" --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission"
curl -vX POST -4 --data "0123456789" --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 404 Not Found"
echo "curl -X POST -4 --data "0123456789" --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist"
curl -vX POST -4 --data "0123456789" --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 405 Method Not Allowed"
echo "curl -X POST -4 --data "0123456789" --resolve server5:8085:127.0.0.1 http://server5:8085/read_only"
curl -vX POST -4 --data "0123456789" --resolve server5:8085:127.0.0.1 http://server5:8085/read_only
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 413 Content Too Long"
echo "curl -X POST -4 --data "01234567890" --resolve server6:8086:127.0.0.1 http://server6:8086/read_only"
curl -vX POST -4 --data "01234567890" --resolve server6:8086:127.0.0.1 http://server6:8086/read_only
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 500 Internal Server Error"
echo "curl -X POST -4 --data "0123456789" --resolve server7:8087:127.0.0.1 http://server7:8087/file.php"
curl -vX POST -4 --data "0123456789" --resolve server7:8087:127.0.0.1 http://server7:8087/file.php
read -n 1

rm -f ./html/tests/post_request_test/*
rm -f ./cgi-bin/php-cgi-no-permission
