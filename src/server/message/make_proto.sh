#!/bin/sh

cd source
rm *.pb.cc *.pb.h 1>/dev/null 2>&1


cd ..

for i in proto/*.proto
do
	./protoc -I=./proto --cpp_out=./source $i
	if [ "$?" -eq 0 ]
	then
		echo "已生成$i.pb.h $i.pb.cc"
	else
		exit 1
	fi
done

