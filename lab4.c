#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MENU1_OPTION1 "\n1. Do a practice\n"
#define MENU1_OPTION2 "2. Do a test\n"
#define MENU1_OPTION3 "3. Quit the program\n"

#define MENU2_OPTION1 "\n1. Additions\n"
#define MENU2_OPTION2 "2. Subtractions\n"
#define MENU2_OPTION3 "3. Additions & Subtractions\n"

#define CHOICE_PROMPT "Enter your choice: "

#define PRACTICE 1
#define TEST 2
#define QUIT 3

#define ADDITION 1
#define SUBTRACTION 2
#define MIXED_OPERATIONS 3

#define PRACTICE_QUESTIONS 10
#define TEST_QUESTIONS 15

#define STORE 0
#define PRINT 1

int display_menu1(void);
int display_menu2(int *session_type);
void welcome_message(void);
void print_questions(int n, int format, int *session_type);
void addition(int *session_type);
void subtraction(int *session_type);
char *goodanswer(void);
char *badanswer(void);
void manage_results(int mode, int a, int b, int answer, int correct_answer, char operator);
void practice(int answer, int correct_answer, int a, int b, char operator);
void test(int answer, int correct_answer, int a, int b, char operator);
void session(int *session_type, int answer, int correct_answer, int a, int b, char operator);


int main(void)
{
    int choice1 = 0; // Practice/Test/Exit
    int format = 0;  // Addition/Subtraction/Both
    int question_quantity = 0;

    srand(time(NULL));
    welcome_message();

    while (choice1 != QUIT) {
        choice1 = display_menu1();

        if (choice1 == QUIT) {
            printf("\nGoodbye!\n");
            break;
        }

        if (choice1 != PRACTICE && choice1 != TEST) {
            printf("\nWatch out, read the menu carefully, you can choose 1, 2 or 3.\n");
            continue;
        }

        if (choice1 == PRACTICE)
            question_quantity = PRACTICE_QUESTIONS;
        else
            question_quantity = TEST_QUESTIONS;

        format = display_menu2(&choice1);
        print_questions(question_quantity, format, &choice1);

        if (choice1 == TEST) {
            manage_results(PRINT, 0, 0, 0, 0, ' ');
        }
    }
    return 0;
}

void welcome_message(void)
{
    char name[20];
    printf("Enter your name: ");
    scanf_s("%19s", name);
    printf("\nWelcome, %s!\n", name);
}

int display_menu1()
{
    int choice;

    printf("\nYou can choose:\n");
    printf(MENU1_OPTION1);
    printf(MENU1_OPTION2);
    printf(MENU1_OPTION3);
    printf(CHOICE_PROMPT);
    scanf_s("%d", &choice);
    return choice;
}

int display_menu2(int *session_type)
{
    int choice;

    if (*session_type == PRACTICE)
        printf("\nNow, you can choose to do practices on:\n");
    else
        printf("\nNow, you can choose to do a test on:\n");

    printf(MENU2_OPTION1);
    printf(MENU2_OPTION2);
    printf(MENU2_OPTION3);
    printf(CHOICE_PROMPT);
    scanf_s("%d", &choice);
    return choice;
}

void print_questions(int n, int format, int *session_type)
{
    printf("\nNow, you will be given %d questions to solve:\n", n);

    if (format == ADDITION) {
        for (int i = 1; i <= n; i++) {
            printf("%d. ", i);
            addition(session_type);
        }
    } else if (format == SUBTRACTION) {
        for (int i = 1; i <= n; i++) {
            printf("%d. ", i);
            subtraction(session_type);
        }
    } else if (format == MIXED_OPERATIONS) {
        for (int i = 1; i <= n; i++) {
            printf("%d. ", i);
            if (i % 2 == 0)
                addition(session_type);
            else
                subtraction(session_type);

        }
    }
}

void manage_results(int mode, int a, int b, int answer, int correct_answer, char op)
{
    static int  s_a[TEST_QUESTIONS];
    static int  s_b[TEST_QUESTIONS];
    static int  s_answer[TEST_QUESTIONS];
    static int  s_correct[TEST_QUESTIONS];
    static char s_op[TEST_QUESTIONS];
    static int  count = 0;

    if (mode == STORE) {
        s_a[count] = a;
        s_b[count] = b;
        s_answer[count] = answer;
        s_correct[count] = correct_answer;
        s_op[count] = op;
        count++; // ready for next question
    }
    else if (mode == PRINT) {
        int correct = 0;

        printf("\n----- TEST RESULTS -----\n");
        for (int i = 0; i < count; i++) {
            int is_correct = (s_answer[i] == s_correct[i]); // check scores for test result
            if (is_correct)
                correct++;

            printf("%2d) %d %c %d = %d (you answered %d) [%s]\n", i + 1, s_a[i], s_op[i], s_b[i], s_correct[i], s_answer[i], is_correct ? "CORRECT" : "WRONG");
        }
        
        double percent = 100.0 * correct / count;
        printf("\nScore: %d/%d (%.1f%%)\n", correct, count, percent);

        count = 0;// Reset for next test
    }
}


char *goodanswer(void)
{
    static char *positive_feedback[] = {"Very good!", "Excellent", "Nice Work", 
        "Well done!", "Great", "Keep up the good work!"};
    return positive_feedback[rand() % (sizeof(positive_feedback) / sizeof(char*))];
}

char *badanswer(void)
{
    static char *negative_feedback[] = {"No. Please try again.", "Wrong. Try once again.",
        "Don't give up!", "No. Keep trying.", "Sorry, it is not right"};
    return negative_feedback[rand() % (sizeof(negative_feedback) / sizeof(char*))];
}

void addition(int *session_type)
{
    int a = rand() % 100;
    int b = rand() % (100 - a);
    char operator = '+';
    int answer, correct_answer;

    correct_answer = a + b;
    session(session_type, answer, correct_answer, a, b, operator);
}

void subtraction(int *session_type)
{
    int a = rand() % 100;
    int b = rand() % 100;
    char operator = '-';
    int answer, correct_answer, temp;

    if (b > a) {
        temp = a;
        a = b;
        b = temp;
    }
    correct_answer = a - b;
    session(session_type, answer, correct_answer, a, b, operator);
}
void practice(int answer, int correct_answer, int a, int b, char operator)
{
        do {
        printf("%d %c %d = ", a, operator, b);
        scanf_s("%d", &answer);

        if (answer == correct_answer)
            printf("%s\n", goodanswer());
        else
            printf("%s ", badanswer());
    } while (answer != correct_answer);
}
void test(int answer, int correct_answer, int a, int b, char operator)
{
    printf("%d %c %d = ", a, operator, b);
    scanf_s("%d", &answer);
    manage_results(STORE, a, b, answer, correct_answer, operator);
}
void session(int *session_type, int answer, int correct_answer, int a, int b, char operator)
{
       if (*session_type == PRACTICE) {
        practice(answer, correct_answer, a, b, operator);
    } else { // TEST
        test(answer, correct_answer, a, b, operator);
    } 
}
