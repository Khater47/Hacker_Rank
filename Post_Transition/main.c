#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 6

struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;



void print_all_packages(town t) {
    int i, j;

    printf("%s:\n", t.name);
    for(i = 0; i < t.offices_count; i++){
        printf("\t%d:\n", i);
        for(j = 0; j < t.offices[i].packages_count; j++){
            printf("\t\t%s\n", t.offices[i].packages[j].id);
        }
    }
    return;
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {
	int *mark = NULL;
	int i, j = 0, package_weight, accepted_packages = 0, new_size, pos;
	post_office *source_office = &(source->offices[source_office_index]);
	post_office *target_office = &(target->offices[target_office_index]);
	package *temp = NULL;
	int target_min_weight = target_office->min_weight;
	int target_max_weight = target_office->max_weight;
	//create mark array
	mark = (int *)calloc(source_office->packages_count, sizeof(int));
	if(mark == NULL){
		printf("Memory allocation error\n");
		exit(1);
	}
	// fill mark: accepted = 1 , refused = 0;
	for(i = 0; i < source_office->packages_count; i++){
		package_weight = source_office->packages[i].weight;
		if( (package_weight >= target_min_weight) && (package_weight <= target_max_weight) ){
			mark[i] = 1;
			accepted_packages++;
		}else{
			mark[i] = 0;
		}
	}
	// reallocate memory to add accepted packages to target
	new_size = target_office->packages_count + accepted_packages;
	target_office->packages = (package *) realloc(target_office->packages, (new_size * sizeof(package)));
	if(target_office->packages == NULL){
		printf("Memory reallocation error\n");
		exit(1);
	}
	//move accepted packages to target with the same order
	for(i = 0; i < source_office->packages_count; i++){
		if(mark[i] == 1){
			//copy package of index i from source to target in pos after target_office->package_count
			pos = ++(target_office->packages_count);
			target_office->packages[pos].weight = source_office->packages[i].weight;
			target_office->packages[pos].id = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
			if(target_office->packages[pos].id == NULL){
				printf("Memory allocation error\n");
				exit(1);
			}
			strcpy(target_office->packages[pos].id, source_office->packages[i].id);
		}
	}
	//modify source packages
	new_size = source_office->packages_count - accepted_packages;
	temp = (package *)malloc(new_size * sizeof(package));
	if(temp == NULL){
		printf("Memory allocation error\n");
		exit(1);
	}
	//make a temp copy of the new source office packages queue
	for(i = 0; i < source_office->packages_count; i++){
		if(mark[i] == 0){
			temp[j].weight = source_office->packages[i].weight;
			temp[j].id = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
			if(temp[j].id == NULL){
				printf("Memory allocation error\n");
				exit(1);
			}
			strcpy(temp[j].id, source_office->packages[i].id);
			j++;
		}
	}
	//cancel old queue and make the new one
	free(source_office->packages);
	source_office->packages = (package *)malloc(new_size * sizeof(package));
	if(source_office->packages == NULL){
		printf("memory allocation error\n");
		exit(1);
	}
	for(i = 0; i < new_size; i++){
		source_office->packages[i].id = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
		if(source_office->packages[i].id == NULL){
			printf("Memory allocation error\n");
			exit(1);
		}
		strcpy(source_office->packages[i].id, temp[i].id);
		source_office->packages[i].weight = temp[i].weight;
	}
	source_office->packages_count = new_size;
	//free memory
	free(mark);
	for(i = 0; i < new_size; i++){
		free(temp[i].id);
	}
	free(temp);
	return;
}

town town_with_most_packages(town* towns, int towns_count) {
	int *packages_count = (int *)malloc(towns_count * sizeof(int));
	int i, j, max, max_index;
	
	if(packages_count == NULL){
		printf("Memory alloction error\n");
		exit(1);
	}
	for(i = 0; i < towns_count; i++){
		packages_count[i] = 0;
		for(j = 0; j < towns[i].offices_count; j++){
			packages_count[i] += towns[i].offices[j].packages_count;
		}
	}
	max_index = 0;
	max = packages_count[0];
	for(i = 0; i < towns_count; i++){
		if(packages_count[i] > max){
			max = packages_count[i];
			max_index = i;
		}
	}
	return towns[max_index];
}

town* find_town(town* towns, int towns_count, char* name) {
	int i;
	for(i = 0; i < towns_count; i++){
		if(strcmp(towns[i].name, name) == 0){
			return &towns[i];
		}
	}
	return NULL;
}

int main()
{
	int towns_count;
    FILE *fp = NULL;
    fp = fopen("/home/axe47/C C++/Hacker_Rank/Post_Transition/input.txt", "r");
    if(fp == NULL){
        printf("file read error\n");
        exit(0);
    } 
	fscanf(fp, "%d%*c", &towns_count);
	town* towns = malloc(sizeof(town)*towns_count);
	for (int i = 0; i < towns_count; i++) {
		towns[i].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
		fscanf(fp, "%s", towns[i].name);
		fscanf(fp, "%d%*c", &towns[i].offices_count);
		towns[i].offices = malloc(sizeof(post_office)*towns[i].offices_count);
		for (int j = 0; j < towns[i].offices_count; j++) {
			fscanf(fp, "%d%d%d", &towns[i].offices[j].packages_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
			towns[i].offices[j].packages = malloc(sizeof(package)*towns[i].offices[j].packages_count);
			for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
				towns[i].offices[j].packages[k].id = malloc(sizeof(char) * MAX_STRING_LENGTH);
				fscanf(fp, "%s", towns[i].offices[j].packages[k].id);
				fscanf(fp, "%d", &towns[i].offices[j].packages[k].weight);
			}
		}
	}
	int queries;
	fscanf(fp, "%d", &queries);
	char town_name[MAX_STRING_LENGTH];
	while (queries--) {
		int type;
		fscanf(fp, "%d", &type);
		switch (type) {
		case 1:
			fscanf(fp, "%s", town_name);
			town* t = find_town(towns, towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			fscanf(fp, "%s", town_name);
			town* source = find_town(towns, towns_count, town_name);
			int source_index;
			fscanf(fp, "%d", &source_index);
			fscanf(fp, "%s", town_name);
			town* target = find_town(towns, towns_count, town_name);
			int target_index;
			fscanf(fp, "%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
			break;
		}
	}
	return 0;
}