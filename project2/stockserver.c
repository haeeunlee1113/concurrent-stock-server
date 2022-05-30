/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

#define MAXFD 100


int main(int argc, char **argv) 
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid;

    FILE* readfp = fopen("stock.txt", "r");
    if(readfp == NULL){
        printf("file open error\n");
        exit(1);
    }

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    headNode = NULL;
    int id, price, left_stock;
    while(fscanf(readfp, "%d %d %d\n", &id, &left_stock, &price)!=EOF){

        stock* newNode = (stock*) malloc(sizeof(stock));
        newNode->ID = id; newNode->price = price; newNode->left_stock = left_stock;
        newNode->left = NULL; newNode->right = NULL;
        newNode->readcnt = 0; sem_init(&newNode->mutex, 0, 1);
        if(headNode == NULL){
            headNode = newNode;
        }
        else{
            headNode = addNode(headNode, newNode);
        }
    }

    listenfd = Open_listenfd(argv[1]);


    while (1) {

        clientlen = sizeof(struct sockaddr_storage);
        connfdp = Malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        Pthread_create(&tid, NULL, thread, connfdp);
    }

    fclose(readfp);
    //fclsoe(writefp);
    exit(0);
}
/* $end echoserverimain */

void *thread(void *vargp){
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self());
    Free(vargp);
    echo(connfd);
    Close(connfd);
    return NULL;
}

stock * addNode(stock *newNode ,stock *headNode) {

    /* if (newNode->ID < headNode->ID)
         headNode->left = addNode(newNode, headNode->left);
     else
         headNode->right = addNode(newNode, headNode->right);*/

    if (newNode->ID < headNode->ID) {
        if (headNode->left == NULL) {

            headNode->left = newNode;

        } else {

            addNode(newNode, headNode->left);

        }
    }
    else {
        if (headNode->right == NULL) {
            headNode->right = newNode;
        } else
            addNode(newNode, headNode->right);

    }

    return headNode;
}

