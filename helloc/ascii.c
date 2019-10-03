#include <stdio.h>

int main()
{
    int i;

    printf("|ASCII Table (31-126)|c\n");
    printf("|!code|!char|\n");
    for (i=31; i<127; ++i)
        printf("|%4d | '%c' |\n", i, i);
}
