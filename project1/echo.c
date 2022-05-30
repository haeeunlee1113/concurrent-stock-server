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
            char menu[5];
            int id = 0, num = 0, index = 0;
            len = strlen(buf);
            if (buf[3] == ' ') {
                for (index = 0; index < 4; index++) {
                    if (buf[index] == ' ') {
                        menu[index] = '\0';
                    } else {
                        menu[index] = buf[index];
                    }

                }
            } else if (buf[3] != ' ') {
                for (index = 0; index < 5; index++) {
                    if (buf[index] == ' ') {
                        menu[index] = '\0';
                    } else {
                        menu[index] = buf[index];
                    }
                }
            }

            while(buf[index] != ' ') {
                id = id * 10 + (buf[index]-'0');
                index++;
            }


            index = index +1;


            while(buf[index] != '\n') {

                num = num * 10 + (buf[index] - '0');
                index++;
            }

            stock* node = searchNode(headNode, id);
            if(node == NULL){
                Rio_writen(connfd, "invalid ID\n\0", 12);
            }

            if(!strcmp(menu, "sell")){
                if (node->readcnt == 0){
                    if (node->left_stock < num){
                        Rio_writen(connfd, "Not enough left stock\n\0", 23);
                    }
                    else{
                        node->readcnt++;
                        node->left_stock -= num;
                        node->readcnt--;
                        Rio_writen(connfd, "[sell] success\n\0", 15);

                    }
                }

            }
            else{
                if(node->readcnt == 0){
                    node->readcnt++;
                    node->left_stock += num;
                    node->readcnt--;
                    Rio_writen(connfd, "[buy] success\n\0", 16);
                }
            }
            FILE *writefp = fopen("stock.txt", "w");
            writeResult(headNode, writefp);
            fclose(writefp);
        }
    }

}
/* $end echo */
stock* searchNode(stock* head, int id){
    if(head == NULL){
        return NULL;
    }
    else if(id == head->ID){
        return head;
    }
    else if(id < head->ID){
        searchNode(head->left, id);
    }
    else if(id > head->ID){
        searchNode(head->right, id);
    }

}

void showTree(stock * head, int connfd, char buf[MAXLINE]){
    char buf2[MAXLINE];
    if(head != NULL){
        showTree(head->left, connfd, buf);
        sprintf(buf2, "%d %d %d\n", head->ID, head->left_stock, head->price);
        strcat(buf, buf2);
    
        showTree(head->right, connfd, buf);
    }
}



void writeResult(stock * head, FILE* writefp){
    if(head != NULL){
        writeResult(head->left, writefp);
        fprintf(writefp, "%d %d %d\n", head->ID, head->left_stock, head->price);
        writeResult(head->right, writefp);
    }

}





