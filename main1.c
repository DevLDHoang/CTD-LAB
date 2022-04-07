#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct LinkedList {
    char word[MAX_SIZE];
    int times;
    char listIndex[MAX_SIZE];
    struct LinkedList *next;
} node;

node *head = NULL, *stop = NULL;

node *sortList(node *head_m, node *temp) {
    if (head_m == NULL || strcmp(temp->word, head_m->word) < 0) {
        temp->next = head_m;
        head_m = temp;
    } else {
        node *current_node = head_m, *previous;
        while (current_node->next != NULL && strcmp(temp->word, current_node->word) > 0) {
            previous = current_node;
            current_node = current_node->next;
        }
        if (current_node->next != NULL) {
            temp->next = current_node;
            previous->next = temp;
        } else {
            temp->next = current_node->next;
            current_node->next = temp;
        }
    }
    return head_m;
}

node *insertList(node *head_m, char str[], int line) {
    node *temp = head_m;
    int check_times = 0;
    while (temp != NULL) {
        if (!strcmp(str, temp->word)) {
            check_times = 1;
            break;
        }
        temp = temp->next;
    }
    if (check_times == 0) {
        node *newNode = (node *) malloc(sizeof(node));
        strcpy(newNode->word, str);
        newNode->times = 1;
        sprintf(newNode->listIndex, " %d", line);
        newNode->next = NULL;
        head_m = sortList(head_m, newNode);
        return head_m;
    } else {
        temp->times++;
        char str[10] = "";
        sprintf(str, " %d", line);
        int i = strlen(str) - 1;
        for (i; i != 0; --i) {
            if (str[i] != temp->listIndex[strlen(temp->listIndex) + i - strlen(str)]) {
                sprintf(temp->listIndex, "%s, %d", temp->listIndex, line);
            }
        }
        return head_m;
    }
}

int check(char str[]) {
    node *temp = stop;
    while (temp != NULL) {
        if (!strcmp(str, temp->word))
            return 1;
        temp = temp->next;
    }
    return 0;
}

void readPara() {
    FILE *ptr = fopen("vanban.txt", "r");
    int countLine = 1, mark = 0;
    if (ptr == NULL) {
        printf("Fail to open file vanban.txt!");
    }
    else{
        while (!feof(ptr)) {
            char str[100] = "";
            int chr = 0, curLine = countLine, upper = 0;
            do {
                if (strlen(str) == 0 && chr != tolower(chr))
                    upper = 1;
                sprintf(str, "%s%c", str, tolower(chr));
                chr = fgetc(ptr);
                if (chr == '\n')
                    countLine++;
            } while (isalpha(chr));
            if (strlen(str) != 0 && !check(str))
                head = insertList(head, str, curLine);
            if (mark)
                mark = 0;
            if (chr == '.')
                mark = 1;
        }
    }
    fclose(ptr);
}

void readStop() {
    FILE *ptr = fopen("stopw.txt", "r");
    if (ptr == NULL) {
        printf("Fail to open file stopw.txt!");
    }
    else{
        while (!feof(ptr)) {
            char s[100] = "";
            fscanf(ptr, "%s", s);
            stop = insertList(stop, s, 0);
        }
    }
    fclose(ptr);
}


void Traverser(node *head_m) {
    node *temp = head_m;
    while (temp != NULL) {
        printf("%-15s %-10d %-10s\n", temp->word, temp->times, temp->listIndex);
        temp = temp->next;
    }
}

void freeList(node *head_m) {
    node * p = head_m;
    while (p != NULL)
    {
        free(p);
        p = p->next;
    }
    
}

int main() {
    readStop();
    readPara();
    printf("%-15s %-10s %-10s\n","Word","Times","Indexes(Line)");
    Traverser(head);
    freeList(head);
    freeList(stop);
    return 0;
}