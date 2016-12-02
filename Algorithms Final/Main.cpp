#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

/* Input
#nodes, #edges
all edges
critical node where first is starting ending point


*/

int main()
{
	int num_nodes, num_edges;
	cin >> num_nodes >> num_edges;

	vector<vector<double>> total_map;

	for (int i = 0; i < num_nodes; i++)
	{
		vector <double> temp;
		for (int j = 0; j < num_nodes; j++)
		{
			if (i == j)
				temp.push_back(0);
			else
				temp.push_back(INFINITY); //IS THIS RIGHT????~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		}
		total_map.push_back(temp);
	}

	for (int i = 0; i < num_edges; i++)
	{
		double start, end, dist;
		cin >> start >> end >> dist;

		//nodes are 1 indexed and vectors are 0 indexed
		total_map[start - 1][end - 1] = dist;
	}

	//Run Floyds
	for (int k = 0; k < num_nodes; k++)
	{
		for (int i = 0; i < num_nodes; i++)
		{
			for (int j = 0; j < num_nodes; j++)
			{
				total_map[i][j] = min(total_map[i][j], total_map[i][k] + total_map[k][j]);
			}
		}
	}
	
	//Get critical nodes
	deque<int> critical_nodes;
	for (int i = 0; i < num_nodes; i++)
	{
		int node;
		cin >> node;
		critical_nodes.push_back(node);
	}

	//Branch and Bound for TSP

}