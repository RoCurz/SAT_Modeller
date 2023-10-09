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
        outputfile << 2*n << " " << 2*n << endl;
        // Adding constraint to restrict vertex to lie in only one subgraph
        for (int i = 0; i < n; i++)
        {
            outputfile << i+1 << ' ' << n+i+1 << ' ' << 0 << endl
                       << -(i+1) << ' ' << -(n+i+1) << ' ' << 0 << endl; // remove endl while submit
        }
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
        for (int i = 0; i < n; i++){
            iss >> num;
            if (num>0){
                output_file << num << ' ';
            }
        }
        output_file << endl << "#2" << endl;
        for (int i = 0; i < n; i++)
        {
            iss >> num;
            if (num > 0){
                output_file << num << ' ';
            }
        }
        output_file << endl;   
    }
    else{
        output_file << 0;
    }
}

int main(void)
{
    string a = "test";
    SAT_modeler(a);
    return 0;
}
