make: 
	g++ -o interface interface.cc -lleveldb -lpthread -I../include -L../build -std=c++11
	g++ -o case case.cc -lleveldb -lpthread -I../include -L../build -std=c++11

clear:
	rm -f interface case *.txt *.value
	rm -rf /tmp/testdb