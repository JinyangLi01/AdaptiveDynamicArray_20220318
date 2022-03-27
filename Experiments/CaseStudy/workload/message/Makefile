all: message1 message2

message1: message1.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib message1.cpp -lmysqlcppconn -pthread -o message1


message2: message2.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib message2.cpp -lmysqlcppconn -pthread -o message2


