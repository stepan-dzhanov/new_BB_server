#!/bin/bash

sshpass -p "casper77" scp -r root@174.138.14.251:/home/new_server /home/stepan
cd /home/stepan/new_server
#rm *.o
#killall app
rm app
make
reboot

