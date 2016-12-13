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
	sorting(vector<visiting> crit, vector<int> pth, int prior)
	{
		crit_nodes = crit;
		curr_path = pth;
		priority = prior;
	}

	vector<visiting> crit_nodes;
	vector<int> curr_path;
	int priority;


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
double get_lowest_bound_start(vector<vector<double>> total_map, vector<visiting> critical_nodes)
{
	double lowest_bound = 0;
	
	//Find the two shortest paths leaving each critical node that has not been pathed
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		//Ignore pathed critical nodes
		if (critical_nodes[i].pathed == true)
			continue;

		vector<double> paths;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			//If possible to get to the critical node && the node is not going to itself
			if (total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1] != DBL_MAX && j != i)
				paths.push_back(total_map[critical_nodes[i].node - 1][critical_nodes[j].node - 1]);
		}

		sort(paths.begin(), paths.begin() + paths.size());
		lowest_bound += paths[0] + paths[1];
	}

	return lowest_bound;
}

/*
INPUT:
-total_map = Matrix that Floyd's was ran on
-critical_nodes = list of nodes that must be vistied and have already been visited
-path = the order that the nodes have been traversed

OUTPUT:
-An exact lower bound for the entire problem given that you have followed the path
*/
int get_lowest_bound(vector<vector<double>> total_map, vector<visiting> critical_nodes, vector<int> path)
{

	//Calculate the lowest bound of the path
	double lowest_bound = 0;
	for (int i = 0; i < path.size() - 1; i++)
	{
		lowest_bound += total_map[path[i] - 1][path[i + 1] - 1];
		total_map[path[i] - 1][path[i + 1] - 1] = DBL_MAX;

		double min = DBL_MAX;
		for (int j = 0; j < critical_nodes.size(); j++)
		{
			if (total_map[path[i] - 1][critical_nodes[j].node - 1] != DBL_MAX && j != i && total_map[path[i] - 1][critical_nodes[j].node - 1] < min)
				min = total_map[path[i] - 1][critical_nodes[j].node - 1];				
		}
		lowest_bound += min;
	}


	
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		if (path[path.size() - 1] == critical_nodes[i].node)
		{
			critical_nodes[i].pathed = false;
			break;
		}
	}


	//get lowest bound of points not in the map
	double rest_of_bound = get_lowest_bound_start(total_map, critical_nodes);
	
	return int((lowest_bound + rest_of_bound) / 2);
}

//OUTPUT: Finds all children of the current node that are critical nodes
vector<int> get_children(vector<vector<double>> total_map, vector<visiting> critical_nodes, int current_node)
{
	vector<int> children;
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		if (total_map[current_node - 1][critical_nodes[i].node - 1] != DBL_MAX && current_node != critical_nodes[i].node && critical_nodes[i].pathed == false)
			children.push_back(critical_nodes[i].node);
	}
	
	return children;
}

bool all_true(vector<visiting> critical_nodes)
{
	for (int i = 0; i < critical_nodes.size(); i++)
	{
		if (critical_nodes[i].pathed == false && critical_nodes[i].node != 1)
			return false;
	}
	return true;
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

	//Run approximate Branch and Bound for Travelling Salesman


	/*
	Path is a list of the nodes we have visited and what order we have visited them
	-We add the first critical node because that is our starting/ending location
	*/
	vector<int> path = { critical_nodes[0].node, critical_nodes[1].node};
	critical_nodes[0].pathed = true;
	critical_nodes[1].pathed = true;

	priority_queue<sorting> expand_next;
	expand_next.push(sorting(critical_nodes, path, 0));
	int pass = 0;
	while (true)
	{
		pass++;
		cout << "Pass: " << pass << endl;

		//This should never happen but just in case
		if (expand_next.size() == 0)
			break;


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


		
		if (curr.curr_path.back() == 24)
		{
			curr.curr_path.push_back(25);
			for (int i = 0; i < curr.crit_nodes.size(); i++)
			{
				if (curr.crit_nodes[i].node == 25)
				{
					curr.crit_nodes[i].pathed = true;
					break;
				}
			}
			expand_next.push(sorting(curr.crit_nodes, curr.curr_path, get_lowest_bound(total_map, curr.crit_nodes, curr.curr_path)));
			continue;
		}

		if (curr.curr_path.back() == 21)
		{
			curr.curr_path.push_back(23);
			for (int i = 0; i < curr.crit_nodes.size(); i++)
			{
				if (curr.crit_nodes[i].node == 23)
				{
					curr.crit_nodes[i].pathed = true;
					break;
				}
			}
			expand_next.push(sorting(curr.crit_nodes, curr.curr_path, get_lowest_bound(total_map, curr.crit_nodes, curr.curr_path)));
			continue;
		}

		for (int i = 0; i < children.size(); i++)
		{
			//node 1 must be last node visited
			if (!all_true(curr.crit_nodes) && children[i] == 1)
				continue;

			curr.curr_path.push_back(children[i]);
			int j = 0;
			for (; j < critical_nodes.size(); j++)
			{
				if (children[i] == curr.crit_nodes[j].node)
				{
					curr.crit_nodes[j].pathed = true;
					break;
				}
			}
			expand_next.push(sorting(curr.crit_nodes, curr.curr_path, get_lowest_bound(total_map, curr.crit_nodes, curr.curr_path)));

			curr.crit_nodes[j].pathed = false;
			curr.curr_path.pop_back();
		}		
	}
}