#!/bin/bash

pwdpath=$(pwd)

AllSrv="
loginserver
gateserver 
gameserver 
dataserver 
centralserver
biserver
paymentserver
"
signo="-9"
if [[ $# != 0 ]]; then
	signo="-9"
fi

for srv in ${AllSrv}
do
	full_path=$pwdpath/$srv
	srvpid=$(ps aux | grep -v "grep" | grep ${full_path} | awk '{print $2}') 
	if [ "${srvpid}" != "" ]
	then
		kill $signo ${srvpid}
		text="kill $signo ${srv}:${srvpid} ..."
	else
		text="${srv} is not Running ..."
	fi

	echo -e '\033[0;31;1m'${text}'\033[0m'
done
