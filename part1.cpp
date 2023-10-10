#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
using namespace std;

void SAT_modeler(string& file_name){
    ifstream inputfile(file_name+".graph");
    ofstream outputfile(file_name+".satinput");
    int n,e,k1,k2;
    // vector<vector<int>> graph;

    if (inputfile.is_open() && outputfile.is_open()){
        string line;
        getline(inputfile, line);
        istringstream iss(line);
        iss >> n >> e >> k1 >> k2;
        int v1,v2;
        vector<vector<int>> graph(n,vector<int>(n));
        for (int i = 0; i < e; i++){
            getline(inputfile, line);
            istringstream iss(line);
            iss >> v1 >> v2;
            graph[v1-1][v2-1] = 1;
            graph[v2-1][v1-1] = 1;
        }
        inputfile.close();
        outputfile << "p cnf " << n*2+(n+1)*(k1+k2+2) << " " << 4*n*(k1+k2)+3*n+k1+k2+4+2*(((n)*(n-1)/2)-e) << endl;
        // Adding constraint to restrict vertex to lie in only one subgraph
        for (int i = 0; i < n; i++)
        {
            outputfile << -(i+1) << ' ' << -(n+i+1) << ' ' << 0 << endl; // remove endl while submit
        }
        // Adding Conctraints to check completeness
        for (int i = 0; i < n; i++)
        {
            for (int j = i+1; j < n; j++)
            {
                if (!(graph[i][j]))
                {
                    outputfile << -(i+1) << ' ' << -(j+1) << ' ' << 0 << endl // remove endl while submit
                               << -(n+i+1) << ' ' << -(n+j+1) << ' ' << 0 << endl; // remove endl while submit
                } 
            }   
        }
        // Adding Constraint to get exactly k1 sized subgraph
        for (int i = 1; i <= n+1; i++)
        {
            outputfile << 2*n+i << ' ' << 0 << endl; // remove endl while submit
        }
        for (int i = 1; i <= k1; i++)
        {
            outputfile << -(2*n+i*(n+1)+1) << ' ' << 0 << endl; // remove endl while submit
        }
        int var, temp_n, temp_k;
        for (int i = 1; i <= n*k1; i++)
        {
            if (i%n==0){
                temp_n = n;
            }
            else{
                temp_n = i%n;
            }
            temp_k = (i-1)/n+1;
            var = 2*n + temp_k*(n+1) + temp_n + 1;
            outputfile << -(var) << ' ' << var-1 << ' ' << temp_n << ' ' << 0 << endl
                       << -(var) << ' ' << var-1 << ' ' << var-(n+2) << ' ' << 0 << endl
                       <<   var  << ' ' << -(var-1) << ' ' << 0 << endl
                       <<   var  << ' ' << -(temp_n) << ' ' << -(var-(n+2)) << ' ' << 0 << endl;
        }
        outputfile << 2*n+(n+1)*(k1+1) << ' ' << 0 << endl;
        
        // Adding Constraint to get exactly k2 sized subgraph
        int jump = 2*n+(n+1)*(k1+1);
        for (int i = 1; i <= n+1; i++)
        {
            outputfile << jump+i << ' ' << 0 << endl; // remove endl while submit
        }
        for (int i = 1; i <= k2; i++)
        {
            outputfile << -(jump+i*(n+1)+1) << ' ' << 0 << endl; // remove endl while submit
        }
        for (int i = 1; i <= n*k2; i++)
        {
            if (i%n==0){
                temp_n = n;
            }
            else{
                temp_n = i%n;
            }
            temp_k = (i-1)/n+1;
            var = jump + temp_k*(n+1) + temp_n + 1;
            outputfile << -(var) << ' ' << var-1 << ' ' << n+temp_n << ' ' << 0 << endl
                       << -(var) << ' ' << var-1 << ' ' << var-(n+2) << ' ' << 0 << endl
                       <<   var  << ' ' << -(var-1) << ' ' << 0 << endl
                       <<   var  << ' ' << -(n+temp_n) << ' ' << -(var-(n+2)) << ' ' << 0 << endl;
        }
        outputfile << n*2+(n+1)*(k1+k2+2) << ' ' << 0;
        outputfile.close();
    }  
}

void SAT_mapper(string& file_name){
    ifstream input_file(file_name+".satoutput");
    ifstream graph_file(file_name+".graph");
    ofstream output_file(file_name+".mapping");
    string line;
    int n, num;
    getline(graph_file, line);
    istringstream iss(line);
    iss >> n;
    graph_file.close();
    getline(input_file, line);
    if (line == "SAT"){   
        output_file << "#1" << endl;
        getline(input_file, line);
        istringstream iss(line);
        int k = 0;
        for (int i = 0; i < n; i++){
            iss >> num;
            if (num>0 && !k){
                output_file << num;
                k = 1;
            }
            else if (num>0)
            {
                output_file << ' ' << num;
            } 
        }
        k = 0;
        output_file << endl << "#2" << endl;
        for (int i = 0; i < n; i++)
        {
            iss >> num;
            if (num>0 && !k){
                output_file << num-n;
                k = 1;
            }
            else if (num>0)
            {
                output_file << ' ' << num-n;
            }
        }   
    }
    else{
        output_file << 0;
    }
    output_file.close();
}

int main(void)
{
    string a = "test";
    SAT_mapper(a);
    return 0;
}
