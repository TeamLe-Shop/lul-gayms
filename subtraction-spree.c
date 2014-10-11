#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <stdbool.h>

/* Give the greeting message,
 * for when they first open the program.
 */
void greet(void);

int main(void)
{
    int score = 0;
    int num_x, num_y, num_z;
    const int question_count = 25;
    int question_num;

    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

    int answer;

    char input[100];

    /* Get a new seed for our random number generator. */
    srand(time(0));

    greet();

    for (question_num = 0; question_num < question_count; question_num++) {
        num_x = (rand() % 30) + 10;
        num_y = (rand() % 18);

        printf("Q%d) What is %d - %d?\n>", question_num + 1, num_x, num_y);

        if (poll(&mypoll, 1, 5000)) {
            scanf("%100s", input);
        } else {
            printf(" *** Too slow!\n");
            continue;
        }

        answer = strtol(input, NULL, 10);
        if (errno == EINVAL) {
            printf(" &&& Not a valid number!\n");
            errno = 0;
            question_num--;
            continue;
        }

        if (answer == num_x - num_y) {
            printf(" *** Correct! [ +1 POINT ]\n");
            score++;
        } else {
            printf(" *** Nope!\n");
        }
    }

    printf(" *** TOTAL SCORE: %d\n", score);

    if (score < 8) {
        printf("\tUrp. Looks like you had trouble with those.\n");
        printf("\t\t\t\t TRY AGAIN\n");
    } else if (score < 15) {
        printf("\tYou did okay.\n");
        printf("\t\t\t\t O.K.\n");
    } else if (score < 25) {
        printf("\tWell done, well done!\n");
        printf("\t\t\t\t SUPERB\n");
    } else {
        printf("\tLooks like you NAILED IT!\n");
        printf("\t\t\t\t ** PERFECT **\n");
    }

    return 0;
}

void greet(void)
{
    printf("\n             -- SUBTRACTION SPREE --\n\n ");
    printf("         Welcome to drill \"Subtraction Spree\".\n");
    printf("  In this one, you'll have to attempt to subtract two or\n");
    printf("  more numbers together within a certain time limit.\n\n");
    printf("                  Good luck!\n\n");
}
