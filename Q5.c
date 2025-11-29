#include <stdio.h>
#include<string.h>
#include <error.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    char **content;
    int totalLines;
    int maxCapacity;
} Document;

void setup(Document *doc){
    doc->totalLines = 0;
    doc->maxCapacity = 50;
    doc->content = (char **)malloc(doc->maxCapacity * sizeof(char*));
    if (doc->content == NULL){
        printf("Allocation error\n");
        exit(1);
    }
}
void addLine(Document *doc, const char *str, int pos){
    if (pos < 0 || pos > doc->totalLines) {
        printf("Invalid position\n");
        return;
    }
    if(doc->totalLines >= doc->maxCapacity){ 
        doc->maxCapacity *= 2;
        char **temp = (char **)realloc(doc->content, sizeof(char *) * doc->maxCapacity);
        if(temp == NULL){
            printf("Reallocation error\n");
            exit(1);
        }
        doc->content = temp;
    }
    char *newStr = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if(newStr == NULL){
        printf("Allocation error\n");
        exit(1);
    }
    strcpy(newStr, str);
    memmove(&doc->content[pos + 1], &doc->content[pos], 
            (doc->totalLines - pos) * sizeof(char *));
    doc->content[pos] = newStr;
    doc->totalLines++;
}
void removeLine(Document *doc, int pos){
    if(pos < 0 || pos >= doc->totalLines){
        printf("Invalid position\n");
        return;
    }
    free(doc->content[pos]);
    memmove(&doc->content[pos], &doc->content[pos+1], (doc->totalLines - pos - 1) * sizeof(char *));
    doc->totalLines--;
}
void display(Document *doc) {
    for (int i = 0; i < doc->totalLines; i++) {
        printf("%d: %s\n", i, doc->content[i]);
    }
}
void cleanup(Document *doc) {
    for (int i = 0; i < doc->totalLines; i++) {
        free(doc->content[i]);
    }
    free(doc->content);
    doc->totalLines = 0;
    doc->maxCapacity = 0;
    doc->content = NULL;
}
void compact(Document *doc){
    if(doc->maxCapacity == doc->totalLines) return;
    char **temp = (char **)realloc(doc->content, sizeof(char*) * doc->totalLines);
    if(temp == NULL){
        printf("Compact failed\n");
        exit(1);
    }
    doc->content = temp;
    doc->maxCapacity = doc->totalLines;
}
void writeFile(Document *doc, const char *fname){
    FILE *fp = fopen(fname, "w");
    if (fp == NULL){
        printf("Cannot open file\n");
        exit(1);
    }
    for (int i = 0; i < doc->totalLines; i++){
        fprintf(fp, "%s\n", doc->content[i]);
    }
    fclose(fp);
    printf("Saved\n");
}

void readFile(Document *doc, const char *fname){
    cleanup(doc);
    setup(doc);
    FILE *fp = fopen(fname, "r");
    if (fp == NULL){
        printf("Cannot open file\n");
        exit(1);
    }
    char buf[1000];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strcspn(buf, "\n")] = 0;
        addLine(doc, doc->totalLines, buf);
    }
    fclose(fp);
    printf("Loaded\n");
}

int main() {
    Document doc;
    setup(&doc);
    int opt, pos;
    char str[1000], fname[100];
    while (1) {
        printf("\nEditor\n");
        printf("1. Add\n2. Remove\n3. View\n");
        printf("4. Save\n5. Load\n6. Compact\n7. Quit\n");
        printf("Pick: ");
        scanf("%d", &opt);
        
        switch (opt) {
            case 1:
                printf("Pos: "); scanf("%d", &pos);
                printf("Text: "); 
                getchar();
                fgets(str, sizeof(str), stdin);
                str[strcspn(str, "\n")] = 0;
                addLine(&doc, str, pos);
                break;
                
            case 2:
                printf("Pos: "); 
                scanf("%d", &pos);
                removeLine(&doc, pos);
                break;
                
            case 3:
                display(&doc);
                break;
                
            case 4:
                printf("Filename: "); 
                scanf("%s", fname);
                writeFile(&doc, fname);
                break;
                
            case 5:
                printf("Filename: ");
                scanf("%s", fname);
                readFile(&doc, fname);
                break;
                
            case 6:
                compact(&doc);
                break;
                
            case 7:
                cleanup(&doc);
                printf("Done\n");
                return 0;
        }
    }
}

