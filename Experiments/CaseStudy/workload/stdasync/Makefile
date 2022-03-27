all: eg1 
eg1: eg1.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib eg1.cpp -lmysqlcppconn -pthread -o eg1



