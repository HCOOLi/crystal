crystal.so:polymer.o crystal.o 
	g++ -O3 -shared polymer.o crystal.o -o crystal.so -lpython3.6m -lboost_python-py36
polymer.o:
	g++ -O3 -c -fPIC -I/usr/include/python3.6m  polymer.cpp -o polymer.o 

crystal.o: polymer.o
	
	g++ -O3 -c -fPIC -I/usr/include/python3.6m  crystal.cpp -o crystal.o 

clean: 
	rm -rf polymer.o crystal.o  


