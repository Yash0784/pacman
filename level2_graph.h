#ifndef LEVEL2_GRAPH_H
#define LEVEL2_GRAPH_H

#define GRAPH_NODES 516
#define NO_NODE     -1
#define HEIGHT 24
#define WIDTH  55


typedef struct {
    int id;
    int x, y;           
    int nb[4];          
} GraphNode;

extern GraphNode g_nodes[GRAPH_NODES];


extern signed char dir_table[GRAPH_NODES][GRAPH_NODES];

extern int pos_to_node[HEIGHT][WIDTH];

extern const int DIR_DX[4];
extern const int DIR_DY[4];

void graph_init(const char* adjlist_path);


int node_at(int x, int y);

#endif

