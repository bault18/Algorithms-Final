#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

/* Input
#nodes, #edges
all edges
critical node where first is starting ending point


*/

struct visiting
{
	int node;
	double value;
};


//Find lowest bound of whole problem
int get_lowest_bound_start(vector<vector<double>> total_map, vector<int> critical_nodes)
{
	double lowest_bound = 0;
	//get two minimal paths from the current node and add to lowest bound
	//expand next critical node and do the same
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		vector<doubles> paths;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			//if possible path and not leading to itself
			if (total_map[critical_nodes[i] - 1][critical_nodes[j] - 1] != INFINITY && j != i)
				paths.push_back(total_map[critical_nodes[i] - 1][critical_nodes[j] - 1]);
		}

		sort(paths.begin(), paths.begin() + paths.size()); //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ maybe size() - 1
		lowest_bound += paths[0] + paths[1];
	}

	//ceiling of lowest bound divided by two
	lowest_bound /= 2;
	return int(lowest_bound + 0.5);
}

//Find lowest bound given that you came from certain nodes
double get_lowest_bound(vector<vector<double>> total_map, vector<int> critical_nodes, vector<int> path)
{
	//calculate lowest bound up until the end of the path

	//calculate lowest bound for all points that are not in the path
}

//returns vector of critical nodes visitable by current node
vector<int> get_children(vector<vector<double>> total_map, vector<int> critical_nodes, int current_node)
{
	vector<int> children;
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		//note nodes are one indexed
		//If the node in the map is visitable and not the current node
		//add to children
		if (total_map[current_node - 1][critical_nodes[i] - 1] != INFINITY && total_map[current_node - 1][critical_nodes[i] - 1] != 0)
			children.push_back(i);
	}
	
	return children;
}

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
	vector<int> critical_nodes;
	vector<int> path;
	vector<int> traversed;

	for (int i = 0; i < num_nodes; i++)
	{
		int node;
		cin >> node;
		critical_nodes.push_back(node);
	}

	//Branch and Bound for TSP
	/*
	while path hasn't been found
		if first round
			calculate lower bound
			get all children
				calculate their lower bound
				expand lowest bound
		if != first round
			should have lower bound from last round
			get all children
				calulate their lower bound
				expand lowest bound
	*/
	bool first_round = true;
	while (true)
	{
		if (first_round == true)
		{
			first_round = false;

			//calculate lowest bound
			int lowest = get_lowest_bound_start(total_map, critical_nodes);

			//Get children first node

			//calculate lowest bound assuming you went to this child

			//Keep track of which child has lowest bound and expand that one into the path
			

		}
		else
		{

		}
	}
}