
all: move query reorder swap insert delete

move: move.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp move.cpp -o move


query: query.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
#	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp query.cpp -o query \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql
	g++ -Wall -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
    ../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp \
    -I/usr/include/cppconn -o query query.cpp -L/usr/lib -lmysqlcppconn

    #g++ -Wall -I/usr/include/cppconn -o eg4 eg4.cpp -L/usr/lib -lmysqlcppconn


reorder: reorder.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp reorder.cpp -o reorder

swap: swap.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp swap.cpp -o swap


insert: insert.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
#	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp insert.cpp -o insert \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql
	g++ -std=c++11 -Wall  ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
    ../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp insert.cpp \
    -I/usr/include/cppconn -o insert -L/usr/lib -lmysqlcppconn

delete: delete.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
#	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp delete.cpp -o delete \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql
	g++ -std=c++11 -Wall  ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
    ../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp delete.cpp \
    -I/usr/include/cppconn -o delete -L/usr/lib -lmysqlcppconn