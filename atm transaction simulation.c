#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* Structure to store account details */
struct Account {
 int accNo;
 char name[50];
 int pin;
 float balance;
};
/* Function declarations */
void createAccount();
void deposit();
void withdraw();
void balanceEnquiry();
void miniStatement();
int main() {
 int choice;
 do {
 printf("\n===== ATM TRANSACTION SIMULATION 
=====\n");
 printf("1. Create Account\n");
 printf("2. Deposit\n");
 printf("3. Withdraw\n");
 printf("4. Balance Enquiry\n");
 printf("5. Mini Statement\n");
 printf("0. Exit\n");
 
printf("=====================================\n");
 printf("Enter your choice: ");
 scanf("%d", &choice);
 switch(choice) {
 case 1: createAccount(); break;
 case 2: deposit(); break;
 case 3: withdraw(); break;
 case 4: balanceEnquiry(); break;
 case 5: miniStatement(); break;
 case 0: printf("Thank you for using ATM!\n"); break;
 default: printf("Invalid choice!\n");
 }
 } while(choice != 0);
 return 0
}
 /* Create new account */
void createAccount() {
 struct Account a;
 FILE *fp = fopen("accounts.txt", "ab");
 if(fp == NULL) {
 printf("File error!\n");
 return;
 }
 printf("Enter Account Number: ");
 scanf("%d", &a.accNo);
 getchar(); // clear buffer
 printf("Enter Name: ");
 fgets(a.name, sizeof(a.name), stdin);
 a.name[strcspn(a.name, "\n")] = '\0';
 do {
 printf("Set 4-digit PIN: ");
 scanf("%d", &a.pin);
 } while(a.pin < 1000 || a.pin > 9999);
 do {
 printf("Enter Initial Deposit: ");
 scanf("%f", &a.balance);
 } while(a.balance < 0);
 fwrite(&a, sizeof(a), 1, fp);
 fclose(fp);
 printf("Account created successfully!\n");
}
/* Deposit amount */
void deposit() {
 struct Account a;
 int acc, enteredPin, found = 0;
 float amt;
 time_t t;
 FILE *fp = fopen("accounts.txt", "rb");
 FILE *temp = fopen("temp.txt", "wb");
 if(fp == NULL || temp == NULL) {
 printf("File error!\n");
 return;
 }
 printf("Enter Account Number: ");
 scanf("%d", &acc);
 printf("Enter Amount to Deposit: ");
 scanf("%f", &amt);
 if(amt <= 0) {
 printf("Invalid amount!\n");
 return;
 }
 while(fread(&a, sizeof(a), 1, fp)) {
 if(a.accNo == acc) {
 printf("Enter PIN: ");
 scanf("%d", &enteredPin);
 if(enteredPin == a.pin) {
 a.balance += amt;
 time(&t);
 FILE *tr = fopen("transactions.txt", "a");
 if(tr != NULL) {
 fprintf(tr, "%d Deposit %.2f %s", acc, amt, 
ctime(&t));
 fclose(tr);
 }
 printf("Deposit successful!\n");
 printf("New Balance: %.2f\n", a.balance);
 } else{
 printf("Incorrect PIN!\n");
 }
 found = 1;
 }
 fwrite(&a, sizeof(a), 1, temp);
 }
 fclose(fp);
 fclose(temp);
 remove("accounts.txt");
 rename("temp.txt", "accounts.txt");
 if(!found)
 printf("Account not found!\n");
}
/* Withdraw amount */
void withdraw() {
 struct Account a;
 int acc, enteredPin, found = 0;
 float amt;
 time_t t;
 FILE *fp = fopen("accounts.txt", "rb");
 FILE *temp = fopen("temp.txt", "wb");
 if(fp == NULL || temp == NULL) {
 printf("File error!\n");
 return;
 }
 printf("Enter Account Number: ");
 scanf("%d", &acc);
 printf("Enter Amount to Withdraw: ");
 scanf("%f", &amt);
 if(amt <= 0) {
 printf("Invalid amount!\n");
 return;
 }
 while(fread(&a, sizeof(a), 1, fp)) {
 if(a.accNo == acc) {
 printf("Enter PIN: ");
 scanf("%d", &enteredPin);
 if(enteredPin == a.pin) {
 if(a.balance >= amt) {
 a.balance -= amt;
 time(&t);
 FILE *tr = fopen("transactions.txt", "a");
 if(tr != NULL) {
 fprintf(tr, "%d Withdraw %.2f %s", acc, amt, 
ctime(&t));
 fclose(tr);
 }
 printf("Withdrawal successful!\n");
 printf("New Balance: %.2f\n", a.balance);
 } else {
 printf("Insufficient balance!\n");
 }
 } else {
 printf("Incorrect PIN!\n");
 }
 found = 1;
 }
 fwrite(&a, sizeof(a), 1, temp);
 }
 fclose(fp);
 fclose(temp);
 remove("accounts.txt");
 rename("temp.txt", "accounts.txt");
 if(!found)
 printf("Account not found!\n");
 }
/* Balance enquiry */
void balanceEnquiry() {
 struct Account a;
 int acc, enteredPin, found = 0;
 FILE *fp = fopen("accounts.txt", "rb");
 if(fp == NULL) {
 printf("File error!\n");
 return;
 }
 printf("Enter Account Number: ");
 scanf("%d", &acc);
 while(fread(&a, sizeof(a), 1, fp)) {
 if(a.accNo == acc) {
 printf("Enter PIN: ");
 scanf("%d", &enteredPin);
 if(enteredPin == a.pin) {
 printf("\nAccount No: %d", a.accNo);
 printf("\nName : %s", a.name);
 printf("\nBalance : %.2f\n", a.balance);
 } else {
 printf("Incorrect PIN!\n");
 }
 found = 1;
 break;
 }
 }
 fclose(fp);
 if(!found)
 printf("Account not found!\n");
}
/* Mini statement */
void miniStatement() {
 int acc, accNo;
 char line[200];
 int found = 0;
 FILE *fp = fopen("transactions.txt", "r");
 if(fp == NULL) {
 printf("No transactions available!\n");
 return;
 printf("Enter Account Number: ");
 scanf("%d", &acc);
 printf("\n--- Mini Statement ---\n");
 while(fgets(line, sizeof(line), fp)) {
 sscanf(line, "%d", &accNo);
 if(accNo == acc) {
 printf("%s", line);
 found = 1;
 }
 }
 fclose(fp);
 if(!found)
 printf("No transactions found for this account!\n");
}