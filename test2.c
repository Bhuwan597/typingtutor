#include <stdio.h>
#include <windows.h>
#include <time.h> // Add the time.h header for time_t, localtime, and strftime

DWORD WINAPI TimeThread(LPVOID lpParam) {
    while (1) {
        // Get the current time
        time_t currentTime = time(NULL);

        // Convert the time to a string representation
        char timeString[9]; // HH:MM:SS format + null terminator
        struct tm *timeInfo = localtime(&currentTime);
        strftime(timeString, sizeof(timeString), "%H:%M:%S", timeInfo);

        // Clear the console and print the current time
        system("cls");
        printf("Current Time: %s\n", timeString);

        // Wait for a short interval (e.g., 1 second) before updating the time again
        Sleep(1000);
    }
    return 0;
}

int main() {
    // Create a thread for updating and displaying the time
    HANDLE threadHandle = CreateThread(NULL, 0, TimeThread, NULL, 0, NULL);

    // Your main program logic here
    // For demonstration, we'll just run a loop for some time
    for (int i = 0; i < 10; i++) {
        printf("Main loop iteration: %d\n", i);
        Sleep(500);
    }

    // Wait for the thread to finish (you can omit this if the program continues indefinitely)
    WaitForSingleObject(threadHandle, INFINITE);

    return 0;
}
