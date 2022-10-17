#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

    freopen("input.txt", "r", stdin);

    int no_of_types_resource;
    scanf("%d", &no_of_types_resource);
    int max_resource_count[no_of_types_resource];
    for (int i = 0; i < no_of_types_resource; i++){
        int temp;
        scanf("%d", &temp);
        max_resource_count[i] = temp;
    }
        
    int no_of_processes;
    scanf("%d", &no_of_processes);
    int process_requirement[no_of_processes][no_of_types_resource];
    int process_allocated[no_of_processes][no_of_types_resource];
    int done[no_of_processes];
    for (int i = 0; i < no_of_processes; i++){
        for (int j = 0; j < no_of_types_resource; j++){
            process_allocated[i][j] = 0;
        }
        done[i] = 0;
    }

    for (int i = 0; i < no_of_processes; i++){
        for (int j = 0; j < no_of_types_resource; j++){
            int temp;
            scanf("%d", &temp);
            process_requirement[i][j] = temp;
        }
    }



    char choice = 'y';
    // scanf("%c", &choice);
    if (choice == 'y' || choice == 'Y'){
        for (int i = 0; i < no_of_processes; i++){
            for (int j = 0; j < no_of_types_resource; j++){
                int temp;
                scanf("%d", &temp);
                process_allocated[i][j] = temp;
                // max_resource_count[j] -= temp;
            }
        }
    }


    /// input done at this point
    printf("Total\tAlloc\n");
    for (int i = 0; i < no_of_processes; i++){
        for (int j = 0; j< no_of_types_resource; j++){
            printf("%d ", process_requirement[i][j]);
        } printf("\t");
        for (int j = 0; j< no_of_types_resource; j++){
            printf("%d ", process_allocated[i][j]);
        } printf("\n");
    }
    printf("\n----------------\nRes Tot: ");
    for (int i = 0; i < no_of_types_resource; i++){
        printf("%d ", max_resource_count[i]);
    } printf("\n");

    for (int i = 0; i < no_of_processes; i++){
        int j = 0;
        for (; j < no_of_processes; j++){
            if (done[j]) continue;

            //checking for jth process
            int k = 0;
            for (; k < no_of_types_resource; k++){
                if (process_requirement[j][k] > max_resource_count[k])
                    break;
            }

            if (k==no_of_types_resource){
                //success!!
                printf("Process number: %d is done!!\n", j);
                done[j] = 1;
                //return captured resource!
                for (k = 0; k < no_of_types_resource; k++){
                    max_resource_count[k] += process_allocated[j][k];
                }
                break;
            }


        }
        if (j == no_of_processes){
            printf("Unsafe State, cant move forword!\n");
            return 0;
        }
    }

    return 0;
}