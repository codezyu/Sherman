#!/bin/bash

addr=$(head -1 ../memcached.conf)
sshport=$(awk 'NR==3{print}' ../memcached.conf)
echo "ssh prot: ${sshport}"
port=$(awk 'NR==2{print}' ../memcached.conf)
echo "memcached port: ${port}"
key_path="~/.ssh/id_rsa_project"
# kill old me
ssh -i ${key_path} ${addr}  -p ${sshport}  "cat /tmp/memcached.pid | xargs kill"

# launch memcached
ssh -i ${key_path} ${addr}  -p ${sshport}  "memcached -u root -l ${addr} -p  ${port} -c 10000 -d -P /tmp/memcached.pid"
sleep 1

# init 
echo -e "set serverNum 0 0 1\r\n0\r\nquit\r" | nc ${addr} ${port}
echo -e "set clientNum 0 0 1\r\n0\r\nquit\r" | nc ${addr} ${port}
