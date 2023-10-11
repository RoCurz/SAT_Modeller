#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void optimal_subgraph(string file_name)
{
    ifstream graph_file(file_name + ".graph");
    int n, e, num,k;
    string line;
    // vector<vector<int>> graph;

    if (graph_file.is_open())
    {
        getline(graph_file, line);
        istringstream iss(line);
        iss >> n >> e;
        int v1, v2;
        vector<vector<int>> graph(n, vector<int>(n));
        for (int i = 0; i < e; i++)
        {
            getline(graph_file, line);
            istringstream iss(line);
            iss >> v1 >> v2;
            graph[v1 - 1][v2 - 1] = 1;
            graph[v2 - 1][v1 - 1] = 1;
        }
        graph_file.close();
        int left = 1, right = n;
        while (left<=right)
        {
            k = left + (right-left)/2;
            ofstream input_file(file_name + ".satinput");
            input_file << "p cnf " << n + (n + 1) * (k + 1) << " " << 4 * n * k + n + k + 2 + ((n) * (n - 1) / 2) - e << endl;
            // Adding Conctraints to check completeness
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (!(graph[i][j]))
                    {
                        input_file << -(i + 1) << ' ' << -(j + 1) << ' ' << 0 << endl;
                    }
                }
            }
            // Adding Constraint to get exactly k2 sized subgraph
            int jump = n;
            for (int i = 1; i <= n + 1; i++)
            {
                input_file << jump + i << ' ' << 0 << endl;
            }
            for (int i = 1; i <= k; i++)
            {
                input_file << -(jump + i * (n + 1) + 1) << ' ' << 0 << endl;
            }
            int var, temp_n, temp_k;
            for (int i = 1; i <= n * k; i++)
            {
                if (i % n == 0)
                {
                    temp_n = n;
                }
                else
                {
                    temp_n = i % n;
                }
                temp_k = (i - 1) / n + 1;
                var = jump + temp_k * (n + 1) + temp_n + 1;
                input_file << -(var) << ' ' << var - 1 << ' ' << temp_n << ' ' << 0 << endl
                           << -(var) << ' ' << var - 1 << ' ' << var - (n + 2) << ' ' << 0 << endl
                           << var << ' ' << -(var - 1) << ' ' << 0 << endl
                           << var << ' ' << -(temp_n) << ' ' << -(var - (n + 2)) << ' ' << 0 << endl;
            }
            input_file << n + (n + 1) * (k + 1) << ' ' << 0;
            input_file.close();
            string command = "./minisat " + file_name + ".satinput " + file_name + ".satoutput" + "> output.txt";
            system(command.c_str());
            ifstream output_file(file_name + ".satoutput");
            getline(output_file, line);
            if (line == "SAT")
            {
                ofstream mapper_file(file_name + ".mapping");
                mapper_file << "#1" << endl;
                getline(output_file, line);
                output_file.close();
                istringstream iss(line);
                int control = 0;
                for (int i = 0; i < n; i++)
                {
                    iss >> num;
                    if (num > 0 && !control)
                    {
                        mapper_file << num;
                        control = 1;
                    }
                    else if (num > 0)
                    {
                        mapper_file << ' ' << num;
                    }
                }
                mapper_file.close();
                left = k+1;
            }
            else
            {
                input_file.close();
                right = k-1;
            }
        }
    }
}
int main(int argc, char* argv[])
{
    optimal_subgraph(argv[1]);
    return 0;
}