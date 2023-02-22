#!/bin/bash

echo "conf_test1 test"
echo "curl -4 --resolve conf_test_1:8080:127.0.0.1 http://conf_test_1:8080/"
echo "press enter key to run"
read
curl -4 --resolve conf_test_1:8080:127.0.0.1 http://conf_test_1:8080/
echo "------------------------------------------------------------------------------------------"
echo ""

echo "conf_test_same_port test"
echo "curl -4 --resolve conf_test_same_port:8080:127.0.0.1 http://conf_test_same_port:8080/"
echo "press enter key to run"
read
curl -4 --resolve conf_test_same_port:8080:127.0.0.1 http://conf_test_same_port:8080/
echo "------------------------------------------------------------------------------------------"
echo ""

echo "default_error_page test"
echo "curl http://localhost:8083/no_index.html"
echo "press enter key to run"
read
curl http://localhost:8083/no_index.html
echo "------------------------------------------------------------------------------------------"
echo ""

echo "limit_client_body test"
echo "curl -X POST -H \"Content-Type: plain/text\" --data \"BODY IS HERE write something shorter or longer than body limit\""
echo "press enter key to run"
read
curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://localhost:8084/index.html
echo "------------------------------------------------------------------------------------------"
echo ""

echo "different_dir test"
echo "curl http://localhost:8085/"
echo "press enter key to run"
read
curl http://localhost:8085/
echo "------------------------------------------------------------------------------------------"
echo ""

echo "method_except test - limit_except"
echo "curl -vX DELETE --header "ABC" http://localhost:8086/permission.txt"
echo "press enter key to run"
read
curl -vX DELETE http://localhost:8086/permission.txt
echo "------------------------------------------------------------------------------------------"
echo ""

echo "method_except test - permission"
echo "curl -vX DELETE http://localhost:8087/permission.txt"
echo "press enter key to run"
read
curl -vX DELETE http://localhost:8087/permission.txt
echo "------------------------------------------------------------------------------------------"
echo ""
