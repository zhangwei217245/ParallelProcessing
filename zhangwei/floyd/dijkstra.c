#include "dijkstra.h"

int chooseVertex(int *dist, int n, int *found){
		int i, tmp, leastPosition;
		int least = INFTY;

		for (i = 0; i < n; i++){
				tmp = dist[i];
				if ((!found[i]) && (tmp < least)){
						least = tmp;
						leastPosition = i;
				}
		}
		return leastPosition;
}

void dijkstra(int SOURCE, int n, int **edge, int *dist){
		int infinity = INFTY;
		int i, j, count, *found;
		found = (int*) calloc (n, sizeof(int));
		for (i = 0; i < n; i++){
				found[i]=0;
				dist[i]=edge[SOURCE][i];
		}
		found[SOURCE] = 1;
		count = 1;
		while ( count < n ){
				j = chooseVertex(dist, n, found);
				found[j] = 1;
				count++;
				for (i = 0; i < n; i++){
						if (!(found[i])){
								int distplus = (edge[j][i] == infinity)? infinity: dist[j] + edge[j][i];
								dist[i] = min(dist[i], distplus);
						}
				}
		}
}
