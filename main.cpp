#include<iostream>
#include"vec.h"
#include"polymer.h"
#include<set>
using namespace std;



int main() {

	/*Room r = Room(100, 100, 100);
	r.inputECC(90*90, 90);*/
	// A BIG  BUG
	Room r = Room(32,32,32,1.0,1.0,0);
	//r.input_stop_chain2();
	r.inputECC(30*30, 30); 
	for(int i=0;i<10;i++) r.deletechain(i);
	//r.save();
	//
	double  a = r.cal_Ec() + r.cal_Ep(); 
	cout << r.cal_Ec() << ',' << r.cal_Ep() << endl;
	cout << a<< endl;
	
	///*cout << ".........................................." << endl;
	//for (int i = 0; i < r.polymer_list.size(); i++)
	//	cout << r.polymer_list[i];
	//cout << ".........................................." << endl;
	//*/
	try{r.movie(50000, 50, 10); }

	catch(...){
		cout << "movie error";
		throw;
	}
	//cout << ".........................................."<<endl;
	//for (int i = 0; i < r.polymer_list.size(); i++)
	//	cout << r.polymer_list[i];
	//cout << ".........................................." << endl;
	//double b = r.cal_Ec() + r.cal_Ep();
	//cout << r.cal_Ec()<<','<<r.cal_Ep()<< endl;
	//cout << b<<endl;
	//double c = b - a;
	//cout << a<< endl;
	//cout << c << endl;
	return 0;
}

