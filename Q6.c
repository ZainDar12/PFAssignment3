#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "members.dat"

typedef struct PersonRec {
    int sid;
    char name[100];
    char dept[16];
    char reg_date[11];
    char birth_date[11];
    int club;   // 1=IEEE, 2=ACM, 3=Both
} PersonRec;


void strip_newline(char *txt) {
    txt[strcspn(txt, "\n")] = '\0';
}

void read_text(const char *msg, char *buf, int size) {
    printf("%s", msg);

    int c = getchar();
    if (c != '\n')
        ungetc(c, stdin);

    fgets(buf, size, stdin);
    strip_newline(buf);
}

int read_int(const char *msg, int min, int max) {
    int val;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &val) == 1 && val >= min && val <= max) {
            while (getchar() != '\n');
            return val;
        }
        printf("Invalid input. Try again.\n");
        while (getchar() != '\n');
    }
}

int load_records(int *count, PersonRec ***ptr) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        fp = fopen(DATA_FILE, "wb");
        return 1;
    }

    PersonRec **arr = NULL;
    PersonRec temp;
    int idx = 0;

    while (fread(&temp, sizeof(PersonRec), 1, fp)) {
        arr = realloc(arr, (idx + 1) * sizeof(PersonRec *));
        PersonRec *node = malloc(sizeof(PersonRec));
        *node = temp;
        arr[idx++] = node;
    }

    *count = idx;
    *ptr = arr;
    fclose(fp);
    return 0;
}

int id_available(PersonRec **list, int len, int sid) {
    for (int i = 0; i < len; i++) {
        if (list[i]->sid == sid)
            return 0;
    }
    return 1;
}

int add_to_memory(PersonRec ***list, int *len, PersonRec rec) {
    PersonRec **arr = *list;

    arr = realloc(arr, (*len + 1) * sizeof(PersonRec *));
    if (!arr) {
        printf("Memory allocation failed (list).\n");
        return 0;
    }

    PersonRec *node = malloc(sizeof(PersonRec));
    if (!node) {
        printf("Memory allocation failed (record).\n");
        return 0;
    }

    *node = rec;
    arr[*len] = node;

    (*len)++;
    *list = arr;
    return 1;
}

int register_menu(PersonRec ***list, int *len) {
    FILE *fp = fopen(DATA_FILE, "ab+");
    if (!fp) {
        printf("Could not open database.\n");
        return 1;
    }

    PersonRec rec;
    printf("\n--- New Registration ---\n");

    rec.sid = read_int("Student ID (1–99999): ", 1, 99999);

    if (!id_available(*list, *len, rec.sid)) {
        printf("ID already exists.\n");
        fclose(fp);
        return 0;
    }

    read_text("Full Name: ", rec.name, sizeof(rec.name));
    read_text("Batch (Dept): ", rec.dept, sizeof(rec.dept));
    read_text("Registration Date (DD/MM/YY): ", rec.reg_date, sizeof(rec.reg_date));
    read_text("Birth Date (DD/MM/YY): ", rec.birth_date, sizeof(rec.birth_date));

    rec.club = read_int("Club (1=IEEE, 2=ACM, 3=Both): ", 1, 3);

    if (fwrite(&rec, sizeof(PersonRec), 1, fp) != 1) {
        printf("Error writing record.\n");
    } else {
        if (add_to_memory(list, len, rec))
            printf("Record saved and loaded into memory.\n");
        else
            printf("Record saved but not loaded to memory.\n");
    }

    fclose(fp);
    return 0;
}

int find_by_id(int sid, PersonRec **list, int len) {
    for (int i = 0; i < len; i++) {
        if (list[i]->sid == sid)
            return i;
    }
    return -1;
}

int update_menu(PersonRec **list, int *len) {
    int sid = read_int("Enter Student ID: ", 1, 99999);
    int idx = find_by_id(sid, list, *len);

    if (idx == -1) {
        printf("Student not found.\n");
        return 1;
    }

    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp) {
        printf("Could not open file.\n");
        return 1;
    }

    PersonRec *orig = list[idx];
    PersonRec temp = *orig;

    fseek(fp, idx * sizeof(PersonRec), SEEK_SET);

    int ch = read_int("[1] Update Club\n[2] Update Batch\nChoice: ", 1, 2);

    if (ch == 1) {
        temp.club = read_int("Club (1=IEEE, 2=ACM, 3=Both): ", 1, 3);
    } else {
        read_text("New Batch: ", temp.dept, sizeof(temp.dept));
    }

    if (fwrite(&temp, sizeof(PersonRec), 1, fp) != 1) {
        printf("Failed to update record.\n");
        fclose(fp);
        return 1;
    }

    *orig = temp;

    printf("Record updated.\n");
    fclose(fp);
    return 0;
}

int delete_menu(PersonRec ***list, int *len) {
    int sid = read_int("Enter Student ID: ", 1, 99999);
    int idx = find_by_id(sid, *list, *len);

    if (idx == -1) {
        printf("Student not found.\n");
        return 1;
    }

    FILE *orig = fopen(DATA_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!orig || !temp) {
        printf("File error.\n");
        return 1;
    }

    PersonRec rec;
    while (fread(&rec, sizeof(PersonRec), 1, orig)) {
        if (rec.sid != sid)
            fwrite(&rec, sizeof(PersonRec), 1, temp);
    }

    fclose(orig);
    fclose(temp);

    remove(DATA_FILE);
    rename("temp.dat", DATA_FILE);

    printf("Record deleted.\n");

    load_records(len, list);
    return 0;
}

int show_all(PersonRec **list, int len) {
    printf("\n====== ALL STUDENTS ======\n");

    if (len == 0) {
        printf("No data found.\n");
        return 0;
    }

    printf("%-6s %-20s %-10s %-12s %-12s %-10s\n",
           "ID", "Name", "Batch", "RegDate", "BirthDate", "Club");

    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < len; i++) {
        PersonRec *p = list[i];

        const char *club =
            (p->club == 1) ? "IEEE" :
            (p->club == 2) ? "ACM" :
            "Both";

        printf("%-6d %-20s %-10s %-12s %-12s %-10s\n",
               p->sid, p->name, p->dept, p->reg_date, p->birth_date, club);
    }

    return 0;
}

int report_by_batch(PersonRec ***list, int *len) {
    char batch[16];
    read_text("Enter Batch: ", batch, 16);

    printf("\n====== BATCH REPORT ======\n");

    int count = 0;

    printf("%-6s %-20s %-10s %-12s %-12s %-10s\n",
           "ID", "Name", "Batch", "RegDate", "BirthDate", "Club");

    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < *len; i++) {
        PersonRec *p = (*list)[i];
        if (strcmp(p->dept, batch) == 0) {
            const char *club =
                (p->club == 1) ? "IEEE" :
                (p->club == 2) ? "ACM" : "Both";

            printf("%-6d %-20s %-10s %-12s %-12s %-10s\n",
                   p->sid, p->name, p->dept, p->reg_date, p->birth_date, club);
            count++;
        }
    }

    printf("Total in batch '%s': %d\n", batch, count);
    return 0;
}

int main() {
    int len = 0;
    PersonRec **list = NULL;

    printf("Loading database...\n");
    load_records(&len, &list);

    while (1) {
        printf("\n=========== MENU ===========\n");
        printf("1. Register Student\n");
        printf("2. Update Record\n");
        printf("3. Delete Student\n");
        printf("4. View All Records\n");
        printf("5. Batch-wise Report\n");
        printf("6. Exit\n");
        printf("Choose: ");

        int ch;
        scanf("%d", &ch);

        switch (ch) {
            case 1: register_menu(&list, &len); break;
            case 2: update_menu(list, &len); break;
            case 3: delete_menu(&list, &len); break;
            case 4: show_all(list, len); break;
            case 5: report_by_batch(&list, &len); break;
            case 6:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}
