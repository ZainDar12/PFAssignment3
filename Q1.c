
#include <stdio.h>
#include <math.h>
double calculateRepayment(double loan, double interestRate, int years, double installment, double extraPayment, int year) {
    if (loan <= 0 || years == 0) {
        printf("Year %d: Remaining loan = 0\n", year);
        return 0;
    }
    printf("Year %d: Remaining loan = %.2f\n", year, loan);
    double payment = installment + extraPayment;
    double newLoan = loan - payment;
    if (newLoan < 0) {
        payment += newLoan;
        newLoan = 0;
    }
    newLoan *= (1 + interestRate);
    return payment + calculateRepayment(newLoan,interestRate,years-1,installment,extraPayment,year+1);
}
int main() {
    double total = calculateRepayment(100000, 0.05, 3, 33333.33, 0, 1);
    printf("Total paid: %.2f\n", total);
    return 0;

}
