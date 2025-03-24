#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define DOT_DURATION 100
#define TONE_FREQ 750

const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char* morse[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",     // A–I
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",   // J–R
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",          // S–Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....",     // 0–5
    "--...", "---..", "----."                                          // 6–9
};


void toggle_key(WORD vk) {
    keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
    keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}


BOOL is_key_on(int vk) {
    return (GetKeyState(vk) & 0x0001) != 0;
}


void flash_caps(int duration_ms) {
    toggle_key(VK_CAPITAL);
    Sleep(duration_ms);
    toggle_key(VK_CAPITAL);
    Sleep(50);
}

HANDLE hConsole;

void play_morse_symbol(char symbol) {
    WORD flash_color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    WORD default_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    int dash_duration = DOT_DURATION * 3;
    int duration = (symbol == '.') ? DOT_DURATION : dash_duration;

    if (symbol == '.' || symbol == '-') {
        SetConsoleTextAttribute(hConsole, flash_color);
        printf("*");
        fflush(stdout);

        Beep(TONE_FREQ, duration);
        flash_caps(duration);

        printf("\b \b");
        fflush(stdout);
        SetConsoleTextAttribute(hConsole, default_color);
        Sleep(DOT_DURATION);
    } else if (symbol == ' ') {
        Sleep(DOT_DURATION * 2);
    } else if (symbol == '/') {
        Sleep(DOT_DURATION * 5);
    }
}

void encode_to_morse(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = toupper(text[i]);

        if (ch == ' ') {
            printf(" / ");
            play_morse_symbol('/');
            continue;
        }

        const char* ptr = strchr(alphabet, ch);
        if (ptr) {
            int index = ptr - alphabet;
            const char* morse_code = morse[index];

            printf("%s ", morse_code);

            for (int j = 0; morse_code[j] != '\0'; j++) {
                play_morse_symbol(morse_code[j]);
            }

            play_morse_symbol(' ');
        } else {
            printf("? ");
        }
    }
    printf("\n");
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char input[100];

    printf("Enter text (A-Z, 0-9): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    printf("Morse code: ");
    encode_to_morse(input);

    return 0;
}
