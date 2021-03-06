/*
Name:- Tanmay Borkar

The following code is use the calculate the Gomory Hu Cut Graph using the GusField pseudo code. The code makes uses of the Ford-Fulkerson algortihm to get the minimum cut between source and sink.
Input:
Number of nodes
Number of edges
Capacity of each edge 
Output:
Cut tree consisting of the nodes and their corresponding maxflow.


Reference:-
1. http://ranger.uta.edu/~weems/NOTES5311/ff.c
2. https://epubs-siam-org.ezproxy.uta.edu/doi/pdf/10.1137/0219009

*/

#include <stdio.h>

// Basic Definitions

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 3000
#define oo 1000000000


// Declarations

int n;  // number of nodes
int e;  // number of edges
short capacity[MAX_NODES][MAX_NODES]; // capacity matrix
short flow[MAX_NODES][MAX_NODES];     // flow matrix
short color[MAX_NODES]; // needed for breadth-first search               
short pred[MAX_NODES];  // array to store augmenting path
short tree[MAX_NODES][MAX_NODES];

int min(int x, int y) {
	return x<y ? x : y;  // returns minimum of x and y
}

// A Queue for Breadth-First Search

int head, tail;
short q[MAX_NODES + 2];

void enqueue(int x) {
	q[tail] = x;
	tail++;
	color[x] = GRAY;
}

int dequeue() {
	int x = q[head];
	head++;
	color[x] = BLACK;
	return x;
}

// Breadth-First Search for an augmenting path

int bfs(int start, int target) {
	int u, v, i;
	for (u = 0; u<n; u++) {
		color[u] = WHITE;
	}
	head = tail = 0;
	enqueue(start);
	pred[start] = -1;
	while (head != tail) {
		u = dequeue();
		// Search all adjacent white nodes v. If the capacity
		// from u to v in the residual network is positive,
		// enqueue v.
		for (v = 0; v<n; v++) {
			if (color[v] == WHITE && capacity[u][v] - flow[u][v]>0) {
				enqueue(v);
				pred[v] = u;
			}
		}
	}

	// If the color of the target node is black now,
	// it means that we reached it.
	return color[target] == BLACK;
}

// Ford-Fulkerson Algorithm

int max_flow(int source, int sink) {
	int i, j, u;
	// Initialize empty flow.
	int max_flow = 0;
	for (i = 0; i<n; i++) {
		for (j = 0; j<n; j++) {
			flow[i][j] = 0;
		}
	}
	// While there exists an augmenting path,
	// increment the flow along this path.
	while (bfs(source, sink)) {
		// Determine the amount by which we can increment the flow.
		int increment = oo;
		for (u = sink; pred[u] != (-1); u = pred[u]) {
			increment = min(increment, capacity[pred[u]][u] - flow[pred[u]][u]);
		}
		// Now increment the flow.
		for (u = sink; pred[u] != (-1); u = pred[u]) {
			flow[pred[u]][u] += increment;
			flow[u][pred[u]] -= increment;  // Reverse in residual
		}
		max_flow += increment;
	}
	// No augmenting path anymore. We are done.
	return max_flow;
}

// Reading the input file and the main program

void read_input_file() {
	int a, b, c, i, j;

	// read number of nodes and edges
	scanf("%d %d", &n, &e);
	// initialize empty capacity matrix 
	for (i = 0; i<n; i++) {
		for (j = 0; j<n; j++) {
			capacity[i][j] = 0;
		}
	}
	// read edge capacities
	for (i = 0; i<e; i++) {
		scanf("%d %d %d", &a, &b, &c);
		capacity[a][b] = c;
		capacity[b][a] = c;// Could have parallel edges
	}
}


//Calculate the cut graph
void mgh() {
	short i, j, s, p[MAX_NODES], t, pos, f1[MAX_NODES], minimumCut;

	for (i = 0; i < n; i++) {
		p[i] = 0;
		f1[i] = 0;
		for (j = 0; j < n; j++) {
			tree[i][j] = 0;
		}
	}



	for (s = 1; s < n; s++) {

		//Initialize sink
		t = p[s];
		
		//Calculate the minimum cut
		minimumCut = max_flow(s, t);

		f1[s] = minimumCut;

		
		/*for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				tree[i][j] = 0;
			}
		}*/

		//Mark the nodes in the supernode with its representative
		for (i = 0; i < n; i++) {
			if (i != s && p[i] == t && color[i] == BLACK) {
				p[i] = s;
			}
		}

		//Change the label
		if (color[p[t]] == BLACK) {
			p[s] = p[t];
			p[t] = s;
			f1[s] = f1[t];
			f1[t] = minimumCut;
		}
		
		//Store the final cut tree when s is the last node of the input graph.
		if(s==n-1)
			{
				for (i = 1; i <= s; i++) {
					tree[i][p[i]] = f1[i];
				}
			}

	}
	
	//Print the Cut Tree
	printf("\n");
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			if(tree[i][j]>0)
				printf("%d %d %d\n", i, j, tree[i][j]);
}

int main() {

	read_input_file();
	mgh();

	return 0;
}

