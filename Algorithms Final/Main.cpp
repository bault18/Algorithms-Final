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
double get_lowest_bound_start(vector<vector<double>> total_map, vector<int> critical_nodes)
{
	double lowest_bound = 0;
	//get two minimal paths from the current node and add to lowest bound
	//expand next critical node and do the same
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		if (critical_nodes[i] == -1)
			continue;

		vector<double> paths;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			//if possible path and not leading to itself
			if (critical_nodes[j] == -1)
				continue;
			if (total_map[critical_nodes[i] - 1][critical_nodes[j] - 1] != DBL_MAX && j != i)
				paths.push_back(total_map[critical_nodes[i] - 1][critical_nodes[j] - 1]);
		}

		sort(paths.begin(), paths.begin() + paths.size()); //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ maybe size() - 1
		lowest_bound += paths[0] + paths[1];
	}

	//ceiling of lowest bound divided by two
	
	return lowest_bound;
}

//Find lowest bound given that you came from certain nodes
int get_lowest_bound(vector<vector<double>> total_map, vector<int> critical_nodes, vector<int> path)
{
	/*calculate lowest bound up until the end of the path
		get dist from path[i] to path[i+1]
		find the next lowest path from path[i] to any other node besides the one above
		add the two to the lowest_bound total
	*/
	double lowest_bound = 0;
	for (int i = 0; i < path.size() - 1; i++)
	{
		lowest_bound += total_map[path[i] - 1][path[i + 1] - 1];
		total_map[path[i] - 1][path[i + 1] - 1] = DBL_MAX;

		double min = DBL_MAX;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			if (total_map[path[i] - 1][critical_nodes[j] - 1] != DBL_MAX && j != i && total_map[path[i] - 1][critical_nodes[j] - 1] < min)
				min = total_map[path[i] - 1][critical_nodes[j] - 1];				
		}
		lowest_bound += min;
	}

	//calculate lowest bound for all points that are not in the path

	/* idea
		take path[i] and set all its col in total_map to DBL_MAX
		call get_lowest_bound_start to get rest of lowest bound
	*/

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Issue with paths being too short ex B -> C after A removed
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		for (int j = 0; j < path.size() - 1; j++)
		{
			if (path[j] == critical_nodes[i])
			{
				total_map[critical_nodes[i] - 1][path[j + 1] - 1] = DBL_MAX;
				critical_nodes[i] = -1;
				break;
			}
		}
	}

	//Finding rest of lower bound
	double rest_of_bound = get_lowest_bound_start(total_map, critical_nodes);

	return int((lowest_bound + rest_of_bound) / 2);
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
		if (total_map[current_node - 1][critical_nodes[i] - 1] != DBL_MAX && current_node != critical_nodes[i])
			children.push_back(critical_nodes[i]);
	}
	
	return children;
}

vector<vector<double>> floyds(vector<vector<double>> total_map)
{
	int size = total_map.size();
	for (int k = 0; k < size; k++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				total_map[i][j] = min(total_map[i][j], total_map[i][k] + total_map[k][j]);
			}
		}
	}

	return total_map;
}

int main()
{
	int num_nodes, num_edges;
	cin >> num_nodes >> num_edges;

	vector<vector<double>> total_map;

	//Generating the base map (no path values included)
	for (int i = 0; i < num_nodes; i++)
	{
		vector <double> temp;
		for (int j = 0; j < num_nodes; j++)
		{
			if (i == j)
				temp.push_back(0);
			else
				temp.push_back(DBL_MAX);
		}
		total_map.push_back(temp);
	}

	//Addint distanced to map
	for (int i = 0; i < num_edges; i++)
	{
		double start, end, dist;
		cin >> start >> end >> dist;

		//nodes are 1 indexed and vectors are 0 indexed
		total_map[start - 1][end - 1] = dist;
		total_map[end - 1][start - 1] = dist;
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
			path.push_back(critical_nodes[0]);

			//calculate lowest bound ??????????????????????????
			double low = get_lowest_bound_start(total_map, critical_nodes);
			low /= 2;
			int lowest = int(low + 0.5);

			//Get children first node
			vector<int> children = get_children(total_map, critical_nodes, critical_nodes[0]);

			int lowest_child_bound = INFINITY;
			int lowest_child;
			//for each child
				//calculate lowest bound assuming you went to this child
				//keep track of which child has lowest bound
			for (int i = 0; i < children.size(); i++)
			{
				path.push_back(children[i]);
				int bound = get_lowest_bound(total_map, critical_nodes, path);
				if (lowest_child_bound > bound)
				{
					lowest_child_bound = bound;
					lowest_child = children[i];
				}
			}

			//assign lowest bound child to new current node
			path.push_back(lowest_child);
		}
		else
		{
			//end condition
			if (path[0] == path[path.size() - 1])
				break;

			//Get children first node
			vector<int> children = get_children(total_map, critical_nodes, critical_nodes[0]);

			int lowest_child_bound = INFINITY;
			int lowest_child;
			//for each child
			//calculate lowest bound assuming you went to this child
			//keep track of which child has lowest bound
			for (int i = 0; i < children.size(); i++)
			{
				path.push_back(children[i]);
				int bound = get_lowest_bound(total_map, critical_nodes, path);
				if (lowest_child_bound > bound)
				{
					lowest_child_bound = bound;
					lowest_child = children[i];
				}
			}
		}
	}

	for (int i = 0; i < path.size(); i++)
		cout << path[i] << " ";
}