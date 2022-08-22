#include "Hungarian.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono> 

// used this function for memory usage
void process_mem_usage(double& vm_usage, double& resident_set)   
{

   vm_usage     = 0.0;
   resident_set = 0.0;

   std::ifstream stat_stream("/proc/self/stat",std::ios_base::in);

   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt>> utime >> stime >> cutime >> cstime >> priority >> nice>> O >> itrealvalue >> starttime >> vsize >> rss; 

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

//this main will be used to test our matrix
int main(void)
{
    //"-std=c++11" for this initialization of vector.
    
	vector<vector<double>> costMatrix;  // declare vector of vectors
	
    std::string tempstr;
    int tempint;
    char delimiter;
    
    std::ifstream ifs("5x5mat.txt");
    
    // this is where I read my vector matrix line by line.
    //read line by line from a file into a string
    while (std::getline(ifs, tempstr)) {    
        std::istringstream iss(tempstr);
        // initialize the stringstream with that string
        std::vector<double> tempv;
        // extract the numbers from a stringstream
        while (iss >> tempint) {  
            tempv.push_back(tempint);  // push it onto our temporary vector
            iss >> delimiter;    //  read the , delimiter
        }
        //push the vector onto vector of vectors
        costMatrix.push_back(tempv);  
    }
    
	HungarianAlgorithm HungAlgo;
	vector<int> assignment;
    
    using namespace std::chrono;  //thanks to chrono library for time

    auto start = high_resolution_clock::now();   // start the time in front of the function

	double cost = HungAlgo.Solve(costMatrix, assignment);
	
	auto stop = high_resolution_clock::now();     // until function's end
	
	cout << "Assignment problem result: " << cost <<endl;  // print the optimal result of the assignment problem
	
	double vm, rss;
    process_mem_usage(vm, rss); //call the function we got above for memory usage.
    cout << "Virtual Memory: " << vm << " , Resident set size:" << rss << endl;
	
    // convert nanoseconds to seconds I print in seconds
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();  
    time_taken *= 1e-9;  
    cout << "Time " << fixed << time_taken << " seconds" << endl;

	ofstream txt;
    txt.open ("outputforcpp.txt");  // transfer the information we want to our output file.
    txt << "Assignment problem result: " << cost << endl;
    txt << "Virtual Memory: " << vm << " , Resident set size: " << rss << endl;
    txt <<  "Time " << fixed << time_taken << " seconds" << endl;

	return 0;
}