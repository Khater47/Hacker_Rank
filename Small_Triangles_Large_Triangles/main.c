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

#define is_null(x) ((x == NULL) ? (1) : (0))

typedef struct
{
	int tri_index;
	double p, s;
} info;

void swap_info(info *tri_info, int i, int j)
{
	info temp;
	temp.p = tri_info[i].p;
	temp.s = tri_info[i].s;
	temp.tri_index = tri_info[i].tri_index;

	tri_info[i].p = tri_info[j].p;
	tri_info[i].s = tri_info[j].s;
	tri_info[i].tri_index = tri_info[j].tri_index;

	tri_info[j].p = temp.p;
	tri_info[j].s = temp.s;
	tri_info[j].tri_index = temp.tri_index;
	return;
}

void sort_by_area(triangle *tr, int n)
{
	/**
	* Sort an array a of the length n
	*/
	int i, j;
	info *tri_info;
	triangle *copy = NULL;
	tri_info = (info *)calloc(n, sizeof(info));
	if (is_null(tri_info))
	{
		printf("Memory allocation error\n");
		exit(1);
	}
	for (i = 0; i < n; i++)
	{
		tri_info[i].p = (double)(tr[i].a + tr[i].b + tr[i].c) / 2;
		tri_info[i].s = sqrt((tri_info[i].p * (tri_info[i].p - tr[i].a) * (tri_info[i].p - tr[i].b) * (tri_info[i].p - tr[i].c)));
		tri_info[i].tri_index = i;
	}
	//sorting info based on s
	for (i = n - 1; i >= 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (tri_info[j].s > tri_info[j + 1].s)
			{
				swap_info(tri_info, j, j + 1);
			}
		}
	}

	//make a copy of original order
	copy = (triangle *)calloc(n, sizeof(triangle));
	if(copy == NULL){
		printf("Memory allocation error\n");
		exit(1);
	}
	for(i = 0; i < n; i++){
		copy[i].a = tr[i].a;
		copy[i].b = tr[i].b;
		copy[i].c = tr[i].c;
	}
	//sort original tr based on info
	for (i = 0; i < n; i++)
	{
		tr[i].a = copy[tri_info[i].tri_index].a;
		tr[i].b = copy[tri_info[i].tri_index].b;
		tr[i].c = copy[tri_info[i].tri_index].c;
	}
	free(copy);
	free(tri_info);
	return;
}

int main()
{
	int n;
	FILE *fp = NULL;
	fp = fopen("/home/axe47/C C++/Hacker_Rank/Small_Triangles_Large_Triangles/input.txt", "r");
	if (fp == NULL)
	{
		printf("File open error\n");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%d%*c", &n);
	triangle *tr = malloc(n * sizeof(triangle));
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d%d%d%*c", &tr[i].a, &tr[i].b, &tr[i].c);
	}
	sort_by_area(tr, n);
	for (int i = 0; i < n; i++)
	{
		printf("%d %d %d\n", tr[i].a, tr[i].b, tr[i].c);
	}
	fclose(fp);
	return 0;
}