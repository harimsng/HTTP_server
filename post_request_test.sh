#!/bin/bash

echo "creating test files"
echo "read_only" > ./html/tests/get_request_test/read_only
chmod 444 ./html/tests/get_request_test/no_permission
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
echo "expected: 400 Bad Request"
echo "curl -X GET -4 --resolve server2:8082:127.0.0.1 --header \"invalid header\" http://server2:8082/read_only"
curl -vX GET -4 --resolve server2:8082:127.0.0.1 --header "invalid header:A" http://server2:8082/read_only
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 403 Forbidden"
echo "curl -X GET -4 --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission"
curl -vX GET -4 --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 404 Not Found"
echo "curl -X GET -4 --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist"
curl -vX GET -4 --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist
echo "press enter key to continue"
read -n 1

echo "--------------------------------------------------------------------------------"
echo "expected: 405 Method Not Allowed"
echo "curl -X GET -4 --resolve server5:8085:127.0.0.1 http://server5:8085/read_only"
curl -vX GET -4 --resolve server5:8085:127.0.0.1 http://server5:8085/read_only

rm -f ./html/tests/post_request_test/*
