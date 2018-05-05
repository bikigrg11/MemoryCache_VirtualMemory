#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int *base;

int read_handler(int phy_addr){
    int *ret_value = (int*) &base[phy_addr];
    return *ret_value;
}

void init_memory(){
    base = (int *) malloc(256 * sizeof(int)); 
    for(int i=0;i<256;i++){
        base[i] = 0;
    }
}

int main(){
    sleep(5);

    init_memory();

    //char input[255];
    char input_func[255];
    //char X[255];
    //char Y[255];
    //char Z[255];

    int fd1;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char str1[80];
    //char str2[80];


    // First open in read only and read
    fd1 = open(myfifo,O_RDONLY);
    read(fd1, str1, 80);
    close(fd1);
    sscanf(str1,"%s",input_func);
    printf("LISTENING - PHYSICAL MEMORY: \n");

    while(strcmp(input_func,"exit") != 0){
        if(!strcmp(input_func,"read")){
            printf("READING FROM PHYSICAL MEMORY.\n");
            int processX;
            int addressY;

            sscanf(str1,"%s %d %d",input_func, &processX, &addressY);
            if(addressY % 4 != 0){
                printf("Address is not aligned\n");
                break;
            }
            if(addressY >= 256){
                printf("address out of range\n");
                break;
            }
            if(processX == 0){
                // do nothing
            }
            else if(processX == 1){
                addressY = addressY + 256;
            }
            else if(processX == 2){
                addressY = addressY + 512;
            }
            else if(processX == 3){
                addressY = addressY + 768;
            }
            else{
                printf("ERROR NOT VALID PROCESS\n");
            }

            int val = addressY / 4;
            char buf[10];
            // convert 123 to string [buf]
            sprintf(buf, "%d", base[val]);
            printf("Reading Value: %d\n",base[val]);
            fd1 = open(myfifo,O_WRONLY);
            write(fd1, buf, 30);
            close(fd1);
        }
        else if(!strcmp(input_func,"write")){
            printf("WRITING TO PHYSICAL MEMORY.\n");
            int processX;
            int addressY;
            int valueZ;

            sscanf(str1,"%s %d %d %d",input_func,&processX,&addressY,&valueZ);
            printf("writing value: %d in Address: %d for Process: %d .\n",valueZ, addressY, processX);

            if(addressY % 4 != 0){
                printf("Address is not aligned\n");
                fd1 = open(myfifo,O_WRONLY);
                write(fd1, "Address is not aligned\n", 30);
                close(fd1);
            }
            if(addressY >= 256){
                printf("address out of range\n");
                fd1 = open(myfifo,O_WRONLY);
                write(fd1, "Address out of range\n", 30);
                close(fd1);
            }

            if(processX == 0){
                // do nothing
            }
            else if(processX == 1){
                addressY = addressY + 256;
            }
            else if(processX == 2){
                addressY = addressY + 512;
            }
            else if(processX == 3){
                addressY = addressY + 768;
            }
            else{
                printf("ERROR NOT VALID PROCESS\n");
            }

            addressY = addressY /4;
            base[addressY] = valueZ;

            //base[addressX] = valueY;
            //printf("replace the stored value in the memory X to address Y value");

            // CHECKING THE FOLLOWING CONDITON:
            // - request address out of range
            // - requested address is not algined( not multiple of 4)
        }else{
            printf("Wrong input USER.\n");
        }
        printf("\n");
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);
        close(fd1);
        sscanf(str1,"%s",input_func);

    }      

    return 0;
}