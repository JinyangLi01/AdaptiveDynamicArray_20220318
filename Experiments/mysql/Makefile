all:  eg2 eg3 eg4

eg1: eg1.cpp
	g++ -o eg1 eg1.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql


eg2: eg2.cpp
	g++ -std=c++11 -o eg2 eg2.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

eg3: eg3.cpp
	g++ -std=c++11 eg3.cpp -o eg3 -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

eg4: eg4.cpp
	#g++ -o eg4 eg4.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql
	g++ -Wall -I/usr/include/cppconn -o eg4 eg4.cpp -L/usr/lib -lmysqlcppconn
