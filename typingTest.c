#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define FILENAME_SIZE 1024
#define MAX_LINE 2048
struct USER
{
    char name[50];
    char password[50];
    char time[50];
    int tries;
    float speed;
    float accuracy;
};
struct TYPING
{
    char name[50];
    float wpm;
    float accuracy;
    char time[50];
};

COORD cord = {0,0};
void gotoxy(int x, int y){
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cord);
}
void storeTyping(char *username, float acc, float wordpm ){
    struct TYPING t;
    struct USER u;
    int flag;
    FILE *fp;
    fp = fopen("typinglist.bin", "ab+");
    strcpy(t.name , username);
    t.accuracy = acc;
    t.wpm = wordpm;
    time_t ltime;
    time(&ltime);
    struct tm* now = localtime(&ltime);
    strftime(t.time, sizeof(t.time), "%Y-%m-%d %H:%M:%S", now);
    fwrite(&t, sizeof(struct TYPING), 1,fp);
    fclose(fp);
    fp = fopen("userlist.bin", "rb+"); // Open the file in read-write mode
        while (fread(&u, sizeof(u), 1, fp) == 1) {
            if (strcmp(username, u.name) == 0) {
                u.tries += 1;
                u.speed = (u.speed + wordpm)/u.tries;
                u.accuracy = (u.accuracy + acc)/u.tries;
                fseek(fp, -sizeof(struct USER), SEEK_CUR);
                fwrite(&u, sizeof(struct USER), 1, fp);
                break;
            }
        }
        fclose(fp);
}
int deletefiles(){
    FILE *fp;
    fp = fopen("tempuserlist.bin","r");
    if(fp !=NULL){
        fclose(fp);
    remove("userlist.bin");
    rename("tempuserlist.bin", "userlist.bin");
    }
}
void deleteUser(const char *username) {
    system("cls");
    FILE *file, *tempFile;
    struct USER u;

    file = fopen("userlist.bin", "rb");

    tempFile = fopen("tempuserlist.bin", "wb+");

    while (fread(&u, sizeof(struct USER), 1, file) == 1) {
        if (strcmp(u.name, username) == 0) {
            continue;
        }
        fwrite(&u, sizeof(struct USER), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);
    printf("User '%s' is deleted succefully.\n", username);
    system("pause");
    exit(1);
}

void createUser(){
    system("cls");
    struct USER u;
    char username[20];
    char password[20];
    char input[20]; // Buffer to hold the input
    printf("Your Username (Not more than 50 characters):\n");
    fflush(stdin); 
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%s", username);
    printf("Your password (Not more than 50 characters):\n");
    fflush(stdin);
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%s", password);
    FILE *fp;
    fp = fopen("userlist.bin", "ab+");
    int flg = 1;
    while (fread(&u, sizeof(u), 1, fp) == 1) {
        if (strcmp(username, u.name) == 0) {
                flg = 0;
                break;
            }
    }
    if(flg == 1){
        strcpy(u.name, username);
        strcpy(u.password, password);
        u.tries = 0;
        u.speed = 0;
        u.accuracy = 0;
        time_t ltime;
        time(&ltime);
        struct tm* now = localtime(&ltime);
        strftime(u.time, sizeof(u.time), "%Y-%m-%d %H:%M:%S", now);
        fwrite(&u, sizeof(struct USER), 1,fp);
        printf("Successfully created user %s on %s.\n", u.name, u.time);
    }else{
        printf("Duplicate user entry! Try again.\n");
        system("pause");
        exit(1);
    }
    fclose(fp);
    system("pause");
}
void viewLogs(){
    system("cls");
    struct TYPING t;
    FILE *fp;
    fp = fopen("typinglist.bin", "ab+");
	printf("\n=========================""=============""=================""================================\n");
	printf("\nNAME\t\tACCURACY\t\tSPEED\t\t\tTIME\n");
	printf("===========================""===========""===================""==============================\n");

	while (fread(&t, sizeof(t), 1, fp) == 1)
		printf("\n%s\t\t%4.f %%\t\t%9.f w/min\t\t%s",t.name, t.accuracy,t.wpm, t.time);

	printf("\n");
    fclose(fp);
    system("pause");
}
void viewUser(){
    system("cls");
    struct USER u;
    struct TYPING t;
    FILE *fp;
    fp = fopen("userlist.bin", "ab+");
	printf("\n===============""=================""===================""==========""=======================");
	printf("\nNAME\t\tSPEED\t\tACCURACY\tTRIES\t\tCREATED ON\n");
	printf("=================""=================""===================""===========""====================\n");

	while (fread(&u, sizeof(u), 1, fp) == 1)
		printf("\n%s\t\t%.f wpm\t\t%.f %%\t\t%-5d\t\t%s",
			u.name,u.speed, u.accuracy,u.tries,u.time);

	printf("\n");
    fclose(fp);
	system("pause");
}
int loginUser(char *username, char *pass){
    system("cls");
    struct USER u;
    FILE *fp;
    fp = fopen("userlist.bin", "ab+");
    while(fread(&u, sizeof(u), 1, fp)){
        if( strcmp(username, u.name)==0 && strcmp(pass, u.password)==0){
            return 1;
        }
    }
    fclose(fp);
}

int getRandomLine(FILE *fp, char *buffer) {
    int num_lines = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            num_lines++;
        }
    }

    if (num_lines == 0) {
        printf("Error: The file is empty.\n");
        return 0;
    }

    srand(time(NULL));
    int random_line_number = rand() % num_lines;

    rewind(fp);
    int current_line = 0;
    while (current_line < random_line_number) {
        if (fgets(buffer, MAX_LINE, fp) == NULL) {
            printf("Error: The file does not contain enough lines.\n");
            return 0;
        }
        current_line++;
    }

    if (fgets(buffer, MAX_LINE, fp) != NULL) {
        printf("\n");
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        return 1;
    } else {
        printf("Error: Unable to read the random line.\n");
        return 0;
    }
}



int tokenizeSentence(char *sentence, char *words[]) {
    char *token = strtok(sentence, " ");
    int count = 0;
    while (token != NULL) {
        words[count++] = token;
        token = strtok(NULL, " ");
    }
    return count;
}

void checkspeed(const char *sentence, char *username) {
    system("cls");
    int correct_chars = 0;
    float accuracy;
    char typedsentence[MAX_LINE];
    float wpm;
    int total_chars;
    char another;
    printf("Want to start typing test (y/n)?\n");
    scanf(" %c", &another);
    getchar();
    while (another == 'y')
    {
        system("cls");
        printf("%s\n\nType the text:\n", sentence);
        time_t now = time(NULL);
        fgets(typedsentence, MAX_LINE, stdin);
        time_t total_secs = time(NULL) - now;
        if (typedsentence[strlen(typedsentence) - 1] == '\n')
            typedsentence[strlen(typedsentence) - 1] = '\0';

        total_chars = strlen(sentence);
        for (int i = 0; i < strlen(typedsentence); i++) {
            if (typedsentence[i] == sentence[i]) {
                correct_chars++;
            }
        }
        accuracy = ((float)correct_chars / total_chars) * 100;
        printf("Accuracy is %0.2f %%.\n", accuracy);
        wpm = ((float)strlen(typedsentence) / (total_secs)) * 60/5.0;
        printf("Speed is %d words/min.", (int)wpm);
        storeTyping(username, accuracy, wpm);
        correct_chars = 0; // Reset for the next test
        printf("\nWant to test again (y/n)?\n");
        scanf(" %c", &another);
        getchar();
    }
}
void openFile(char *username){
    system("cls");
    FILE *fp;
    char filename[FILENAME_SIZE] = "typingText.txt";
    char buffer[MAX_LINE];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file.");
    }

    if (getRandomLine(fp, buffer)) {
        checkspeed(buffer, username);
    } else {
        printf("Unable to get a random line from the file.");
    }
    fclose(fp);
    system("cls");
}
int main() {
    char choice;
    char name[50];
    char pass[50];
    char input[20]; 
    system("Color 3F");
    system("cls");
    printf("\n\n\n");
	printf("\n\t\t\t\t=============""====================================""===========");
	printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~");
	printf("\n\t\t\t\t===================""==============================""===========");
	printf("\n\t\t\t\t++++++++++++++++++ TYPING TUTOR ++++++++++++++++++++++++++++");
	printf("\n\t\t\t\t===================""==============================""===========");
	printf("\n\t\t\t\t~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~");
	printf("\n\t\t\t\t=====================""===========================""============\n");
	printf("\n\n\t\t\t\t\t\t""Developer : @Bhuwan Acharya Upadhyaya""\n\n\t\t\t\t");

	system("pause");
	system("cls");
    while (1)
     {
        deletefiles();
        system("cls");
        gotoxy(31,10);
        printf("\n1. CREATE USER\n");
        gotoxy(31,12);
        printf("\n2. LOGIN\n");
        gotoxy(31,14);
        printf("\n3. VIEW TYPING LOGS\n");
        gotoxy(31,16);
        printf("\n4. VIEW USER RECORDS\n");
        gotoxy(31,18);
        printf("\n5. DELETE USER\n");
        gotoxy(31,20);
        printf("\n6. EXIT\n");
        gotoxy(31,22);
        printf("\nType your choice:\n");
        scanf("%c", &choice);
        switch (choice)
        {
        case '1':
            createUser();
            break;
        case '2':
            system("cls");
            printf("Enter your username:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", name);
            printf("Enter your password:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", pass);
            if(loginUser(name, pass)){
                printf("Successfully logged in as %s.\n", name);
                system("pause");
                openFile(name);
            }else{
                printf("Login failed.\n");
                system("pause");
            }
            break;
        case '3':
            viewLogs();
            break;
        case '4':
            viewUser();
            break;
        case '5':
            system("cls");
            printf("Enter your username:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", name);
            printf("Enter your password:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", pass);
            if(loginUser(name, pass)){
                deleteUser(name);
            }else{
                printf("Login failed.\n");
                system("pause");
            }
            break;
        case '6':
            exit(0);
            break;
        
        default:
        printf("Wrong choice!\n");
        }
    }
        return 0;
}
