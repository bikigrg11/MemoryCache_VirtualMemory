#include <stdio.h>
#include <stdlib.h>


// Structs for Various Process:

typedef struct second_layer{
    int virtual_address[256];
    int virtual_flag[256];
    //int index_ref[256];
    int valid_bit[256];
}second_level;

typedef struct first_layer{
    second_level *ptr[1];
    int use_flag;
    pthread_t ptr_thread;
}first_level;

// Mem Cache
typedef struct mem_cache{
    int p_addr;
    int value;
}mem_cache;


// Initalization of Global Variables
int allocIndex[4];
mem_cache *cache_arr[4];
int cache_counter = 0;
int evec_flag=0;


int cse320_malloc(int X){
    int index=allocIndex[X];
    //int dir1_pointer=0;
    int address = 0;
    address = address | index;
    address = address << 12;
    return address;
}

void add_to_cache(int physical_add, int val){
  
    for(int i=0;i<4;i++){
        // Replace old value with the new one for same address
        if(cache_arr[i]->p_addr == physical_add){
            cache_arr[i]->value = val;
            return;
        }
    }

    if(evec_flag == 1){
        if(cache_counter >=4 ){
            cache_counter = 0;
        }
        printf("eviction\n");
    }

    if(cache_counter >=4 && evec_flag==0){
        printf("eviction\n");
        cache_counter = 0;
        evec_flag= 1;
    }


    cache_arr[cache_counter]->p_addr = physical_add;
    cache_arr[cache_counter]->value = val;
    cache_counter++;
}

int cse320_virt_to_phys(int virtual_address, int proces_id){
    virtual_address = virtual_address >> 12;
    
    if(proces_id == 0){
        // do nothing
    }
    else if(proces_id == 1){
        virtual_address = virtual_address + 256;
    }
    else if(proces_id == 2){
        virtual_address = virtual_address + 512;
    }
    else if(proces_id == 3){
        virtual_address = virtual_address + 768;
    }
    else{
        printf("ERROR NOT VALID PROCESS\n");
    }
    return virtual_address;
}

int flag_thread = 0;

void *run_method(){
    
    while(1){
        if(flag_thread == 1)
            return NULL;
    }
    return NULL;
}

//REFERENCED : https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
void printBits(size_t const size, void const * const ptr){
    unsigned char characters;
    unsigned char *b = (unsigned char*) ptr;
    for (int i=size-1;i>=0;i--){
        for (int j=7;j>=0;j--){
            characters = (b[i] >> j) & 1;
            printf("%u", characters);
        }
    }
    printf("\n");
}

// REFERENCE: https://www.geeksforgeeks.org/program-binary-decimal-conversion/
int conver_binary_to_dec(char *str){
    char *ptr;
    unsigned long int n = strtol(str, &ptr, 10);
    unsigned long int  Y=0;
    unsigned long int  start=1;
    unsigned long int  rem=0;
    while (n > 0)
    {
        rem = n % 10;
        Y = Y + rem * start;
        n = n / 10 ;
        start = start * 2;
    }
    return Y;
}
