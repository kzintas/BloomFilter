#include <vector>
#include <string>
#include<iostream>
#include<cmath>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include "Headers/MurmurHash3.h"



using namespace std;

typedef std::vector<bool> bit_vector;

class bloom_filter {
	uint32_t size; // Probable Number of elements in universe
	double fpr;//False positive rate
	int m; //optimal size of bloom filter
	int k; //Number of hash functions
	bit_vector bloom;
	
public: 
	int get_size(){
		return size;
	}


	double get_fpr(){
		return fpr;
	}

	bloom_filter(int n, double fpr) {
		this->size = n;
		this->fpr = fpr;
		this->m = ceil(-((n * log(fpr))/pow(log(2),2.0))); //Natural logarithm	m = −n ln p/(ln 2)2
		//cout << m<<	"\n";
		this->k = ceil((m / n) * log(2));// Calculate k k = (m/n) ln 2 􃱺 2-k ≈ 0.6185 m/n
		//cout << k;
		bloom.resize(m,false);
		
	}
	

	void insert(string S) {
		uint32_t* p = new uint32_t(1); //For storing Hash Vaue
		const void* str = S.c_str();//Convert string to C string to use  as a parameter for constant void

		int index;
		//cout<<S.length()<<"\t"<<sizeof(str)<<"\n";

		//cout<<S<<"\n";

		for (int i = 0; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, S.length(), i+1, p); //String, String size
			index = *p % m;

			//cout<<*p<<"\t"<<index<<"\t";
			bloom[index]=true;

		}

		//cout<<"\n";
		//print();
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
		//cout<<S<<"\n";
		for (int i = 0; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, S.length(), i + 1, p); //String, String size
			index = *p % m;

			//cout<<*p<<"\t"<<index<<"\t";
			if(bloom[index] == false) return 'N';


		}
		return 'Y';
	}



	void save(ofstream& of){
		of.write( reinterpret_cast<const char *> (&size), sizeof(size));

		of.write( reinterpret_cast<const char *> (&fpr), sizeof(fpr));
		of.write( reinterpret_cast<const char *> (&m), sizeof(m));
		of.write( reinterpret_cast<const char *> (&k), sizeof(k));
		
		std::copy(bloom.begin(), bloom.end(), std::ostreambuf_iterator<char>(of));
		
		
	}

    bloom_filter(ifstream& inf){
		

		//cout<<"\n size"<<sizeof(size)<<"\n";
		inf.read((char*) (&size), sizeof(size));
		
		//cout<<"\nread\t"<<size<<"\t abc\n";
		inf.read((char*) (&fpr), sizeof(fpr));
		inf.read((char*) (&m), sizeof(m));
		inf.read((char*) (&k), sizeof(k));
		//cout<<"\n"<<size<<"\t"<<fpr<<"\t"<<m<<"\t"<<k<<"\t";
		//std::istreambuf_iterator iter(inf);
		//bit_vector b;
		while(!inf.eof()){
			bloom.push_back(inf.get());
		}
		/*


		cout<<"\nbitvector\n";
		for(int i=0; i<bloom.size(); i++){
			cout<<bloom.at(i);

		}
		*/
		//std::copy(iter,begin(), iter.end(), std::back_inserter(b));

	}


};

class block_bloom_filter {
	int size; // Probable Number of elements in universe
	double fpr;//False positive rate
	int m; //optimal size of bloom filter
	int k; //Number of hash functions
	int s;//Number of bloom filters 
	bit_vector block_bloom;
	int cache_line_size ;


public:
	int get_size(){
		return size;
	}


	double get_fpr(){
		return fpr;
	}
	block_bloom_filter(int n, double fpr) {
		this->size = n;
		this->fpr = fpr;
		this->m = ceil(-((n * log(fpr)) / pow(log(2), 2.0))); //Natural logarithm	m = −n ln p/(ln 2)2
		//cout << m << "\n";
		this->k = ceil((m / n) * log(2));// Calculate k k = (m/n) ln 2 􃱺 2-k ≈ 0.6185 m/n
		//cout << k<<"\n";
		this->cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE)*8;
		
		this->s = ceil((double) m / cache_line_size); //Total number of Bloom Filters
		//cout<<s<<"s valye\n";
		block_bloom.resize(cache_line_size*s, false);
		
	}

	void insert(string S) {
		
		int block_number;
		int first_index,  last_index;
		int index;
		

		uint32_t* p = new uint32_t(1); //For storing Hash Value
		const void* str = S.c_str();//Convert string to C string to use  as a parameter for constant void
		
		MurmurHash3_x86_32(str, sizeof(str), 1, p); //String, String size//Find out block number

		
		//if(s!=0) 
		block_number = *p % s; //Which block the filter belongs to
		//else block_number=0; //Because number of blocks is zero then
		//cout<<"\n"<<*p<<"\n";
		//cout<<"\n"<<s<<" SSS\n";
		//cout<<"\n"<<block_number<<"b\n";

		first_index = block_number * cache_line_size;
		//last_index = (block_number + 1) * cache_line_size - 1;

		//cout <<"\n"<< S.length() << "\t" << sizeof(str) << "\n";

		//cout<<S<<"\n";
		//int div=min(cache_line_size,m);
		//cout<<div<<"\n";
		for (int i = 1; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, S.length(), i + 1, p); //String, String size
			//cout<<*p<<"\n";
			//cout<<"div="<<div << "\n";
			index = (*p) % cache_line_size;
			//cout<<index<<"\t";
			//if(index>m) cout<<"\n"<<index<<"\tError detected\n";
			//cout<<"\n"<<index<<"a\t\n";
			//cout<<"\n"<<first_index<<"a\t\n";
			
			//cout<<(index+first_index)<<"a\t\n";
			
			block_bloom[index+first_index] = true;

		}
		//cout<<"\n";
		//print();
	}

	void print() {
		cout<<"\nprinting\n";
		for (int i = 0; i < block_bloom.size(); i++) {
			cout << block_bloom.at(i);
		}
	}

	char query(string S) {

		int block_number;
		int first_index,  last_index;

		int index;

		uint32_t* p = new uint32_t(1); //For storing Hash Vaue
		const void* str = S.c_str();//Convert string to C string to use  as a parameter for constant void


		MurmurHash3_x86_32(str, sizeof(str), 1, p); //String, String size//Find out block number

		//if(s!=0) 
		block_number = *p % s; //Which block the filter belongs to
		//else block_number=0; //Because number of blocks is zero then

		
		first_index = block_number * cache_line_size;
		//last_index = (block_number + 1) * cache_line_size - 1;

		//int div=min(cache_line_size,m);

		//cout << S.length() << "\t" << sizeof(str) << "\n";
		//cout<<S<<"\n";
		for (int i = 1; i < k; i++) {
			//MurmurHash3_x64_128();
			MurmurHash3_x86_32(str, S.length(), i + 1, p); //String, String size
			index = *p % cache_line_size;
			//cout<<index<<"\t";
			if (block_bloom[index + first_index] == false) return 'N';

		}
		return 'Y';
	}

	void save(ofstream& of){
		of.write( reinterpret_cast<const char *> (&size), sizeof(size));

		of.write( reinterpret_cast<const char *> (&fpr), sizeof(fpr));
		of.write( reinterpret_cast<const char *> (&m), sizeof(m));
		of.write( reinterpret_cast<const char *> (&k), sizeof(k));
		of.write(reinterpret_cast<const char *> (&s), sizeof(s));
		of.write(reinterpret_cast<const char *> (&cache_line_size), sizeof(cache_line_size));
		//cout<<"\nWriting\n";
		
		std::copy(block_bloom.begin(), block_bloom.end(), std::ostreambuf_iterator<char>(of));
		//for (const auto &e: block_bloom) of<<e;
		//cout<<"\nFinished Writing\n";
		
	}
    block_bloom_filter(ifstream& inf){
		inf.read((char*) (&size), sizeof(size));
		
		//cout<<"\nread\t"<<size<<"\t abc\n";
		inf.read((char*) (&fpr), sizeof(fpr));
		inf.read((char*) (&m), sizeof(m));
		inf.read((char*) (&k), sizeof(k));
		inf.read((char*) (&s), sizeof(s));
		inf.read((char*) (&cache_line_size), sizeof(cache_line_size));
		
		//cout<<"\n"<<size<<"\t"<<fpr<<"\t"<<m<<"\t"<<k<<"\t";
		//std::istreambuf_iterator iter(inf);
		//bit_vector b;
		while(!inf.eof()){
			block_bloom.push_back(inf.get());
		}
	}




};

int main(int argc, char** argv) 
{ 

	if(argc<3){
		cout<< "Not al parameters given"<<endl;
		return 1;
	}

	// $bf build -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
	/*

	cout<<argc<<"\n";
	for(int i=0; i<argc; i++){
		cout<< argv[i]<<"\n";
	}
	*/

	std::string arg = argv[1];
	


	if(arg=="build") {
		std::string input = argv[2];
		std::string output = argv[5];

		ifstream keyfile (input);
		ofstream outfile (output, std::ios::out | std::ofstream::binary);

		int n;
		double fpr;

		n= atoi(argv[4]);
		//cout<<n<<"\n";

		fpr=atof(argv[3]);
		//cout<<fpr<<"\n";
		block_bloom_filter bf(n,fpr);
		string S;

		while (keyfile >> S)
		{
			bf.insert(S);
				// process pair (a,b)
		}
		keyfile.close();
		//bf.print();
		//cout<<"\nDOne Inserting\n";
		if(outfile.is_open()){
			bf.save(outfile);
		}
		outfile.close();
		//cout<<"Here\n";
		//ifstream inff(output, std::ios::in | std::ifstream::binary);
		//bf.load(inff);
		//inff.close();

	}

	if(arg=="query"){
		std::string input = argv[2];
		std::string query = argv[3];


		ifstream inff(input, std::ios::in | std::ifstream::binary);
		ifstream keyfile (query);

		block_bloom_filter bf(inff);
		inff.close();

		string S;

		while (keyfile >> S)
		{
			cout<<S<<":"<<bf.query(S)<<"\n";
				// process pair (a,b)
		}
		
		keyfile.close();

	}

	if(arg=="query1"){
		std::string input = argv[2];
		std::string query_all = argv[3];

		std::string query_half = argv[4];

		std::string query_none = argv[5];

		char records[3][1000];


		ifstream inff(input, std::ios::in | std::ifstream::binary);
		ifstream keyallfile (query_all);
		ifstream keyhalffile (query_half);
		ifstream keynonefile (query_none);


		ofstream outfile ("benchmark_block.txt" , std::ios_base::app | std::ios_base::out);
		outfile.setf(ios::fixed,ios::floatfield);
		


		block_bloom_filter bf(inff);
		inff.close();

		string S;

		auto start = std::chrono::system_clock::now();

		int i=0;

		while (keyallfile >> S)
		{
			records[0][i++]=bf.query(S);
			//cout<<S<<":"<<bf.query(S)<<"\n";
				// process pair (a,b)
		}
		auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

		auto start2 = std::chrono::system_clock::now();
		i=0;

		while (keyhalffile >> S)
		{
			records[1][i++]=bf.query(S);
			//cout<<S<<":"<<bf.query(S)<<"\n";
				// process pair (a,b)
		}
		auto end2 = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed_seconds2 = end2-start2;

		auto start3 = std::chrono::system_clock::now();

		i=0;

		while (keynonefile >> S)
		{
			records[2][i++]=bf.query(S);

			//cout<<S<<":"<<bf.query(S)<<"\n";
				// process pair (a,b)
		}
		auto end3 = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed_seconds3 = end3-start3;
		double empirical_fpr1=0;
		double empirical_fpr2=0;
		int count1=0;
		int count2=0;
		int count3=0;
		for(int j=500; j< 1000;j++){
			if(records[1][j]=='Y') count1++;
		}
		

		for(int j=0; j< 1000;j++){
			if(records[2][j]=='Y') count2++;
		}
		for(int j=0; j< 1000;j++){
			if(records[0][j]=='N') count3++;
		}
		empirical_fpr1= (double ) count1/500;

		empirical_fpr2= (double ) count2/1000;


		outfile<<bf.get_size()<<","<<bf.get_fpr()<<","<<elapsed_seconds.count()/1000.0<<","<< count3<<","<<elapsed_seconds2.count()/1000.0<<","<<empirical_fpr1<<","<<elapsed_seconds3.count()/1000.0<<","<<empirical_fpr2<<"\n";
		outfile.close();

	
		keyallfile.close();
		keyhalffile.close();
		keynonefile.close();

	}



}
/*

int main(void) {

	int n;		// Probable Number of elements in universe
	double fpr;	//False positive rate
	cin >> n >> fpr;

	//cout<<sysconf(_SC_LEVEL1_DCACHE_LINESIZE) <<"\n" ;

	//bloom_filter bf= 
	block_bloom_filter bf(n, fpr);
	bf.insert("Hello");
	bf.insert("Hi");
	bf.insert("Janina");
	bf.insert("Vallagena");
	cout <<"\n"<< bf.query("Dash");

	cout << "\n" << bf.query("Janina");
	cout << "\n" << bf.query("Hello");
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
*/