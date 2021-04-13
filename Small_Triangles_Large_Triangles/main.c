#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct triangle
{
	int a;
	int b;
	int c;
};

typedef struct triangle triangle;
void sort_by_area(triangle* tr, int n) {
	/**
	* Sort an array a of the length n
	*/
}

int main()
{
	int n;
	FILE *fp = NULL;
	fp = fopen("/home/axe47/C C++/Hacker_Rank/Small_Triangles_Large_Triangles", "r");
	if(fp == NULL){
		printf("File open error\n");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%d%*c", &n);
	triangle *tr = malloc(n * sizeof(triangle));
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d%d%d%*c", &tr[i].a, &tr[i].b, &tr[i].c);
	}
	sort_by_area(tr, n);
	for (int i = 0; i < n; i++) {
		printf("%d %d %d\n", tr[i].a, tr[i].b, tr[i].c);
	}
	fclose(fp);
	return 0;
}