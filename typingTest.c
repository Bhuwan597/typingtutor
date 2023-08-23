#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<windows.h>
#define FILENAME_SIZE 1024
#define MAX_LINE 2060
struct USER
{
    char name[50];
    char password[50];
    char time[50];
    int tries;
    float speed;
    float accuracy;
    int rank;
};
struct TYPING
{
    char name[50];
    float wpm;
    float accuracy;
    char time[50];
    int rank;
};

COORD cord = {0,0};
void gotoxy(int x, int y){
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
    fflush(stdout);
}
void loading(){
    system("cls");
    char a = 177, b = 219;
    printf("\n\n\n\n\n\n\n\t\t\t\t\t\t");
    for (int i = 0; i < 100; i++) {
        printf("%c", a);
    }

    for (int i = 0; i < 100; i++) {
        printf("\r");
        printf("\t\t\t\t\t\t");
        printf("%d%%  ", i);
        Sleep(0);
        for (int j = 0; j < i; j++) {
            printf("%c", b);
        }
    }
    system("cls");
}
void storeTyping(char *username, float acc, float wordpm) {
    struct TYPING t;
    struct USER u;
    int flag;
    FILE *fp;
    fp = fopen("typinglist.bin", "ab+");
    strcpy(t.name, username);
    t.accuracy = acc;
    t.wpm = wordpm;
    time_t ltime;
    time(&ltime);
    struct tm *now = localtime(&ltime);
    strftime(t.time, sizeof(t.time), "%Y-%m-%d %H:%M:%S", now);
    fwrite(&t, sizeof(struct TYPING), 1, fp);
    fclose(fp);
    fp = fopen("userlist.bin", "rb+"); // Open the file in read-write mode
        while (fread(&u, sizeof(u), 1, fp) == 1) {
            if (strcmp(username, u.name) == 0) {
                u.tries++;
                u.speed = (u.speed * (u.tries - 1) + wordpm) / u.tries;
                u.accuracy = (u.accuracy * (u.tries - 1) + acc) / u.tries;
                fseek(fp, -sizeof(struct USER), SEEK_CUR);
                fwrite(&u, sizeof(struct USER), 1, fp);
                break;
            }
        }
        fclose(fp);

    // Update ranks for all users
    struct USER users[100]; // Assuming max 100 users, you can adjust this value accordingly
    int numUsers = 0;

    // Read users from the file
    fp = fopen("userlist.bin", "rb");
    while (fread(&users[numUsers], sizeof(struct USER), 1, fp) == 1) {
        numUsers++;
    }
    fclose(fp);

    // Sort the users based on accuracy in descending order
    for (int i = 0; i < numUsers - 1; i++) {
        for (int j = 0; j < numUsers - i - 1; j++) {
            if (users[j].accuracy < users[j + 1].accuracy) {
                struct USER temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    // Update the rank and write back to the file
    fp = fopen("userlist.bin", "wb");
    for (int i = 0; i < numUsers; i++) {
        users[i].rank = i + 1; // Assign the rank based on the sorted order
        fwrite(&users[i], sizeof(struct USER), 1, fp);
    }
    fclose(fp);
}

void updateUsers(){
    FILE *fp;
    fp = fopen("userlist.bin", "rb+");
    struct USER users[100]; 
    int numUsers = 0;

    while (fread(&users[numUsers], sizeof(struct USER), 1, fp) == 1) {
        numUsers++;
    }

    for (int i = 0; i < numUsers - 1; i++) {
        for (int j = 0; j < numUsers - i - 1; j++) {
            if (users[j].accuracy < users[j + 1].accuracy) {
                struct USER temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < numUsers; i++) {
        users[i].rank = i + 1; // Assign the rank based on the sorted order
        fseek(fp, i * sizeof(struct USER), SEEK_SET);
        fwrite(&users[i], sizeof(struct USER), 1, fp);
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
    loading();
    printf("\nUser '%s' is deleted succefully.\n", username);
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
        u.rank = 0;
        time_t ltime;
        time(&ltime);
        struct tm* now = localtime(&ltime);
        strftime(u.time, sizeof(u.time), "%Y-%m-%d %H:%M:%S", now);
        fwrite(&u, sizeof(struct USER), 1,fp);
        char *text = "Creating User.....................";
        loading(text);
        printf("\nSuccessfully created user %s on %s.\n", u.name, u.time);
    }else{
        printf("\nDuplicate user entry! Try again.\n");
        system("pause");
        exit(1);
    }
    fclose(fp);
    system("pause");
}
void logs()
{
    system("cls");
    struct TYPING logs[100]; // Assuming max 100 logs, you can adjust this value accordingly
    int numLogs = 0;

    FILE *fp = fopen("typinglist.bin", "ab+");
    loading();
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // Read typing logs from the file
    while (fread(&logs[numLogs], sizeof(struct TYPING), 1, fp) == 1)
    {
        numLogs++;
    }

    fclose(fp);

    if (numLogs == 0)
    {
        printf("No typing logs found.\n");
        system("pause");
        return;
    }
    for (int i = 0; i < numLogs - 1; i++)
    {
        for (int j = 0; j < numLogs - i - 1; j++)
        {
            if (logs[j].accuracy < logs[j + 1].accuracy)
            {
                struct TYPING temp = logs[j];
                logs[j] = logs[j + 1];
                logs[j + 1] = temp;
            }
        }
    }
    // Display the leaderboard
	printf("\n===============""================="" LEADERBOARD ""==========""=====================\n");
	printf("\nNAME\t\tACCURACY\t\tSPEED\t\t\tTIME\n");
	printf("=================""=================""===================""===========""==============");
    for (int i = 0; i < numLogs; i++)
    {
        printf("\n%s\t\t%4.f %% \t\t%10.f wpm\t\t%s",
			logs[i].name, logs[i].accuracy, logs[i].wpm, logs[i].time);
    }
    printf("\n");
    system("pause");
}

void displayLeaderboard(){
    system("cls");
    struct USER u;
    FILE *fp;
    fp = fopen("userlist.bin", "ab+");
    loading();
	printf("\n===============""================="" LEADERBOARD ""==========""=======================================\n");
	printf("\nRANK\tNAME\t\tSPEED\t\tACCURACY\tTRIES\t\tCREATED ON\n");
	printf("=================""=================""===================""===========""==============================\n");

	while (fread(&u, sizeof(u), 1, fp) == 1)
		printf("\n%d\t%s\t\t%.f wpm\t\t%.f %%\t\t%-6d\t\t%s",u.rank,
			u.name,u.speed, u.accuracy,u.tries,u.time);

	printf("\n");
    fclose(fp);
	system("pause");
}
void viewSpecificUser(char *username){
    struct USER u;
    FILE *fp;
    fp = fopen("userlist.bin", "ab+");
    printf("\nYour Records:");
	printf("\n===============""=================""===================""==========""=======================");
	printf("\nNAME\t\tSPEED\t\tACCURACY\tTRIES\t\tCREATED ON\n");
	printf("=================""=================""===================""===========""====================\n");

	while (fread(&u, sizeof(u), 1, fp) == 1)
        if(strcmp(u.name,username)==0){
		printf("\n%s\t\t%.f wpm\t\t%.f %%\t\t%-5d\t\t%s",
			u.name,u.speed, u.accuracy,u.tries,u.time);
        }
	printf("\n\n");
    fclose(fp);
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
int changePassword(char *username, char *password, char *newPassword){
    struct USER u;
    FILE *fp;
    fp = fopen("userlist.bin", "rb+");
        while (fread(&u, sizeof(u), 1, fp) == 1) {
            if (strcmp(username, u.name) == 0 && strcmp(password, u.password)==0)  {
                strcpy(u.password, newPassword);
                fseek(fp, -sizeof(struct USER), SEEK_CUR);
                fwrite(&u, sizeof(struct USER), 1, fp);
                fclose(fp);
                return 1;
            }
        }
        fclose(fp);
        return 0;
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
        loading();
        accuracy = ((float)correct_chars / total_chars) * 100;
        printf("\nAccuracy is %0.2f %%.\n", accuracy);
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
void printUserManual() {
    loading();
    system("cls");
    printf("=========================================================================================================================\n");
    printf("                                               TYPING TUTOR USER MANUAL                                                 \n");
    printf("=========================================================================================================================\n");
    printf("\nWelcome to the Typing Tutor program!\n");
    printf("This program is designed to help you improve your typing skills.\n");
    printf("Follow the instructions below to use the program effectively.\n");
    printf("\n1. CREATE USER:\n");
    printf("   - Use this option to create a new user account for the typing tutor.\n");
    printf("   - Enter your desired username and password to create the account.\n");
    printf("\n2. LOGIN:\n");
    printf("   - Use this option to login to your existing user account.\n");
    printf("   - Enter your username and password to access your account.\n");
    printf("   - Once logged in, you can start typing tests and view your progress.\n");
    printf("\n3. LOGS:\n");
    printf("   - View your past typing test results in terms of accuracy and typing speed.\n");
    printf("   - The logs show your typing accuracy and words per minute (WPM) for each test.\n");
    printf("\n4. LEADERBOARD:\n");
    printf("   - View the top users based on their typing accuracy.\n");
    printf("   - The leaderboard shows the usernames, accuracy percentage, typing speed, and the time of their last typing test.\n");
    printf("\n5. DELETE USER:\n");
    printf("   - Use this option to delete your user account and all associated records.\n");
    printf("   - You will need to enter your username and password to confirm the deletion.\n");
    printf("   - Once deleted, all data for that user will be permanently removed from the system.\n");
    printf("\n6. CHANGE PASSWORD:\n");
    printf("   - Use this option to change your user account password.\n");
    printf("   - Enter your current password and then provide a new password.\n");
    printf("   - Your password will be updated for future logins.\n");
    printf("\n7. USER MANUAL:\n");
    printf("   - Access this user manual to understand how to use the Typing Tutor program effectively.\n");
    printf("   - This manual explains all the features and functionalities of the program.\n");
    printf("\n8. EXIT:\n");
    printf("   - Use this option to exit the Typing Tutor program.\n");
    printf("\n   DEVELOPERS:\n");
    printf("   - Meet the developers of the Typing Tutor program!\n");
    printf("   - Say hi to @Bhuwan, @Ankush,@Aprpan, and @Shishir for their hard work in creating this program.\n");
    printf("\nEnjoy improving your typing skills with the Typing Tutor program!\n");
    printf("=======================================================================================================================\n");
    fflush(stdin);
    system("pause");
}

void viewTime(){
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);
    int hour = tm_struct->tm_hour;
    int min = tm_struct->tm_min;
    int day = tm_struct->tm_wday;
    if(hour<=12 && hour >= 0)
        // hour = 12 - hour;
    printf("\r");
    printf("\t\t\t\t\t\t\t\t\t   %d : %d %s ", hour,min,(hour>12)?"PM":"AM");
        switch(day){
        case 0 : printf("(Sunday)");
            break;
        case 1 : printf("(Monday)");
            break;
        case 2 : printf("(Tuesday)");
            break;
        case 3 : printf("(Wednesday)");
            break;
        case 4 : printf("(Thursday)");
            break;
        case 5 : printf("(Friday)");
            break;
        case 6 : printf("(Saturday)");
            break;
        default: printf("\n");          
    }
    Sleep(1000);
}
int main() {
    char choice;
    char name[50];
    char pass[50];
    char input[50]; 
    system("color 0A");
    loading();
    system("cls");
    printf("\n\n\n");
    viewTime();
	printf("\n\t\t\t\t\t\t\t=============""====================================""===========");
	printf("\n\t\t\t\t\t\t\t+++++++++++++++++++ PASCHIMANCHAL CAMPUS +++++++++++++++++++");
	printf("\n\t\t\t\t\t\t\t------------------------- BEI 2079 -------------------------");
	printf("\n\t\t\t\t\t\t\t~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~");
	printf("\n\t\t\t\t\t\t\t===================""==============================""===========");
	printf("\n\t\t\t\t\t\t\t++++++++++++++++++++++ TYPING TUTOR ++++++++++++++++++++++++");
	printf("\n\t\t\t\t\t\t\t===================""==============================""===========");
	printf("\n\t\t\t\t\t\t\t~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""~~~~~~~~~~~");
	printf("\n\t\t\t\t\t\t\t=====================""===========================""============\n");
    printf("\n\t\t\t\t\t\t\t\t\t""Developers : @Bhuwan,@Ankush,@Arpan,@Shishir""\n\n\t\t\t\t\t\t\t\t");
	system("pause");
	system("cls");
    deletefiles();
    updateUsers();
    while (1)
     {
        system("cls");
        gotoxy(60,4);
        printf("1. CREATE USER");
        gotoxy(60,6);
        printf("2. LOGIN");
        gotoxy(60,8);
        printf("3. LOGS");
        gotoxy(60,10);
        printf("4. LEADERBOARD");
        gotoxy(60,12);
        printf("5. DELETE USER");
        gotoxy(60,14);
        printf("6. CHANGE PASSWORD");
        gotoxy(60,16);
        printf("7. USER MANUAL");
        gotoxy(60,18);
        printf("8. EXIT");
        gotoxy(60,20);
        printf("Type your choice:");
        gotoxy(60,22);
        scanf(" %c", &choice);
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
            loading();
            if(loginUser(name, pass)){
           time_t now = time(NULL);
            struct tm *tm_struct = localtime(&now);
            int hour = tm_struct->tm_hour;
            if(hour>=0 && hour<12){
                printf("Good morning %s sir!",name);
            }else if (hour>=12 && hour>=17)
            {
                printf("Good Afternoon %s sir!",name);
            }else{
                printf("Good Evening %s sir!",name);
            }
            FILE *fp;
            struct USER u;
            fp = fopen("userlist.bin", "r+");
            while (fread(&u, sizeof(u), 1, fp) == 1)
            {
                if(strcmp(u.name, name)==0){
                    printf("\t\t\t\t\t\tRANK: %d\n",u.rank);
                    break;
                }
            }
            viewSpecificUser(name);
                system("pause");
                openFile(name);
            }else{
                printf("\nLogin failed.\n");
                system("pause");
            }
            break;
        case '3':
            logs();
            break;
        case '4':
            displayLeaderboard();
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
                printf("\nLogin failed.\n");
                system("pause");
            }
            break;
        case '6':
            system("cls");
            char newPassword[50];
            printf("Enter your username:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", name);
            printf("Enter old password:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", pass);
            printf("Enter new password:\n");
            fflush(stdin);
            fgets(input, sizeof(input), stdin); 
            sscanf(input, "%s", newPassword);
            if(changePassword(name, pass, newPassword)==1){
                printf("Dear %s !, Password Changed Successfully\n", name);
            }else{
                printf("\nFailed to change your password.\n");
            }
            system("pause");
            break;
        
        case '7':
            printUserManual();
            break;
        case '8':
            exit(0);
            break;
        
        default:
        printf("Wrong choice!\n");
        }
    }
        return 0;
}
