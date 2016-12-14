#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>

using namespace std;

/* Input order
#nodes, #edges
from, to, distance
critical node where first is starting ending point
*/

struct visiting
{
	int node;
	bool pathed;
};

class sorting
{
public:
	sorting()
	{
		crit_nodes = vector<visiting>();
		curr_path = vector<int>();
	}
	sorting(vector<visiting> crit, vector<int> pth, int prior, double p)
	{
		crit_nodes = crit;
		curr_path = pth;
		priority = prior;
		path_len = p;
	}

	vector<visiting> crit_nodes;
	vector<int> curr_path;
	int priority;
	double path_len;


};

bool operator<(const sorting& a, const sorting& b)
{
	return a.priority > b.priority;
}


/*
INPUT:
-total_map = Matrix that Floyd's was ran on
-critical_nodes = list of nodes that must be vistied and have already been visited

OUTPUT:
-A partial lower bound from the given problem
*/
int get_lb(vector<vector<double>> total_map, vector<visiting> critical_nodes, int path_len)
{
	double lowest_bound = 0;
	
	//Find the two shortest paths leaving each critical node that has not been pathed
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		
		//Ignore pathed critical nodes
		if (critical_nodes[i].pathed == true)
			continue;

		double min1 = DBL_MAX;
		double min2 = DBL_MAX;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			if (j != i)
			{
				if (total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1] < min1)
				{
					min2 = min1;
					min1 = total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1];
				}
				else if (total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1] < min2)
					min2 = total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1];
			}
		}
		lowest_bound += min1 + min2;
	}

	return path_len + int((lowest_bound + 0.5) / 2) ;
}

//OUTPUT: Finds all children of the current node that are critical nodes
vector<int> get_children(vector<vector<double>> total_map, vector<visiting> critical_nodes, int current_node)
{
	vector<int> children;
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		if (current_node != critical_nodes[i].node && critical_nodes[i].pathed == false)
			children.push_back(critical_nodes[i].node);
	}
	
	return children;
}

int main()
{
	int num_nodes, num_edges;
	cin >> num_nodes >> num_edges;

	vector<vector<double>> total_map;

	/* 
	Creates the nxn matrix
	-main diagonal initialized to 0
	-all other spots initialized to DBl_MAX
	*/
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

	/*
	Create the maxtrix map with distances included
	-Note: The vector is zero indexed and the nodes are 1 indexed
	-Graph is non-directed so we add the distance for both direction into matrix
	*/
	for (int i = 0; i < num_edges; i++)
	{
		double start, end, dist;
		cin >> start >> end >> dist;

		//Add distances
		total_map[start - 1][end - 1] = dist;
		total_map[end - 1][start - 1] = dist;
	}

	//Run Floyd's Algorithm to find the shortest path between all critical nodes
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

	/*
	Critical_nodes is a vector of nodes that we must go to
	-We make it type 'visiting' so that we can keep track if we have gone there yet
	*/
	int num_critical_nodes;
	cout << "Get Num Critical Nodes: " << endl;
	cin >> num_critical_nodes;

	vector<visiting> critical_nodes;
	//INPUT: Get all critical nodes
	for (int i = 0; i < num_critical_nodes; i++)
	{
		visiting node;
		cin >> node.node;
		node.pathed = false;
		critical_nodes.push_back(node);
	}


	vector<int> path = {critical_nodes[0].node};
	critical_nodes[0].pathed = true;

	priority_queue<sorting> expand_next;
	expand_next.push(sorting(critical_nodes, path, 0, 0));

	//Run approximate Branch and Bound for Travelling Salesman
	while (true)
	{

		sorting curr = expand_next.top();
		expand_next.pop();
		
		//Get the children of the last traversed node
		vector<int> children = get_children(total_map, curr.crit_nodes, curr.curr_path[curr.curr_path.size() - 1]);

		//End Condition
		if (children.size() == 0)
		{
			double dist = 0;
			curr.curr_path.push_back(curr.crit_nodes[0].node);

			for (int i = 0; i < curr.curr_path.size() - 1; i++)
			{
				dist += total_map[curr.curr_path[i] - 1][curr.curr_path[i + 1] - 1];
				cout << curr.curr_path[i] << " ";
			}
			cout << curr.curr_path[curr.curr_path.size() - 1];
			cout << "\nDist: " << dist << endl;
		
			break;
		}

		for (int i = 0; i < children.size(); i++)
		{
			vector<int> curr_path = curr.curr_path;
			vector<visiting> curr_crit_nodes = curr.crit_nodes;

			curr_path.push_back(children[i]);

			int bound = get_lb(total_map, curr_crit_nodes, curr.path_len);
			int new_path = curr.path_len + int(total_map[curr_path.back() - 1][children[i] - 1]);

			for (int j = 0; j < critical_nodes.size(); j++)
			{
				if (children[i] == curr.crit_nodes[j].node)
				{
					curr_crit_nodes[j].pathed = true;
					break;
				}
			}

			expand_next.push(sorting(curr_crit_nodes, curr_path, bound, new_path));
		}		
	}
}