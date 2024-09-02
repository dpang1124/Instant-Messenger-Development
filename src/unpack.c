#include <stdlib.h>
#include <string.h>

#include "serialize.h"

/* Unpack the given packet into the buffer message.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that message points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */

/* #define REFRESH    0
#define STATUS     1
#define MESSAGE    2
#define LABELED    3
#define STATISTICS 4
*/

int unpack(char *message, void *packed) {
    
    if(*(int *)packed==3){
        //labeled
        //dpang3: @elb wassup, namelength=6
        //0123456789abcdefghi
        
        int namelength=0;
        for(int i=0; *(char *)(packed + (sizeof(int)+(i*sizeof(char))))!='\0'; i++){
            message[i] = *(char *)(packed + (sizeof(int)+(i*sizeof(char))));
            namelength++;
        }
        message[namelength]=':';
        message[namelength+1]=' ';
        message[namelength+2]='@';
        //above prints "dpang3: @"


        size_t messagelength = *(size_t *)(packed+(5*(sizeof(int))));
        size_t targetlength = *(size_t *)(packed+(7*(sizeof(int))));
        
        for(int i=0; i<targetlength; i++){
            message[namelength+3+i] = *(char *)(packed+ (44+messagelength+i) );
        }

        message[namelength+3+targetlength] = ' ';
        //above prints "target "
        //so far: "dpang3: @elb "
        
        for(int i=0; i<messagelength; i++){
            message[namelength+4+targetlength+i] = *(char *)(packed+(44+i));
        }
        
        return *(int *)packed;
        //above prints "message"

        
    }

    else if(*(int *)packed==1){
        //status
        if(*(size_t *)(packed+(5*sizeof(int)))==0){return -1;}
        if(*(size_t *)(packed+(5*sizeof(int)))>MAX_MESSAGE_SIZE){return -1;}
        //david says wassup namelength=5, message starts at index=7
        int namelength=0;
        for(int i=0; *(char *)(packed + (sizeof(int)+(i*sizeof(char))))!='\0'; i++){
            message[i] = *(char *)(packed + (sizeof(int)+(i*sizeof(char))));
            namelength++;
        }
        message[namelength]=' ';

        
        for(int i=0; i<*(size_t *)(packed+(5*sizeof(int))); i++){
            message[namelength+1+i] = *(char *)(packed+(36+i));
        }
        return *(int *)packed;
        
        
    }
    else if(*(int *)packed==2){
        //message
        if(*(size_t *)(packed+(5*sizeof(int)))==0){return -1;}
        if(*(size_t *)(packed+(5*sizeof(int)))>MAX_MESSAGE_SIZE){return -1;}
        //david: wassup namelength=5, message starts at index=7
        int namelength=0;
        for(int i=0; *(char *)(packed + (sizeof(int)+(i*sizeof(char))))!='\0'; i++){
            message[i] = *(char *)(packed + (sizeof(int)+(i*sizeof(char))));
            namelength++;
        }
        message[namelength]=':';
        message[namelength+1]=' ';
        for(int i=0; i<*(size_t *)(packed+(5*sizeof(int))); i++){
            message[namelength+2+i] = *(char *)(packed+(36+i));
        }
        return *(int *)packed;
        
    }

    
    return -1;
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack_statistics(struct statistics *statistics, void *packed) {

    /*
      if(){return -1;}*/

    //statistics pack looks like by bytes: 4-16-16-4-8-8-4
    statistics->messages_count = *(int *)(packed + 14); //int
    statistics->invalid_count = *(long *)(packed + 5); //long
    statistics->refresh_count = *(long *)(packed + 6); //long
    statistics->most_active_count = *(int *)(packed + 9); //int

    int i=0;
    for(i=0; *(char *)(packed + (sizeof(int)+(i*sizeof(char))))!='\0'; i++){
        statistics->sender[i] = *(char *)(packed + (sizeof(int) + i));
        if(i==15){break;}
    }
    statistics->sender[i] = '\0';

    i=0;
    for(i=0; i<*(char *)(packed + (20 + (sizeof(char)*i)))!='\0'; i++){
        statistics->most_active[i] = *(char *)(packed + (20 + i));
        if(i==15){break;}
    }
    statistics->most_active[i] = '\0';

    return *(int *)packed;