all: try1 wl2 try2
try1: try1.cpp ../../../DataStructures/CountedBtree.cpp ../../../DataStructures/DynamicArray.cpp \
../../../DataStructures/LinkedList.cpp ../../../DataStructures/StandardArray.cpp
	g++ -std=c++11 -I /usr/include/boost  \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib try1.cpp -lmysqlcppconn -o try1

wl2: wl2.cpp ../../../DataStructures/CountedBtree.cpp ../../../DataStructures/DynamicArray.cpp \
../../../DataStructures/LinkedList.cpp ../../../DataStructures/StandardArray.cpp ../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../DataStructures/CountedBtree.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/StandardArray.cpp ../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib wl2.cpp -lmysqlcppconn -pthread -o wl2



try2: try2.cpp ../../../DataStructures/CountedBtree.cpp ../../../DataStructures/DynamicArray.cpp \
../../../DataStructures/LinkedList.cpp ../../../DataStructures/StandardArray.cpp ../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../DataStructures/CountedBtree.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/StandardArray.cpp ../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib try2.cpp -lmysqlcppconn -pthread -o try2
