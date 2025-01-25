#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "nodes.h"
#include "functions.h"

#define MAX_USERNAME_SIZE 30
#define MAX_PASSWORD_SIZE 15
#define MAX_POST_SIZE 250

user_t *add_user(user_t *users, const char *username, const char *password) {
    user_t *new_user = malloc(sizeof(user_t));
    assert(new_user != NULL);
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->friends = NULL;
    new_user->posts = NULL;
    new_user->next = NULL;
    if (users == NULL) {
        users = new_user;
    } else {
        user_t *current = users;
        while (current->next != NULL && strcmp(current->next->username, username) < 0) current = current->next;
        new_user->next = current->next;
        current->next = new_user;
    }
    return users;
}

user_t *find_user(user_t *users, const char *username) {
    user_t *current = users;
    while (current != NULL) {
        if (!case_insensitive_strcmp(current->username, username)) return current;
        current = current->next;
    }
    return NULL;
}

friend_t *create_friend(user_t *users, const char *username) {
    friend_t *new_friend = malloc(sizeof(friend_t));
    assert (new_friend != NULL);
    strcpy(new_friend->username, username);
    user_t *user = find_user(users, username);
    if (user == NULL) {
        free(new_friend);
        return NULL;
    }
    new_friend->posts = &user->posts;
    new_friend->next = NULL;
    return new_friend;
}

void add_friend(user_t *users, user_t *user, const char *friend) {
    friend_t *new_friend = create_friend(users, friend);
    if (new_friend == NULL) return;
    if (user->friends == NULL || strcmp(user->friends->username, friend) > 0) {
        new_friend->next = user->friends;
        user->friends = new_friend;
        return;
    }
    friend_t *current = user->friends;
    while (current->next != NULL && strcmp(current->next->username, friend) < 0) current = current->next;
    new_friend->next = current->next;
    current->next = new_friend;
}

_Bool delete_friend(user_t *user, char *friend_name) {
    if (user->friends == NULL) return false;
    if (strcmp(user->friends->username, friend_name) == 0) {
        friend_t *to_delete = user->friends;
        user->friends = user->friends->next;
        free(to_delete);
        return true;
    }
    friend_t *current = user->friends;
    while (current->next != NULL && strcmp(current->next->username, friend_name) != 0) current = current->next;
    if (current->next == NULL) return false;
    friend_t *to_delete = current->next;
    current->next = to_delete->next;
    free(to_delete);
    return true;
}

post_t *create_post(const char *text) {
    post_t *new_post = malloc(sizeof(post_t));
    assert(new_post != NULL);
    strcpy(new_post->content, text);
    new_post->next = NULL;
    return new_post;
}

void add_post(user_t *user, const char *text) {
    post_t *new_post = create_post(text);
    if (user->posts == NULL) {
        user->posts = new_post;
    } else {
        new_post->next = user->posts;
        user->posts = new_post;
    }
}

_Bool delete_post(user_t *user) {
    if (user->posts == NULL) return false;
    post_t *to_delete = user->posts;
    user->posts = to_delete->next;
    free(to_delete);
    return true;
}

void display_all_user_posts(user_t *user) {
    hr();
    printf("%s's Posts:\n", user->username);
    hr();
    if (user->posts == NULL) printf("No posts available for %s.\n", user->username);
    post_t *current = user->posts;
    while (current != NULL) {
        printf("%s\n", current->content);
        current = current->next;
    }
}

void display_user_friends(user_t *user) {
    hr();
    printf("%s's Friends:\n", user->username);
    hr();
    if (user->friends == NULL) printf("No friends available for %s.\n", user->username);
    friend_t *current = user->friends;
    for (int i = 1; current != NULL; i++) {
        printf("%d. %s\n", i, current->username);
        current = current->next;
    }
    printf("\n");
}

void display_posts_by_n(user_t *user, int number) {
    hr();
    printf("%s's Posts:\n", user->username);
    hr();
    _Bool exit = false;
    post_t *current = user->posts;
    while (!exit) {
        for (int i = 0; i < number && current != NULL; i++) {
            printf("%s\n", current->content);
            current = current->next;
        }
        if (current == NULL) {
            printf("All posts have been displayed.\n");
            return;
        }
        exit = !input_bool("Do you want to display more posts? (Y/N)\n\nEnter your choice: ");
    }
}

void teardown(user_t *users) {
    while (users != NULL) {
        user_t *user_to_delete = users;
        while (user_to_delete->friends != NULL) {
            friend_t *friend_to_delete = user_to_delete->friends;
            user_to_delete->friends = user_to_delete->friends->next;
            free(friend_to_delete);
        }
        while (user_to_delete->posts != NULL) {
            delete_post(user_to_delete);
        }
        users = users->next;
        free(user_to_delete);
    }
}

void print_menu() {
    hr();
    printf("Main Menu:\n");
    hr();
    printf("1. Register a new user\n"
           "2. Login with existing user's information\n"
           "3. Exit\n\n");
}

user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(users, current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}

_Bool input_bool(const char *prompt) {
    char input = false;
	_Bool valid = false;
	while (!valid) {
		printf("%s", prompt);
		scanf(" %c", &input);
		if (input != 'Y' && input != 'y' && input != 'N' && input != 'n') {
			printf("The input must be 'Y' for Yes or 'N' for No.\n");
		} else {
			valid = true;
		}
	}
	return input == 'Y' || input == 'y';
}

friend_t *input_friend(const char *prompt, user_t *user) {
    char username[MAX_USERNAME_SIZE];
    printf("%s", prompt);
    scanf("%s", username);
    friend_t *current = user->friends;
    while (current != NULL) {
        if (!strcmp(current->username, username)) return current;
        current = current->next;
    }
    printf("This user is not on your friends list.\n");
    return NULL;
}

user_t *input_username(const char *prompt, user_t *users) {
    char username[MAX_USERNAME_SIZE];
    printf("%s", prompt);
    scanf("%s", username);
    user_t *user = find_user(users, username);
    if (user == NULL) printf("User not found.\n");
    return user;
}

void register_user(user_t *users) {
    hr();
    printf("Registering a new user:\n");
    hr();
    char username[MAX_USERNAME_SIZE];
    printf("Enter a username: ");
    scanf("%s", username);
    user_t *current = users;
    while (current != NULL) {
        if (case_insensitive_strcmp(current->username, username) == 0) {
            printf("That username is already in use.\n");
            return;
        }
        current = current->next;
    }
    char password[MAX_PASSWORD_SIZE];
    _Bool valid = false;
    while (!valid) {
        printf("Enter an up to 15 characters password: ");
        scanf("%s", password);
        if (strlen(password) < 8) {
            printf("The length must be at least eight characters.\n");
        } else {
            valid = true;
        }
    }
    add_user(users, str_to_lower(username), password);
    printf("User added.\n");
}

char char_to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? 'a' + (c - 'A') : c;
}

char *str_to_lower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = char_to_lower(str[i]);
    }
    return str;
}

int case_insensitive_strcmp(const char *str1, const char *str2) {
    int i = 0;
    for (; str1[i] != '\0' && str2[i] != '\0'; i++) {
        int diff = char_to_lower(str1[i]) - char_to_lower(str2[i]);
        if (diff != 0) return diff;
    }
    return char_to_lower(str1[i]) - char_to_lower(str2[i]);
}

void print_logged_in_menu(const char *username) {
    hr();
    printf("Welcome %s:\n", username);
    hr();
    printf("1. Manage profile (change password)\n"
           "2. Manage posts (add/remove)\n"
           "3. Manage friends (add/remove)\n"
           "4. Display a friend's posts\n"
           "5. Exit\n\n");
}

unsigned short input_unsigned_short_between(const char *prompt, const unsigned short min, const unsigned short max) {
    unsigned short input = 0;
	_Bool valid = false;
	while (!valid) {
		printf("%s", prompt);
		scanf("%hu", &input);
		if (input < min || input > max) {
			printf("The input must be between %hu and %hu inclusive.\n\n", min, max);
		} else {
			valid = true;
		}
	}
	return input;
}

_Bool input_password(const char *prompt, const char *password) {
    char guess[MAX_PASSWORD_SIZE];
    printf("%s", prompt);
    scanf("%s", guess);
    if (strcmp(password, guess) != 0) printf("Incorrect password.\n");
    return strcmp(password, guess) == 0;
}

void manage_user(user_t *users, const char *username) {
    user_t *user = find_user(users, username);
    hr();
    printf("Managing %s's Profile:\n", user->username);
    hr();
    if (input_password("Enter your password: ", user->password)) {
        char new_password[MAX_PASSWORD_SIZE];
        printf("Enter a new password up to 15 characters: ");
        scanf("%s", new_password);
        strcpy(user->password, new_password);
        printf("Password changed.\n");
    }
}

void manage_posts(user_t *users, const char *username) {
    user_t *user = find_user(users, username);
    _Bool exit = false;
    while (!exit) {
        hr();
        printf("Managing %s's Posts:\n", user->username);
        hr();
        if (user->posts == NULL) printf("No posts available for %s.\n", user->username);
        printf("1. Add a new post\n"
               "2. Remove a post\n"
               "3. Return to main menu\n\n");
        switch (input_unsigned_short_between("Enter your choice: ", 1, 3)) {
            case 1:
                char post_content[MAX_POST_SIZE];
                printf("Enter your post content: ");
                scanf(" %[^\n]s", post_content);
                add_post(user, post_content);
                display_all_user_posts(user);
                break;
            case 2:
                if (delete_post(user)) {
                    display_all_user_posts(user);
                } else {
                    printf("There were no posts to be deleted.\n");
                }
                break;
            case 3:
                exit = true;
        }
    }
}

void manage_friends(user_t *users, const char *username) {
    user_t *user = find_user(users, username);
    _Bool exit = false;
    while (!exit) {
        hr();
        printf("Managing %s's Friends:\n", user->username);
        hr();
        if (user->friends == NULL) printf("No friends available for %s.\n", user->username);
        printf("1. Add a new friend\n"
               "2. Remove a friend\n"
               "3. Return to main menu\n\n");
        switch (input_unsigned_short_between("Enter your choice: ", 1, 3)) {
            case 1:
                char new_friend_name[MAX_USERNAME_SIZE];
                printf("Enter a new friend's name: ");
                scanf("%s", new_friend_name);
                user_t *new_friend = find_user(users, new_friend_name);
                if (new_friend == NULL) {
                    printf("User not found.\n");
                    break;
                }
                add_friend(users, user, new_friend_name);
                printf("Friend added to the list.\n");
                break;
            case 2:
                display_user_friends(user);
                if (user->friends == NULL) return;
                char friend_to_delete_name[MAX_USERNAME_SIZE];
                printf("Enter a friend's name to delete: ");
                scanf("%s", friend_to_delete_name);
                if (delete_friend(user, friend_to_delete_name)) {
                    printf("Friend deleted.\n");
                } else {
                    printf("Invalid friend's name.\n");
                }
                break;
            case 3:
                exit = true;
        }
    }
}

void logged_in_menu(user_t *users, const char *username) {
    _Bool exit = false;
    while (!exit) {
        print_logged_in_menu(username);
        switch (input_unsigned_short_between("Enter your choice: ", 1, 5)) {
            case 1:
                manage_user(users, username);
                break;
            case 2:
                manage_posts(users, username);
                break;
            case 3:
                manage_friends(users, username);
                break;
            case 4:
                display_friends_posts(users, username);
                break;
            case 5:
                exit = true; 
        }
    }
}

void main_menu(user_t *users) {
    _Bool exit = false;
    while (!exit) {
        print_menu();
        switch (input_unsigned_short_between("Enter your choice: ", 1, 3)) {
            case 1:
                register_user(users);
                break;
            case 2:
                user_t *user = input_username("Enter your username: ", users);
                if (user == NULL) break;
                if (input_password("Enter your password: ", user->password)) logged_in_menu(users, user->username);
                break;
            case 3:
                printf("Goodbye.\n");
                exit = true;
        }
    }
}

void display_friends_posts(user_t *users, const char *username) {
    user_t *user = find_user(users, username);
    if (user == NULL) return;
    friend_t *friend = input_friend("Enter your friend's username: ", user);
    if (friend == NULL) return;
    hr();
    printf("%s's Posts:\n", friend->username);
    hr();
    _Bool exit = false;
    post_t *current = *friend->posts;
    while (!exit) {
        for (int i = 0; i < 3 && current != NULL; i++) {
            printf("%s\n", current->content);
            current = current->next;
        }
        if (current == NULL) {
            printf("All posts have been displayed.\n");
            return;
        }
        exit = !input_bool("Do you want to display more posts? (Y/N)\n\n"
                           "Enter your choice: ");
    }
}

void hr(void) {
    printf("================================================================================\n");
}