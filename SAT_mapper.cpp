#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void SAT_mapper(string file_name)
{
    ifstream input_file(file_name + ".satoutput");
    ifstream graph_file(file_name + ".graph");
    ofstream output_file(file_name + ".mapping");
    string line;
    int n, num;
    getline(graph_file, line);
    istringstream iss(line);
    iss >> n;
    graph_file.close();
    getline(input_file, line);
    if (line == "SAT")
    {
        output_file << "#1" << endl;
        getline(input_file, line);
        istringstream iss(line);
        int k = 0;
        for (int i = 0; i < n; i++)
        {
            iss >> num;
            if (num > 0 && !k)
            {
                output_file << num;
                k = 1;
            }
            else if (num > 0)
            {
                output_file << ' ' << num;
            }
        }
        k = 0;
        output_file << endl
                    << "#2" << endl;
        for (int i = 0; i < n; i++)
        {
            iss >> num;
            if (num > 0 && !k)
            {
                output_file << num - n;
                k = 1;
            }
            else if (num > 0)
            {
                output_file << ' ' << num - n;
            }
        }
    }
    else
    {
        output_file << 0;
    }
    output_file.close();
}
int main(int argc, char* argv[])
{
    SAT_mapper(argv[1]);
    return 0;
}