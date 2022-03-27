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
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp query.cpp -o query \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

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
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp insert.cpp -o insert \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

delete: delete.cpp ../../../DataStructures ../../../DataStructures \
	../../../DA_Bidirectional ../../../DA_Bidirectional \
	../../../DataStructures ../../../DataStructures \
	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/StandardArray.cpp ../../../DataStructures/DynamicArray.cpp \
	../../../DataStructures/LinkedList.cpp ../../../DataStructures/CountedBtree.cpp delete.cpp -o delete \
	-L/usr/include/mysql -lmysqlclient -I/usr/include/mysql