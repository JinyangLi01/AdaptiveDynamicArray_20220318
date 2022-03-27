all: wl2 pool_eg pooling1  pool_eg3 pool_eg4

pool_eg: pool_eg.cpp ThreadPool.h 
	g++ -std=c++11 -I /usr/include/boost  \
	ThreadPool.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib  pool_eg.cpp -lmysqlcppconn -pthread -o pool_eg


wl2: wl2.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib wl2.cpp -lmysqlcppconn -pthread -o wl2

pooling1: pooling1.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib pooling1.cpp -lmysqlcppconn -pthread -o pooling1

pool_eg2: pool_eg2.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
semaphore.h queue.h
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	semaphore.h queue.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib pool_eg2.cpp -lmysqlcppconn -pthread -o pool_eg2

pool_eg3: pool_eg3.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h 
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib pool_eg3.cpp -lmysqlcppconn -pthread -o pool_eg3



pool_eg4: pool_eg4.cpp ../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h 
	g++ -std=c++11 -I /usr/include/boost  \
	../../../../DataStructures/CountedBtree.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/tiered-vector.h \
	-isystem /usr/local/mysql/connector-c++-/include \
	-isystem /home/jinyang/mysql-connector-c++-1.1.8-linux-ubuntu16.04-x86-64bit/include \
	-L /usr/local/lib pool_eg4.cpp -lmysqlcppconn -pthread -o pool_eg4






