/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"

void echo(int connfd) 
{
    int n; 
    char buf[MAXLINE]; 
    rio_t rio;
    int len;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
	    printf("server received %d bytes\n", n);
        //printf("%d %s\n", connfd, buf);
        if(!strcmp(buf, "exit\n")){
            Rio_writen(connfd, buf, n);
            break;
        }
        else if(!strcmp(buf, "show\n")){
            buf[0] = '\0';
            showTree(headNode, connfd, buf);
            len = strlen(buf);
            Rio_writen(connfd, buf, len+1);
        }
        else{
            char command[5];
            int id = 0, num = 0, i = 0;
            len = strlen(buf);
            for(; ;i++){
                if(buf[i] == ' '){
                    command[i] = '\0';
                    break;
                }
                else{
                    command[i] = buf[i];
                }
            }
            for(i+=1; ; i++){
                if(buf[i] == ' '){
                    break;
                }
                else{
                    id = id * 10 + (buf[i] - '0');
                }
            }
            for(i+=1; ;i++){
                if(buf[i] == '\n'){
                    break;
                }
                else{
                    num = num*10 + (buf[i] - '0');
                }
            }
            stock* node = searchNode(headNode, id);
            if(node == NULL){
                Rio_writen(connfd, "invalid ID\n\0", 12);
            }

            if(!strcmp(command, "buy")){
                if (node->readcnt == 0){
                    if (node->left_stock < num){
                        Rio_writen(connfd, "Not enough left stock\n\0", 23);
                    }
                    else{
                        node->readcnt++;
                        P(&node->mutex);
                        node->left_stock -= num;
                        V(&node->mutex);
                        node->readcnt--;
                        Rio_writen(connfd, "[buy] success\n\0", 15);

                    }
                }

            }
            else{
                if(node->readcnt == 0){
                    node->readcnt++;
                    P(&node->mutex);
                    node->left_stock += num;
                    V(&node->mutex);
                    node->readcnt--;
                    Rio_writen(connfd, "[sell] success\n\0", 16);
                }
            }
            FILE *writefp = fopen("stock.txt", "w");
            writeResult(headNode, writefp);
            fclose(writefp);
        }
    }

}
/* $end echo */



void showTree(stock * head, int connfd, char buf[MAXLINE]){
    char buf2[MAXLINE];
    if(head != NULL){
        showTree(head->left, connfd, buf);
        sprintf(buf2, "%d %d %d\n", head->ID, head->left_stock, head->price);
        strcat(buf, buf2);
    
        //printf("%d %d %d\n", head->ID, head->left_stock, head->price);
        showTree(head->right, connfd, buf);
    }
}

stock* searchNode(stock* head, int id){
    if(head == NULL){
        return NULL;
    }
    else if(head->ID == id){
        return head;
    }
    else if(id < head->ID){
        searchNode(head->left, id);
    }
    else if(id > head->ID){
        searchNode(head->right, id);
    }

}

void writeResult(stock * head, FILE* writefp){
    if(head != NULL){
        writeResult(head->left, writefp);
        fprintf(writefp, "%d %d %d\n", head->ID, head->left_stock, head->price);
        writeResult(head->right, writefp);
    }

}

