#include <stdio.h> 
double calculate_final_score(double s1, double s2, double s3, double s4, double s5, double difficulty);

void print_final_score(double final_score);

double read_double(char prompt[]);

int main(void)
{
    double score1 = 0.0;
    double score2 = 0.0;
    double score3 = 0.0;
    double score4 = 0.0;
    double score5 = 0.0;
    double difficulty = 0.0;
    double final_score = 0.0;

    //Read score from user
    difficulty = read_double("What was the difficulty of the dive ");
    score1 = read_double("Score 1");
    score2 = read_double("Score 2");
    score3 = read_double("Score 3");
    score4 = read_double("Score 4");
    score5 = read_double("Score 5");

    //Cakculate scores
    final_score = calculate_final_score(score1, score2, score3, score4, score5, difficulty);

    //Print score
    print_final_score(final_score);

    return 0;
}

double calculate_final_score(double s1, double s2, double s3, double s4, double s5, double difficulty)
{

    double result = 0.0;
    //find Highest score
    double max = s1;
    if (s2 > max) max = s2;
    if (s3 > max) max = s3;
    if (s4 > max) max = s4;
    if (s5 > max) max = s5;
    //find Lowest score
    double min = s1;
    if (s2 < min) min = s2;
    if (s3 < min) min = s3;
    if (s4 < min) min = s4;
    if (s5 < min) min = s5;

    result = ((s1 + s2 + s3 + s4 + s5) - (min + max)) * difficulty; //Discard higjest & lowest score and multiply by difficulty

    return result;
}


double read_double(char prompt[])
{
    double user_input;
    printf("%s: ", prompt);
    scanf("%lf", &user_input);

    return user_input;
}

void print_final_score(double final_score)
{
    printf("Divers score was: %.2f\n", final_score);
}

//EOF
