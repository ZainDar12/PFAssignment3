#include <stdio.h>
#include <string.h>
struct EmployeeInfo
    {
       char name[50];
       char EmployeeID[20];
       char Designation[20];
       float salary;
    };

void DisplayEmployees(struct EmployeeInfo emp[]){
    printf("------------Employees Info--------- \n");
    printf("Name\tEmployeeID\tDesignation\tSalary \n");
    for (int i=0;i<100;i++){
        printf("%s\t%s\t%s\t%.2f\n",emp[i].name,emp[i].EmployeeID,emp[i].Designation,emp[i].salary);
    }
}
void EmployeeData(struct EmployeeInfo *emp){
    printf("Please enter how many Employees data you want to enter:\n");
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        printf("Enter the EmployeeName:\n");
        scanf("%s",(emp+i)->name);
        printf("Enter the EmployeeID: \n");
        scanf("%s",(emp+i)->EmployeeID);
        printf("Enter the Designation: \n");
        scanf("%s",(emp+i)->Designation);
        printf("Enter the Employees Salary:\n");
        scanf("%f",&(emp+i)->salary);
    }
}
void SearchEmployee(struct EmployeeInfo emp[]){
    printf("Would you like to search by 1: Employee ID \n 2:Name of Employee\n");
    int choice;
    scanf("%d",&choice);
    int found = 0;
    switch(choice){
        case 1:
            printf("Please enter the Employee ID: \n");
            char ID[50];
            scanf("%s",ID);
            for (int i=0;i<100&&found==0;i++){
                if(strcmp(emp[i].EmployeeID,ID)==0) found=1;
            }
            break;
        case 2:
            printf("Please enter the Employee Name: \n");
            char Name[50];
            scanf("%s",Name);
            for (int i=0;i<100&&found==0;i++){
                if(strcmp(emp[i].name,Name)==0) found=1;
            }
            break;
        default:
            printf("Wrong input \n");
    }
}
void FindHighestSal(struct EmployeeInfo emp[]){
    float max=0;
    int j=0;
    for (int i=0;i<100;i++){
        if(emp[i].salary>max){
            max=emp[i].salary;
            j=i;
        }
    }
    printf("Employee Name: %s EmployeeID: %s Designation: %s Salary: %.2f\n",emp[j].name,emp[j].EmployeeID,emp[j].Designation,emp[j].salary);
}

int main(){
    // assume we store 100 employees data
    struct EmployeeInfo employees[100];
    printf("Would you like to 1:Enter Employee Data? \n 2:View Employee Records \n 3:Search Employee \n 4: Find Highest Salary");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1:
            EmployeeData(employees);
            break;
        case 2:
            DisplayEmployees(employees);
            break;
        case 3:
            SearchEmployee(employees);
            break;
        case 4:
            FindHighestSal(employees);
            break;
        default:
            printf("Wrong input");
    }
}
