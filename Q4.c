#include <stdio.h>
#include <string.h>
struct Book {
    int id;
    int popularity;
    int timeAdded; // last accessed or added time
};
int main() {
    int capacity, numOperations;
    scanf("%d %d", &capacity, &numOperations);
    struct Book shelf[capacity];
    int bookCount = 0;
    for (int op = 0; op < numOperations; op++) {
        char operation[10];
        scanf("%s", operation);
        if (strcmp(operation, "ADD") == 0) {
            int id, popularity;
            scanf("%d %d", &id, &popularity);
            int exists = 0;
            for (int i = 0; i < bookCount; i++) {
                if (shelf[i].id == id) {
                    exists = 1;
                    shelf[i].popularity = popularity;
                    shelf[i].timeAdded = op;// use the number of operations to refrence time addded
                    break;
                }
            }
            if (!exists) {
                if (bookCount == capacity) {
                    int minIdx = 0;
                    for (int i = 1; i < bookCount; i++) {
                        if (shelf[i].timeAdded < shelf[minIdx].timeAdded) {
                            minIdx = i;
                        }
                    }
                    for (int i = minIdx; i < bookCount - 1; i++) {
                        shelf[i] = shelf[i + 1];
                    }
                    bookCount--;
                }
                shelf[bookCount].id = id;
                shelf[bookCount].popularity = popularity;
                shelf[bookCount].timeAdded = op;
                bookCount++;
            }
        }
        else if (strcmp(operation, "ACCESS") == 0) {
            int id;
            scanf("%d", &id);

            int found = 0;
            for (int i = 0; i < bookCount; i++) {
                if (shelf[i].id == id) {
                    printf("%d\n", shelf[i].popularity);
                    shelf[i].timeAdded = op;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("-1\n");
            }
        }
    }
    return 0;
}
