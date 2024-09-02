#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#include "serialize.h"

/* Pack the user input provided in input into the appropriate message
 * type in the space provided by packed.  You can assume that input is a
 * NUL-terminated string, and that packed is a buffer of size
 * PACKET_SIZE.
 *
 * Returns the packet type for valid input, or -1 for invalid input.
 */
int pack(void *packed, char *input) {

    //checks if the string is just ascii spaces (error)
    int check = 0;
    for(int i=0; i<strlen(input); i++){
        if(input[i]!=' '){
            check = 1;
        }
    }

    if(check==0){return -1;}


    if(strlen(input)>MAX_MESSAGE_SIZE){return -1;}
    
    char *ubit = "dpang3\0\0\0\0\0\0\0\0\0\0";
    
    
    //STATUS
    int track;
        if(input[0]=='/'&&input[1]=='m'&&input[2]=='e'){
            if(input[3]!=' '){return -1;}

            //packet type 4
            *(int *)packed = STATUS;
            //ubit 20
            for(int i=0; i<NAME_SIZE; i++){
                *(char *)(packed + (sizeof(int)+i)) = ubit[i];
            }
  
            //status length 28 
            for(int i=3; i<strlen(input); i++){
                if(input[i]!=' '){
                    track = i;
                    break;
                }
            }

            *(size_t *)(packed + (5* sizeof(int))) = strlen(input) - track;

            //random size_t = 0 end of array signifier 36
            *(size_t *)(packed + (7* sizeof(int))) = 0;

            //status starts at 36 ends at however long the message is
            for(int i = track; i<strlen(input); i++){
                *(char *)(packed + (36 + (i-track))) = input[i];
            }
           
            
            
    }
        //LABELED
        //"@david   hello" track = 6, track2 = 9, strlen = 14, message = 5 
        else if(input[0]=='@'){
            for(int i=0; i<strlen(input); i++){
                if(input[i]==' '){
                    track = i;
                    break;
                }
            }

            if((track-1) > NAME_SIZE){return -1;}
            
            //packet type 4
            *(int *)packed = LABELED;
            //ubit 20
            for(int i=0; i<NAME_SIZE; i++){
                *(char *)(packed + (sizeof(int)+i)) = ubit[i];
            }
            //message length
            int track2;
            for(int i=track; i<strlen(input); i++){
                if(input[i]!=' '){
                    track2 = i;
                    break;
                }
            }
            *(size_t *)(packed + (5*sizeof(int))) = (strlen(input) - (track2));
            //target length
            *(size_t *)(packed + (7*sizeof(int))) = (track-1);
            //random size_t thingy 0
            *(size_t *)(packed + (9*sizeof(int))) = 0;
            //message
            for(int i=track2; i<strlen(input); i++){
                *(char *)(packed + (11*sizeof(int)+(i-track2))) = input[i];
            }
            //target
            for(int i=1; i<track; i++){
                *(char *)(packed + ((11*sizeof(int)) + (strlen(input)-track2) + (i-1))) = input[i];
            }
           
            
        }

        
        //STATISTICS
        else if(input[0]=='/'&&input[1]=='s'&&input[2]=='t'&&input[3]=='a'&&input[4]=='t'&&input[5]=='s'){
            *(int *)packed = STATISTICS;
            for(int i=0; i<NAME_SIZE; i++){
                *(char *)(packed + (sizeof(int)+i)) = ubit[i];
            }
            
            return *(int *)packed;
        }
        //MESSAGE
        else{

            
            //packet type 4
            *(int *)packed = MESSAGE;
            //ubit 20
            for(int i=0; i<NAME_SIZE; i++){
                *(char *)(packed+ (sizeof(int)+i)) = ubit[i];  
            }
            //message length 28 after line below
            *(size_t *)(packed+(5*(sizeof(int)))) = strlen(input);
            //random size_t thingy 36 after line below
            *(size_t *)(packed + 7*(sizeof(int))) = 0;
            //message starts at 36 and ends at however long the message is
            for(int i=0; i<strlen(input); i++){
                *(char *)(packed + (36 + i)) = input[i];
            }
            return *(int *)packed;
        }

    
        return -1;
}

/* Create a refresh packet for the given message ID.  You can assume
 * that packed is a buffer of size PACKET_SIZE.
 *
 * You should start by implementing this method!
 *
 * Returns the packet type.
 */
int pack_refresh(void *packed, int message_id) {

    
    char str[] = "dpang3\0\0\0\0\0\0\0\0\0\0"; // 16 bytes   
    *(int *)packed = REFRESH; //0x0 to 0x3


    //idk what im doing but.. packed plus 4 bytes plus i so we start at 0x3 plus whatever letter were on i guess
    for(int i=0; i<NAME_SIZE; i++){
        *(char *)(packed + (sizeof(int)+i)) = str[i]; 
    }
    
    //address now starting at 0x20
    *(int *)(packed + (5 * sizeof(int))) = message_id;

 

    return *(int *)packed;