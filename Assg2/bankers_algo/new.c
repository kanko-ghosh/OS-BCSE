#include<stdio.h>
#include<stdlib.h>
int no_of_types_resource;
int no_of_processes;
int no_of_requests;

int * max_resource_count;       //  no_of_types_resource
int ** process_requirement;     //  no_of_process * no_of_types_resource
int ** orig;                    //  no_of_process * no_of_types_resource
int ** process_requests;        //  process_requests * (no_of_types_resource+1)
int * permutate;                //  no_of_requests
int * freq;                     //  no_of_processes

int cnt = 0;

void swap(int a, int b){
    int temp = permutate[a];
    permutate[a] = permutate[b];
    permutate[b] = temp;
}

void allocate (int loc){
    int pid = process_requests[loc][0];

    freq[pid] --;

    for (int i = 1; i<=no_of_types_resource; i++){
        process_requirement[pid][i-1] -= process_requests[loc][i];
        max_resource_count[i-1] -= process_requests[loc][i];
    }

    if (freq[pid] == 0){
        for (int i = 0; i < no_of_types_resource; i++){
            max_resource_count[i] += orig[pid][i];
        }
    } 
}

void deallocate (int loc){
    int pid = process_requests[loc][0];

    freq[pid] ++;
    
    for (int i = 1; i<=no_of_types_resource; i++){
        process_requirement[pid][i-1] += process_requests[loc][i];
        max_resource_count[i-1] += process_requests[loc][i];
    }

    if (freq[pid] == 1){
        for (int i = 0; i < no_of_types_resource; i++){
            max_resource_count[i] -= orig[pid][i];
        }
    }
}

void solve(int index){
    if (index == no_of_requests){
        // printf("lol");
        // getchar();
        for (int i = 0; i < no_of_requests; i++){
            printf("%d ", permutate[i]+1);
        } printf("\n");
        cnt++;
        return;
    }

    /**
     * when to return??
     * 1. at least one contains more!!
     * 2. next to be allocated contains more!!
    */

    //1
    // not required??
    int j = 0;
    for (; j < no_of_processes; j++){
        int k = 0;
        for (; k < no_of_types_resource; k++){
            if (process_requirement[j][k] > max_resource_count[k]) break; //break when fails
        }
        if (k == no_of_types_resource) break;
    }
    if (j == no_of_processes) return;


    for (int i = index; i < no_of_requests; i++){ // idx ..... i
        // printf("Index = %d, i = %d\tMax Resource: ", index, i);
        // for (int j = 0; j < no_of_types_resource; j++){
        //     printf("%d ", max_resource_count[j]);
        // } printf("\tNew Request: ");
        // for (int j = 0; j <= no_of_types_resource; j++){
        //     printf("%d ", process_requests[i][j]);
        // } printf("\tPermutation: ");
        // for (int j = 0; j < no_of_requests; j++){
        //     printf("%d ", permutate[j]);
        // } printf("\tProcesses: ");
        // for (int j = 0; j < no_of_processes; j++){
        //     for (int k = 0; k < no_of_types_resource; k++){
        //         printf("%d", process_requirement[j][k]);
        //     } printf("  ");
        // } printf("\n");
        swap(index, i);
         
        int k;
        for (k = 0; k < no_of_types_resource; k++){
            if (process_requests[permutate[index]][k+1] > max_resource_count[k] )break;
        } 

        if (k == no_of_types_resource) {
            int t = permutate[index];
            allocate(t);
            solve (index+1);
            deallocate(t);
        }
        swap(index, i);
    }
}

int main(){
    FILE * fpreq = fopen("prerequisite.txt", "r");
    FILE * falloc = fopen("allocation.txt", "r");
    freopen("output.txt", "w", stdout);
    fscanf(fpreq, "%d", &no_of_types_resource);
    max_resource_count = (int *)malloc (sizeof(int) * no_of_types_resource);
    for (int i = 0; i < no_of_types_resource; i++){
        int temp;
        fscanf(fpreq, "%d", max_resource_count+i);
    }

    fscanf(fpreq, "%d", &no_of_processes);
    freq = (int *) malloc (sizeof(int) * no_of_processes);
    process_requirement = (int **)malloc(sizeof(int *) * no_of_processes);
    orig = (int **)malloc(sizeof(int *) * no_of_processes);
    for (int i = 0; i < no_of_processes; i++){
        freq[i] = 0;
        process_requirement[i] = (int *)malloc(sizeof(int) * no_of_types_resource);
        orig[i] = (int *)malloc(sizeof(int) * no_of_types_resource);
        for (int j = 0; j < no_of_types_resource; j++){
            int temp;
            fscanf(fpreq, "%d", &temp);
            process_requirement[i][j] = temp;
            orig[i][j] = temp;
        }
    }


    fscanf(falloc, "%d", &no_of_requests);
    process_requests = (int **)malloc(sizeof(int*)*no_of_requests);
    for (int i = 0; i < no_of_requests; i++){
        process_requests[i] = (int *)malloc(sizeof(int)*(no_of_types_resource+1));
        for (int j = 0; j < (no_of_types_resource+1); j++){
            int temp;
            fscanf(falloc, "%d", &temp);
            process_requests[i][j] = temp;
            if (j == 0){
                freq[temp] ++;
            }        
        }
    }

    permutate = (int *)malloc(sizeof(int)*no_of_requests);
    for (int i = 0; i < no_of_requests; i++){
        permutate[i] = i;
    }
    
    fclose(fpreq);
    fclose(falloc);

    printf("Valid output sequences are listed below: \n");
    printf("-----------------------------------------\n");

    solve (0);
    printf("Total number of solutions: %d\n\n", cnt);

    return 0;
}