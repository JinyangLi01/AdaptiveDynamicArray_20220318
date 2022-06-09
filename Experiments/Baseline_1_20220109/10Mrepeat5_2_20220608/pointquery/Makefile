all: pointquery average


pointquery: pointquery.cpp ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/DynamicArray.cpp \
                                   	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/CountedBtree.cpp
	g++ -std=c++11 ../../../../DataStructures/StandardArray.cpp ../../../../DataStructures/DynamicArray.cpp \
	../../../../DataStructures/LinkedList.cpp ../../../../DataStructures/CountedBtree.cpp pointquery.cpp -o pointquery


average: average.cpp
	g++ -std=c++11 average.cpp -o average
