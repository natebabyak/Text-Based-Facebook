#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "nodes.h"
#include "functions.h"

int main() {
    FILE *csv_file = fopen("users.csv", "r");

    if (csv_file == NULL) {
        perror("Error opening the CSV file");

        return 1;
    }
    
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);

    printf("Welcome to Text-Based Facebook\n");
 
    main_menu(users);

    teardown(users);

    return EXIT_SUCCESS;
}