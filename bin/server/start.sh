#!/bin/bash
#//程序崩溃时生成core文件，并且不限制文件大小，修改最大文件描述符
ulimit -c unlimited		
ulimit -s unlimited	
ulimit -n 10240
#//修改core文件名格式和路径
echo "1" > /proc/sys/kernel/core_uses_pid
echo "core-%e-%s-%t" > /proc/sys/kernel/core_pattern

pwdpath=$(pwd)
echo -e '\033[0;31;1m'pwdpath = $pwdpath'\033[0m'

export LD_LIBRARY_PATH=$pwdpath/lib

AllSrv="${pwdpath}/centralserver
	  	  ${pwdpath}/gameserver
	    	${pwdpath}/gateserver
	    	${pwdpath}/dataserver
	  	  ${pwdpath}/loginserver"

rm -f nohup.out
flag=0
for srv in ${AllSrv} 
do
	MM=$(ps aux | grep -v "grep" | grep ${srv} | wc -l) 
	if [ $MM == 0 ];then
		srvexe=${srv##*/}
		
		text="Start ${srvexe} ......"
		echo -e '\033[0;31;1m'${text}'\033[0m'

		nohup ${srv} > out.log 2>&1 &
		sleep 5
		flag=1
	fi
done

if [ $flag == 1 ];then
	sleep 2
fi
ps aux | grep ${pwdpath} | grep -v grep
