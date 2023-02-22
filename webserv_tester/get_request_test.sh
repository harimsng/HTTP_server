#!/bin/bash

echo "creating test files"
echo "read_only" > ./html/tests/get_request_test/read_only
chmod 444 ./html/tests/get_request_test/read_only
echo "no_permission" > ./html/tests/get_request_test/no_permission
chmod 333 ./html/tests/get_request_test/no_permission
echo "press enter key to continue"
read -n 1

set -e

echo "--------------------------------------------------------------------------------"
echo "expected: 200 OK"
echo "curl -X GET -4 --resolve server1:8081:127.0.0.1 http://server1:8081/read_only"
curl -vX GET -4 --resolve server1:8081:127.0.0.1 http://server1:8081/read_only
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 403 Forbidden"
echo "curl -X GET -4 --resolve server3:8083:127.0.0.1 http://server3:8082/no_permission"
curl -vX GET -4 --resolve server3:8082:127.0.0.1 http://server3:8082/no_permission
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 404 Not Found"
echo "curl -X GET -4 --resolve server4:8083:127.0.0.1 http://server4:8083/should_not_exist"
curl -vX GET -4 --resolve server4:8083:127.0.0.1 http://server4:8083/should_not_exist
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 405 Method Not Allowed"
echo "curl -X GET -4 --resolve server5:8084:127.0.0.1 http://server5:8084/read_only"
curl -vX GET -4 --resolve server5:8084:127.0.0.1 http://server5:8084/read_only

echo "--------------------------------------------------------------------------------"
echo "autoindex (directory listing)"
echo "curl -X GET -4 --resolve server6:8086:127.0.0.1 http://server6:8086/"
curl -vX GET -4 --resolve server6:8086:127.0.0.1 http://server6:8086/

rm -f ./html/tests/get_request_test/*
