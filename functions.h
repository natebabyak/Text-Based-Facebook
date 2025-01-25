#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "nodes.h"

/**
 * Creates a new user and adds it to a sorted (in non-decreasing order) linked
 * list at the proper location.
 *
 * Parameters:
 * users: The list of users.
 * username: The new user's username.
 * password: The new user's password.
 *
 * Returns:
 * The head of the linked list.
 */
user_t *add_user(user_t *users, const char *username, const char *password);

/**
 * Searches if the user is available in the database.
 * 
 * Parameters:
 * users: The list of users.
 * username: The username to search for.
 * 
 * Returns:
 * A pointer to the user if found and NULL if not found.
 */
user_t *find_user(user_t *users, const char *username);

/**
 * Creates a new friend's node.
 * 
 * Parameters:
 * users: The list of users.
 * username: The new friend's username.
 * 
 * Returns:
 * The newly created node.
 */
friend_t *create_friend(user_t *users, const char *username);

/**
 * Links a friend to a user. The friend's name is added into a sorted (in
 * non-decreasing order) linked list.
 * 
 * Parameters:
 * users: The list of users.
 * user: The user to add the friend to.
 * friend: The friend's name.
 * 
 * Returns:
 * None
 */
void add_friend(user_t *users, user_t *user, const char *friend);

/**
 * Removes a friend from a user's friend list.
 * 
 * Parameters:
 * user: The user to delete a friend from.
 * friend_name: The friend's name.
 * 
 * Returns:
 * True if the friend was deleted and false otherwise.
 */
_Bool delete_friend(user_t *user, char *friend_name);

/**
 * Creates a new user's post.
 * 
 * Parameters:
 * text: The posts's content.
 * 
 * Returns:
 * The newly created post.
 */
post_t *create_post(const char *text);

/**
 * Adds a post to a user's timeline (following a stack).
 * 
 * Parameters:
 * user: The user to add the post to.
 * text: The post's content.
 * 
 * Returns:
 * None
 */
void add_post(user_t *user, const char *text);

/**
 * Removes a post from a user's list of posts.
 * 
 * Parameters:
 * user: The user to remove a post from.
 * 
 * Returns:
 * True if the post was deleted and false otherwise.
 */
_Bool delete_post(user_t *user);

/**
 * Displays all of a specific user's posts.
 * 
 * Parameters:
 * user: The user to display the posts of.
 * 
 * Returns:
 * None
 */
void display_all_user_posts(user_t *user);

/**
 * Displays all of a specific user's friends.
 * 
 * Parameters:
 * user: The user to display the friends of.
 * 
 * Returns:
 * None
 */
void display_user_friends(user_t *user);

/**
 * Displays a given number of posts for a given user. After displaying the
 * given number of posts, it prompts if you want to display more posts.
 * Continues until there are no more posts or the user types no.
 * 
 * Parameters:
 * user: The user to display the posts of.
 * number: The number of posts to display at a time.
 * 
 * Returns:
 * None
 */
void display_posts_by_n(user_t *users, int number);

/**
 * Frees all users from the database before quitting the application.
 * 
 * Parameters:
 * users: The list of users.
 * 
 * Returns:
 * None
 */
void teardown(user_t *users);

/**
 * Prints the main menu with a lith of option for the user to choose from.
 * 
 * Parameters:
 * None
 * 
 * Returns:
 * None
 */
void print_menu();

/**
 * Reads users from the text file.
 * 
 * Parameters:
 * file: The file to read users from.
 * num_users: The number of users.
 * 
 * Returns:
 * The list of users.
 */
user_t *read_CSV_and_create_users(FILE *file, int num_users);

/*
 * Prompts the user to enter 'Y'or 'N'.
 *
 * Parameters:
 * prompt: The prompt.
 *
 * Returns:
 * True if the user enters 'Y' or 'y'.
 * False if the user enters 'N' or 'n'.
 */
_Bool input_bool(const char *prompt);

/*
 * Prompts the user to enter a friend's username.
 *
 * Parameters:
 * prompt: The prompt.
 * user: The user.
 *
 * Returns:
 * The friend with the username if the friend is found.
 * NULL if the friend is not found.
 */
friend_t *input_friend(const char *prompt, user_t *user);

/*
 * Prompts the user to enter a username.
 *
 * Parameters:
 * prompt: The prompt.
 * users: The users.
 *
 * Returns:
 * The user with the username if the user is found.
 * NULL if the user is not found.
 */
user_t *input_username(const char *prompt, user_t *users);

/*
 * Registers a new user.
 * Creates a new user.
 * Adds the new user to users.
 *
 * Parameters:
 * users: The users.
 *
 * Returns:
 * None
 */
void register_user(user_t *users);

/*
 * Converts a character to lowercase.
 *
 * Parameters:
 * c: The character.
 *
 * Returns:
 * The lowercase character.
 */
char char_to_lower(char c);

/*
 * Converts a string to lowercase.
 *
 * Parameters:
 * str: The string.
 *
 * Returns:
 * The lowercase string.
 */
char *str_to_lower(char *str);

/*
 * Case insensitve implementation of strcmp.
 *
 * Parameters:
 * str1: The first string.
 * str2: The second string.
 *
 * Returns:
 * < 0 if str1 is before str2 alphabetically.
 * > 0 if str2 is before str1 alphabetically.
 * 0 if str1 is the same as str2.
 */
int case_insensitive_strcmp(const char *str1, const char *str2);

/**
 * Prints the logged in menu.
 * 
 * Parameters:
 * username: The logged in user's username.
 */
void print_logged_in_menu(const char *username);

/**
 * Prompts the user to enter an unsigned short integer between min and max inclusive.
 * 
 * Parameters:
 * prompt: The prompt.
 * min: The minimum allowed input.
 * max: The maximum allowed input.
 * 
 * Returns:
 * The unsigned short integer.
 */
unsigned short input_unsigned_short_between(const char *prompt, const unsigned short min, const unsigned short max);

/**
 * Prompts the user to enter their password.
 *
 * Parameters:
 * prompt: The prompt.
 * password: The password.
 *
 * Returns:
 * True if the passwords match.
 * False if the passwords do not match.
 */
_Bool input_password(const char *prompt, const char *password);

/**
 * User's profile menu.
 * 
 * Parameters:
 * user: The users.
 * username: The logged in user's username.
 * 
 * Returns:
 * None
 */
void manage_user(user_t *users, const char *username);

/**
 * User's posts menu.
 * 
 * Parameters
 * user: The users
 * username: The logged in user's username.
 * 
 * Returns:
 * None
 */
void manage_posts(user_t *users, const char *username);

/**
 * User's friends menu.
 * 
 * Parameters
 * user: The users
 * username: The logged in user's username.
 * 
 * Returns:
 * None
 */
void manage_friends(user_t *users, const char *username);

/**
 * Logged in menu.
 * 
 * Parameters
 * users: The users
 * user: The logged in user's username.
 * 
 * Returns
 * None
 */
void logged_in_menu(user_t *users, const char *username);

/**
 * Main menu.
 * 
 * Parameters
 * users: The users.
 * 
 * Returns:
 * None
*/
void main_menu(user_t *users);

/**
 * Displays a user's friend's posts.
 * 
 * Parameters:
 * users: The users
 * username: The user's username.
 * 
 * Returns:
 * None
*/
void display_friends_posts(user_t *users, const char *username);

/**
 * Prints a horizontal rule.
 * 
 * Parameters:
 * None
 * 
 * Returns:
 * None
*/
void hr(void);

#endif