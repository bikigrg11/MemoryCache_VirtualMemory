#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "functions.h"


// Structs and Typedef


// Global variables :
int thread_created= 0;
pthread_t thread_inc[4];
//int allocIndex[4];


int main(){

    char input[255];
    char input_func[255];
    //int virtual_array[4];
    first_level *thread_num[4];
    second_level *ptable_num[4];
    int empty_place=0;
 
    // initialize the allocation counter
    for(int i=0;i<4;i++){
        allocIndex[i] = 0;
        cache_arr[i] =  malloc(sizeof(mem_cache));  // allocation for cache
        cache_arr[i] -> p_addr = -1;
    }   
   
    //printf("Please enter your input:\n");
    printf("ENTER - create|kill X|list|mem X|allocate X|read X Y|write X Y Z|exit|:");
    scanf("%s",input);
    //printf("Input Values: %s\n",input);
    sscanf(input,"%s",input_func);

    while(strcmp(input_func,"exit")){
        
            int fd;
            // FIFO file path
            char * myfifo = "/tmp/myfifo";
            mkfifo(myfifo, 0666);
            char array_input[40];


        if(!strcmp(input_func,"create")){
            // CREATE
            if(thread_created < 4){
                pthread_create(&thread_inc[thread_created],NULL,&run_method,NULL);
                thread_num[thread_created] = malloc(sizeof(first_level));
                ptable_num[thread_created] = malloc(sizeof(second_level));
                thread_num[thread_created]-> use_flag = 1;
                thread_num[thread_created]-> ptr_thread = thread_inc[thread_created];
                //ptable_thread1->virtual_address[0] = 0;
                //ptable_thread1->valid_bit[0] = 0;
                printf("Created Process : New thread created.\n\n");
                thread_created ++;
            }
            else if(empty_place == 1){
                for(int i=0;i<4;i++){
                    if(thread_num[i]-> use_flag == 0){
                        pthread_create(&thread_inc[i],NULL,&run_method,NULL);
                        thread_num[i] = malloc(sizeof(first_level));
                        ptable_num[i] = malloc(sizeof(second_level));
                        thread_num[i]-> use_flag = 1;
                        printf("Created Process : New thread created.\n\n");
                        //empty_place = 0;
                        break;
                    }
                }
                for(int i=0;i<4;i++){
                    if(thread_num[i]-> use_flag == 0){
                        empty_place = 1;
                        break;
                    }else{
                        empty_place = 0;
                    }
                }
                //break;
            }else{
                if(thread_created >= 4){
                    printf("Cannot Create more than 4 threads\n");
                }else{
                    pthread_create(&thread_inc[thread_created],NULL,&run_method,NULL);
                    thread_num[thread_created] = malloc(sizeof(first_level));
                    ptable_num[thread_created] = malloc(sizeof(second_level));
                    thread_num[thread_created]-> use_flag = 1;

                    //ptable_thread1->virtual_address[0] = 0;
                    //ptable_thread1->valid_bit[0] = 0;
                    printf("Created Process : New thread created.\n\n");
                    thread_created ++;
                }
            }
        }else if(!strcmp(input_func,"kill")){
            // KILL X
            // Stilll need to clear the memory so that when new one is declred no error
            unsigned long int num=0;
            scanf("%lu",&num);
            //pthread_cancel((pthread_t) num);
            //void *ret;

            for (int i=0;i<4;i++){
                if((int)thread_inc[i] == (int)num){
                    //printf("%lu\n",num);
                    //printf("%lu\n",(unsigned long)thread_num[i]-> ptr_thread);
                    //pthread_cleanup_push(thread_num[i]-> ptr_thread,);
                    //flag_thread = 1;
                    //pthread_cancel(thread_num[i]-> ptr_thread);
                    //pthread_kill(thread_inc[i],0);  
                    //pthread_join(thread_num[i]-> ptr_thread,0);

                    thread_num[i]->use_flag = 0;    // If you do this then the first level is deleted and process is killed
                    thread_inc[i] = 0;
                    free(thread_num[i]);
                    free(ptable_num[i]);
                    empty_place = 1;
                    //thread_created--;
                }                
            }

        }else if(!strcmp(input_func,"list")){
            // LIST 
            printf("LIST OF THREADs:\n");
            printf("____________________________\n");
            for (int i=0;i<4;i++){
                printf("Thread ID's : %d\n",(int)thread_inc[i]);
            }
            printf("____________________________\n\n");
        }else if(!strcmp(input_func,"mem")){
            // MEM X
            int valueX;
            int saveIndex= 0;
            scanf("%d",&valueX);
            int flag_mem = 0;

            for(int i =0;i<4;i++){
                if(valueX == (int)thread_inc[i]){
                    saveIndex = i;
                    flag_mem = 1;
                }
            }
            if(flag_mem == 0){
                printf("PROCESS ID %d DOESN'T EXIST, TRY AGAIN. \n\n",valueX);
            }else{
                printf("\nVIRTUAL ADRESSES:\n");
                printf("____________________________\n");
                int n=0;
                while((ptable_num[saveIndex] -> virtual_flag[n]) != 0 ){
                    if((ptable_num[saveIndex] -> virtual_address[n] ) == 0){
                        printf("0 \t -> \t");
                        printBits(sizeof(ptable_num[saveIndex] -> virtual_address[n]),&ptable_num[saveIndex] -> virtual_address[n]);
                        printf("\n");
                    }else{
                        printf("%d -> \t",ptable_num[saveIndex] -> virtual_address[n]);
                        printBits(sizeof(ptable_num[saveIndex] -> virtual_address[n]),&ptable_num[saveIndex] -> virtual_address[n]);
                        printf("\n");
                    }
                    n++;
                }
                 printf("___________________________\n\n");
            }
        }else if(!strcmp(input_func,"allocate")){
            // ALLOCATE X
            int valueX;
            int saveIndex= 0;
            scanf("%d",&valueX);
            for(int i =0;i<4;i++){
                if(valueX == (int)thread_inc[i]){
                    saveIndex = i;
                }
            }
            int virtual_add = cse320_malloc(saveIndex);
            allocIndex[saveIndex]+=4; // Means allocated 4 bytes SEE later
            int n=0;
            while((ptable_num[saveIndex] -> virtual_flag[n]) != 0 ){
                n++;
            }
            ptable_num[saveIndex] -> virtual_address[n] = virtual_add;
            ptable_num[saveIndex] -> virtual_flag[n] = 1;
        }else if(!strcmp(input_func,"read")){
            // READ X Y
            int X;
            int Y;
            //unsigned long int binary_input;
            scanf("%d",&X);
            printf("Process Id: %d\n",X);
            char str_input_va[33];
            scanf("%s",str_input_va);
            Y = conver_binary_to_dec(str_input_va);
            printf("Virtual Address : %d\n\n",Y);
            int saveProcess= -1;       // SEND THE INDEX TO write
            for(int i =0;i<4;i++){
                if(X == (int)thread_inc[i]){
                    saveProcess = i;
                }
            }
            
            if(saveProcess == -1){
                printf("Invalid Process ID, Not Created yet. \n");               
            }else{
                int n=0;
                int write_flag=0;
                while((ptable_num[saveProcess] -> virtual_flag[n]) != 0 ){
                    if((ptable_num[saveProcess] -> virtual_address[n] ) == Y){
                        write_flag = 1;
                    }
                    n++;
                }
                if(write_flag == 0){
                    printf("Virtual Address Not allocated yet. \n");  
                }else{

                    int phy_addr = cse320_virt_to_phys(Y,saveProcess);
                    int flaghit= 0;
                    for(int i=0;i<4;i++){
                        if(cache_arr[i] -> p_addr == phy_addr){
                            printf("cache hit\n");
                            //return (cache_arr[i] -> value);
                            printf("Value: %d\n",cache_arr[i]->value);
                            flaghit =1;
                            break;
                        }
                    }
                    if(flaghit == 0){
                        printf("cache miss\n");
                        int saveIndex = Y >> 12;
                        char buf[50];
                        sprintf(buf, "read %d %d",saveProcess,saveIndex);
                        fd = open(myfifo, O_WRONLY);
                        write(fd, buf, 30);
                        close(fd);

                        fd = open(myfifo, O_RDONLY);
                        read(fd, array_input, sizeof(array_input));
                        printf("Value: %s\n", array_input);
                        close(fd);
                        int val = atoi(array_input);
                        add_to_cache(phy_addr,val);
                    }
                }
            }
        }else if(!strcmp(input_func,"write")){
            // WRITE X Y Z
            int X;
            int Y;
            int Z;

            scanf("%d",&X);
            printf("Process ID Value: %d\n",X);
            char str_input_va[33];
            scanf("%s",str_input_va);
            Y = conver_binary_to_dec(str_input_va);
            printf("Virtual Address Value: %d\n",Y);
            scanf("%d",&Z);
            printf("Write Value: %d\n",Z);

            int saveProcess= -1;       // SEND THE INDEX TO write
            for(int i =0;i<4;i++){
                if(X == (int)thread_inc[i]){
                    saveProcess = i;
                }
            }
            if(saveProcess == -1){
                printf("Invalid Process ID, Not Created yet. \n");               
            }else{
                int n=0;
                int write_flag=0;
                while((ptable_num[saveProcess] -> virtual_flag[n]) != 0 ){
                    if((ptable_num[saveProcess] -> virtual_address[n] ) == Y){
                        write_flag = 1;
                    }
                    n++;
                }
                if(write_flag == 0){
                    printf("Virtual Address Not allocated yet. \n");  
                }else{
                    int saveIndex = Y >>12;
                    char buf[50];
                    sprintf(buf, "write %d %d %d",saveProcess,saveIndex,Z);
                    // Open FIFO for write only
                    fd = open(myfifo, O_WRONLY);
                    write(fd, buf, 30);
                    close(fd);

                    int write_cache = cse320_virt_to_phys(Y,saveProcess);
                    add_to_cache(write_cache,Z);
                }
            }
            
        }else if(!strcmp(input_func,"exit")){
            // EXIT
            for(int i=0;i<4;i++){
                free(thread_num[i]);
                free(ptable_num[i]);
            }
            flag_thread = 1;
            printf("exit");
            exit(0);
        }else {
            printf("Wrong USER INPUT : Type Again.\n");
        }
        //printf("Please enter your input:\n");
        printf("ENTER - create|kill X|list|mem X|allocate X|read X Y|write X Y Z|exit|:");
        scanf("%s",input);
        sscanf(input,"%s",input_func);
    }
    flag_thread = 1;

    for(int i=0;i<4;i++){
        free(thread_num[i]);
        free(ptable_num[i]);
    }
    return 0;
}

