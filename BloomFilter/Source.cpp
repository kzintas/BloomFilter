#include <vector>
#include <string>
#include<iostream>
#include<cmath>
#include<bitset>
#include <algorithm>
#include <numeric>
#include <set>
#include <fstream>
#include <chrono>
#include <ctime>
#include "MurmurHash3.h"
#include "MurmurHash3.cpp"



using namespace std;

typedef std::vector<bool> bit_vector;

class bloom_filter {
	int size; // Probable Number of elements in universe
	double fpr;//False positive rate
	int m; //optimal size of bloom filter
	int k; //Number of hash functions
	bit_vector bloom;
	
public: 
	bloom_filter(int n, double fpr) {
		this->size = n;
		this->fpr = fpr;
		this->m = ceil(-((n * log(fpr))/pow(log(2),2.0))); //Natural logarithm	m = −n ln p/(ln 2)2
		cout << m<<	"\n";
		this->k = ceil((m / n) * log(2));// Calculate k k = (m/n) ln 2 􃱺 2-k ≈ 0.6185 m/n
		cout << k;
		bloom.resize(m,false);
		
	}
	

	void insert(string S) {
		uint32_t* p = new uint32_t(1); //For storing Hash Vaue
		const void* str = S.c_str();//Convert string to C string to use  as a parameter for constant void

		int index;
		cout<<S.length()<<"\t"<<sizeof(str)<<"\n";
		for (int i = 0; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, sizeof(str), i+1, p); //String, String size
			index = *p % m;
			bloom[index]=true;

		}
		print();
	}

	void print() {
		for (int i = 0; i < bloom.size(); i++) {
			cout << bloom.at(i);
		}
	}

	char query(string S) {
		uint32_t* p = new uint32_t(1); //For storing Hash Vaue
		const void* str = S.c_str();//Convert string to C string to use  as a parameter for constant void

		int index;
		//cout << S.length() << "\t" << sizeof(str) << "\n";
		for (int i = 0; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, sizeof(str), i + 1, p); //String, String size
			index = *p % m;
			if(bloom[index] == false) return 'N';

		}
		return 'Y';
	}


};

int main(void) {

	int n;		// Probable Number of elements in universe
	double fpr;	//False positive rate
	cin >> n >> fpr;

	//bloom_filter bf= 
	bloom_filter bf(n, fpr);
	bf.insert("Hello");
	cout <<"\n"<< bf.query("Hello");
	//string s = "Helllo";
	//uint32_t* p = new uint32_t(1);
	//const void* str=s.c_str();

	
	//MurmurHash3_x86_32(str, sizeof(str), 2, p);
	//cout << sizeof(p);
	//cout << *p;
	//cout << *(p + 1);
	//for (int i = 0; i < sizeof(p); i++) {
		//cout << p << "\n";
	//}
	//cout << n <<"\n"<< fpr;
	
	return 0;
}