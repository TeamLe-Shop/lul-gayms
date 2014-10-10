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

    int penalty_stack = 1;

    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

    int answer;

    char input[100];

    /* Get a new seed for our random number generator. */
    srand(time(0));

    greet();

    for (question_num = 0; question_num < question_count; question_num++) {
        num_x = (rand() % 50) + 10;
        num_y = (rand() % 18);
        if (penalty_stack > 1) num_z = (rand() % (34 * penalty_stack)) + 1;
        else num_z = (rand() % 6) + 1;

        printf("Q%d) What is %d - %d - %d?\n>", question_num + 1, num_x, num_y, num_z);

        if (poll(&mypoll, 1, 5250)) {
            scanf("%s", input);
        } else {
            printf(" *** Too slow!\n");
            score -= penalty_stack;
            penalty_stack++;
            continue;
        }

        answer = strtol(input, NULL, 10);
        if (errno == EINVAL) {
            printf(" &&& Not a valid number!\n");
            errno = 0;
            question_num--;
            continue;
        }

        if (answer == num_x - num_y - num_z) {
            printf(" *** Correct! [ +2 POINT ]\n");
            score+=2;
            penalty_stack=1;
        } else {
            printf(" *** Nope!    [ -%d POINT(S) ]\n", penalty_stack);
            score-=penalty_stack;
            penalty_stack++;
        }
    }

    printf(" *** TOTAL SCORE: %d / 50\n", score);

    if (score < 16) {
        printf("\tUrp. Looks like you had trouble with those.\n");
        printf("\t\t\t\t TRY AGAIN\n");
    } else if (score < 30) {
        printf("\tYou did okay.\n");
        printf("\t\t\t\t O.K.\n");
    } else if (score < 50) {
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
    printf("\n             -- SUBTRACTION SPREE 2 --\n\n ");
    printf("         Welcome to drill \"Subtraction Spree 2\".\n");
    printf("  In this one, you'll have to attempt to subtract two or\n");
    printf("  more numbers together within a certain time limit.\n\n");
    printf("     This one is a little more difficult.\n\n");
    printf("                  Good luck!\n\n");
}
