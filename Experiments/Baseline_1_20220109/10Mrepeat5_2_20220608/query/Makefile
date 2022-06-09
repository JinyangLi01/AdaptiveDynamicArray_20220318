all: query average

query: query.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/CountedBtree.cpp
	g++ -std=c++11 ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/CountedBtree.cpp query.cpp -o query

average: average.cpp
	g++ -std=c++11 average.cpp -o average
