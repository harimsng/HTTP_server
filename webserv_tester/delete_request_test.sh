#!/bin/bash

echo "creating test files"
echo "should_be_deleted" > ./html/tests/delete_request_test/should_be_deleted
echo "file" > ./html/tests/delete_request_test/file
echo "no_permission" > ./html/tests/delete_request_test/no_permission
chmod 555 ./html/tests/delete_request_test/no_permission
echo "press enter key to continue"
read -n 1

set -e

echo "expected: 200 OK"
echo "curl -X DELETE -4 --resolve server1:8081:127.0.0.1 http://server1:8081/should_be_deleted"
curl -vX DELETE -4 --resolve server1:8081:127.0.0.1 http://server1:8081/should_be_deleted
echo "press enter key to continue"
read -n 1

echo "expected: 403 Forbidden"
echo "curl -X DELETE -4 --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission"
curl -vX DELETE -4 --resolve server3:8083:127.0.0.1 http://server3:8083/no_permission
echo "press enter key to continue"
read -n 1

echo "expected: 404 Not Found"
echo "curl -X DELETE -4 --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist"
curl -vX DELETE -4 --resolve server4:8084:127.0.0.1 http://server4:8084/should_not_exist
echo "press enter key to continue"
read -n 1

echo "expected: 405 Method Not Allowed"
echo "curl -X DELETE -4 --resolve server5:8085:127.0.0.1 http://server5:8085/file"
curl -vX DELETE -4 --resolve server5:8085:127.0.0.1 http://server5:8085/file

rm -f ./html/tests/delete_request_test/*
