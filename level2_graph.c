#include "level2_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

GraphNode g_nodes[GRAPH_NODES];
signed char dir_table[GRAPH_NODES][GRAPH_NODES];
int   pos_to_node[HEIGHT][WIDTH];

const int DIR_DX[4] = {-1,  0, +1,  0};
const int DIR_DY[4] = { 0, +1,  0, -1};

typedef struct { int node; int dist; } HeapElem;

typedef struct {
    HeapElem* data;
    int size, cap;
} MinHeap;

static void heap_push(MinHeap* h, int node, int dist) {
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = realloc(h->data, h->cap * sizeof(HeapElem));
    }
    int i = h->size++;
    h->data[i] = (HeapElem){node, dist};
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[p].dist <= h->data[i].dist) break;
        HeapElem tmp = h->data[p]; h->data[p] = h->data[i]; h->data[i] = tmp;
        i = p;
    }
}

static HeapElem heap_pop(MinHeap* h) {
    HeapElem top = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while(1) {
        int l = 2*i+1, r = 2*i+2, s = i;
        if (l < h->size && h->data[l].dist < h->data[s].dist) s = l;
        if (r < h->size && h->data[r].dist < h->data[s].dist) s = r;
        if (s == i) break;
        HeapElem tmp = h->data[s]; h->data[s] = h->data[i]; h->data[i] = tmp;
        i = s;
    }
    return top;
}

static void dijkstra(int src, signed char first_dir[GRAPH_NODES]) {
    int dist[GRAPH_NODES];
    for (int i = 0; i < GRAPH_NODES; i++) { dist[i] = INT_MAX; first_dir[i] = -1; }
    dist[src] = 0;

    MinHeap h;
    h.cap  = 64; h.size = 0;
    h.data = malloc(h.cap * sizeof(HeapElem));
    heap_push(&h, src, 0);

    while (h.size > 0) {
        HeapElem cur = heap_pop(&h);
        int u = cur.node;
        if (cur.dist > dist[u]) continue;   

        for (int d = 0; d < 4; d++) {
            int v = g_nodes[u].nb[d];
            if (v == -1) continue;
            int nd = dist[u] + 1;
            if (nd < dist[v]) {
                dist[v] = nd;
                first_dir[v] = (u == src) ? (signed char)d : first_dir[u];
                heap_push(&h, v, nd);
            }
        }
    }
    free(h.data);
}

void graph_init(const char* adjlist_path) {

    memset(pos_to_node, -1, sizeof(pos_to_node));

    FILE* f = fopen(adjlist_path, "r");
    for (int i = 0; i < GRAPH_NODES; i++) {
        int id, n1, n2, n3, n4;
        if (fscanf(f, "%d -> %d %d %d %d", &id, &n1, &n2, &n3, &n4) != 5) break;
        g_nodes[id].id = id;
        g_nodes[id].nb[0] = n1;   /* up    */
        g_nodes[id].nb[1] = n2;   /* right */
        g_nodes[id].nb[2] = n3;   /* down  */
        g_nodes[id].nb[3] = n4;   /* left  */
        g_nodes[id].x = -1;
        g_nodes[id].y = -1;
    }
    fclose(f);

    g_nodes[0].x = 1;
    g_nodes[0].y = 1;

    int bfs_queue[GRAPH_NODES];
    int bfs_head = 0, bfs_tail = 0;
    bfs_queue[bfs_tail++] = 0;

    while (bfs_head < bfs_tail) {
        int u = bfs_queue[bfs_head++];
        int ux = g_nodes[u].x, uy = g_nodes[u].y;
        for (int d = 0; d < 4; d++) {
            int v = g_nodes[u].nb[d];
            if (v == -1 || g_nodes[v].x != -1) continue; 
            g_nodes[v].x = ux + DIR_DX[d];
            g_nodes[v].y = uy + DIR_DY[d];
            bfs_queue[bfs_tail++] = v;
        }
    }


    int changed = 1;
    while (changed) {
        changed = 0;
        for (int u = 0; u < GRAPH_NODES; u++) {
            if (g_nodes[u].x != -1) continue; 
            for (int d = 0; d < 4; d++) {
                int v = g_nodes[u].nb[d]; 
                if (v == -1 || g_nodes[v].x == -1) continue;
                g_nodes[u].x = g_nodes[v].x - DIR_DX[d];
                g_nodes[u].y = g_nodes[v].y - DIR_DY[d];
                changed = 1;
                break;
            }
        }
    }

   
    for (int id = 0; id < GRAPH_NODES; id++) {
        int nx = g_nodes[id].x, ny = g_nodes[id].y;
        if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH) continue;
        pos_to_node[nx][ny] = id;
        if (ny + 1 < WIDTH)
            pos_to_node[nx][ny + 1] = id;
    }


    for (int s = 0; s < GRAPH_NODES; s++) {
        dijkstra(s, dir_table[s]);
    }
}

int node_at(int x, int y) {
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) return NO_NODE;
    return pos_to_node[x][y];
}

