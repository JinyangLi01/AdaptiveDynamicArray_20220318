all: move query reorder swap insert delete

move: move.cpp DAbidirectional.cpp DAbidirectional.h \
	DAstatic.cpp DAstatic.h \
	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
                   	../../../DataStructures/Stree.cpp move.cpp -o move

query: query.cpp DAbidirectional.cpp DAbidirectional.h \
       DAstatic.cpp DAstatic.h \
       ../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
                   	../../../DataStructures/Stree.cpp query.cpp -o query

reorder: reorder.cpp DAbidirectional.cpp DAbidirectional.h \
         DAstatic.cpp DAstatic.h \
         ../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
                   	../../../DataStructures/Stree.cpp reorder.cpp -o reorder

swap: swap.cpp DAbidirectional.cpp DAbidirectional.h \
               	DAstatic.cpp DAstatic.h \
               	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
                   	../../../DataStructures/Stree.cpp swap.cpp -o swap


insert: insert.cpp DAbidirectional.cpp DAbidirectional.h \
                   	DAstatic.cpp DAstatic.h \
                   	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
                   	../../../DataStructures/Stree.cpp insert.cpp -o insert

delete: delete.cpp DAbidirectional.cpp DAbidirectional.h \
                   	DAstatic.cpp DAstatic.h \
                   	../../../DataStructures ../../../DataStructures
	g++ -std=c++11 ../../../DataStructures/DynamicArray.cpp ../../../DataStructures/DAnolimit.cpp \
	../../../DataStructures/Stree.cpp delete.cpp -o delete