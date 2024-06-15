#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ACCOUNT_NUMBER_LENGTH 10

struct account{
    unsigned long long  acc_num;
    int balance;
    int pin;



};
void main_menu();
void create_account();
void transfer();
void check_balance();
void cash_out();
void cash_in();

void main_menu(){
    while(1){
    printf("WELCOME TO YOUR BANK ACCOUNT\n");
    printf("Enter number to choose option\n");
    printf("1. Transfer Money     2. Check Balance\n3.Withdraw Cash        4.Deposit Cash\n5. Create Account     6. Exit");
    int user_choice;
    printf("Enter Choice:  ");
    scanf("%d",&user_choice);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    //use switch to toggle choices
    switch(user_choice){
        case 1:
            transfer();
            break;
        case 2:
            check_balance();
            break;
        case 3:
            cash_out();
            break;
        case 4:
            cash_in();
            break;
        case 5:
            create_account();
            break;
        case 6:
            exit(0);
            break;
        default:
            printf("Enter choice: ");
            main_menu();


    }
    }

}
char* generate_account_number() {
    static unsigned long long last_account_number = 1000000000; // Initial account number
    last_account_number++;
    
    char *account_number = malloc((ACCOUNT_NUMBER_LENGTH + 1) * sizeof(char)); // +1 for null-terminator
    if (account_number == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    snprintf(account_number, ACCOUNT_NUMBER_LENGTH + 1, "%010llu", last_account_number);
    return account_number;
}

void create_account(){
    FILE *file;
    struct account new;
    file=fopen("acc.txt","ab+");
    if(file==NULL){
        printf("Error! ");
        return;
    }
    char *acc_num_str=generate_account_number();
    new.acc_num = strtoull(acc_num_str, NULL, 10);
    free(acc_num_str);
    new.balance=0;
    printf("This is your account number: %llu\n", new.acc_num);
    printf("\n Set your 4 digit pin code: ");
    scanf("%d",&new.pin);
    fwrite(&new,sizeof(new),1,file);
    printf("Account created succesfully! ");
    fclose(file);
    printf("\nEnter any key to continue");
    getchar();
    getchar();
    main_menu();
    
}

void cash_out(){
    FILE*f;
    struct account add;
    f=fopen("acc.txt","rb+");
    if (f==NULL){
        printf("Error, record file not found!\n");
        return;
    }
    unsigned long long temp_num;
    printf("Enter account number: ");
    scanf("%llu",&temp_num);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    while(fread(&add,sizeof(add),1,f)==1){
        if(temp_num==add.acc_num){
            int amt_to_add;
            printf("\nSuccess!\nEnter amount to withdraw: ");
            scanf("%d",&amt_to_add);
            if(amt_to_add>add.balance){
                printf("Broke!");
                exit(0);
            }
            
            while ((c = getchar()) != '\n' && c != EOF);
            int temp_pin;
            printf("Enter 4 digit pin to confirm withdrawal: ");
            scanf("%d",&temp_pin);
             
            while ((c = getchar()) != '\n' && c != EOF);
            if(temp_pin==add.pin){
                printf("Withdrawal successful! ");
                add.balance-=amt_to_add;
                fseek(f, -sizeof(struct account), SEEK_CUR);
                fwrite(&add,sizeof(add),1,f);
                fclose(f);
                printf("Enter any key to continue...");
                getchar();
                main_menu();
                return;
            }else{
                fclose(f);
                main_menu();
                return;

            }



        }
        printf("Account number not found!\n");
    fclose(f);
    printf("Enter any key to continue...");
    getchar();
    main_menu();
        }

    

    
}

void cash_in(){
    FILE*f;
    struct account add;
    f=fopen("acc.txt","rb+");
    if (f==NULL){
        printf("Error, record file not found!\n");
        return;
    }
    unsigned long long temp_num;
    printf("Enter account number: ");
    scanf("%llu",&temp_num);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    while(fread(&add,sizeof(add),1,f)==1){
        if(temp_num==add.acc_num){
            int amt_to_add;
            printf("\nEnter amount to deposit: ");
            scanf("%d",&amt_to_add);
            
            while ((c = getchar()) != '\n' && c != EOF);
            int temp_pin;
            printf("Enter 4 digit pin to confirm deposit: ");
            scanf("%d",&temp_pin);
             
            while ((c = getchar()) != '\n' && c != EOF);
            if(temp_pin==add.pin){
                printf("Deposit successful! ");
                add.balance+=amt_to_add;
                fseek(f, -sizeof(struct account), SEEK_CUR);
                fwrite(&add,sizeof(add),1,f);
                fclose(f);
                printf("Enter any key to continue...");
                getchar();
                main_menu();
                return;
            }else{
                fclose(f);
                main_menu();
                return;

            }



        }
        printf("Account number not found!\n");
    fclose(f);
    printf("Enter any key to continue...");
    getchar();
    main_menu();
        }

    



}

void transfer() {
    FILE *f;
    struct account source, destination;
    unsigned long long source_acc_num, dest_acc_num;
    int transfer_amount, pin, found_source = 0, found_destination = 0;

    f = fopen("acc.txt", "rb+");
    if (f == NULL) {
        printf("Error, record file not found!\n");
        return;
    }

    printf("Enter your account number: \n");
    scanf("%llu", &source_acc_num);
    printf("Enter your pin: \n");
    scanf("%d", &pin);
    printf("Enter destination account number: \n");
    scanf("%llu", &dest_acc_num);
    printf("Enter transfer amount: \n");
    scanf("%d", &transfer_amount);

    while (fread(&source, sizeof(source), 1, f) == 1) {
        if (source.acc_num == source_acc_num) {
            found_source = 1;
            if (source.pin == pin) {
                if (source.balance >= transfer_amount) {
                    fseek(f, 0, SEEK_SET);
                    while (fread(&destination, sizeof(destination), 1, f) == 1) {
                        if (destination.acc_num == dest_acc_num) {
                            found_destination = 1;
                            break;
                        }
                    }

                    if (found_destination) {
                        source.balance -= transfer_amount;
                        destination.balance += transfer_amount;

                        // Update destination account balance
                        fseek(f, -sizeof(struct account), SEEK_CUR);
                        fwrite(&destination, sizeof(destination), 1, f);

                        // Update source account balance
                        fseek(f, 0, SEEK_SET);
                        while (fread(&source, sizeof(source), 1, f) == 1) {
                            if (source.acc_num == source_acc_num) {
                                fseek(f, -sizeof(struct account), SEEK_CUR);
                                fwrite(&source, sizeof(source), 1, f);
                                break;
                            }
                        }

                        printf("Transaction successful! \n");
                    } else {
                        printf("Destination account not found!\n");
                    }
                } else {
                    printf("Insufficient balance!\n");
                }
            } else {
                printf("Incorrect pin\n");
            }
            break;
        }
    }

    if (!found_source) {
        printf("Source account not found!\n");
    }

    fclose(f);
    printf("Enter any key to continue...");
    getchar();
    getchar();
    main_menu();
}


void check_balance(){
    FILE *f;
    struct account acc;
    unsigned long long temp_num;
    int temp_pin, found = 0;

  printf("Enter account number: ");
    scanf("%llu", &temp_num);
  printf("Enter 4 digit pin: ");
    scanf("%d", &temp_pin);

    f = fopen("acc.txt", "rb");
    if (f == NULL) {
        printf("Error!\n");
        return;
    }

    while(fread(&acc,sizeof(acc), 1, f) == 1) {
        if (acc.acc_num == temp_num && acc.pin == temp_pin) {
            found = 1;
            printf("Your account balance is: %d\n", acc.balance);
            break;
        }
    }

    if(!found) {
        printf("Account number or pin incorrect.\n");
    }

    fclose(f);
    printf("Enter any key to continue...");
    getchar();
    getchar();
    
}

int main() {
    main_menu();
    return 0;
}
