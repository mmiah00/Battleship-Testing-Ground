#ifndef _MSG_H_
#define _MSG_H_

#define OK 2

typedef struct attack_message
{
    int type;
    int x, y;

} attack_message;

typedef struct status_message
{
    int type;
    /* 0 = MISS
     * 1 = HIT
     * 2 = OK
     * 3 = FINISH
     */
    int response;

} status_message;


#endif
