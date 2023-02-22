#!/bin/bash

echo "conf_test1 test"
echo "curl -4 --resolve conf_test_1:8080:127.0.0.1 http://conf_test_1:8080/"
echo "press enter key to run"
read
curl -4 --resolve conf_test_1:8080:127.0.0.1 http://conf_test_1:8080/
echo "------------------------------------------------------------------------------------------"
echo ""

