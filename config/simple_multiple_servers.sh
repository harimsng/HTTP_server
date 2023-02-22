#!/bin/bash
echo "server1 test"
echo "curl -4 --resolve server1:8081:127.0.0.1 http://server1:8081/"
curl -4 --resolve server1:8081:127.0.0.1 http://server1:8081/
echo "press enter key to continue"
read

echo "server2 test"
echo "curl -4 --resolve server2:8082:127.0.0.1 http://server2:8082/"
curl -4 --resolve server2:8082:127.0.0.1 http://server2:8082/
echo "press enter key to continue"
read

echo "server3 test"
echo "curl -4 --resolve server3:8083:127.0.0.1 http://server3:8083/"
curl -4 --resolve server3:8083:127.0.0.1 http://server3:8083/
