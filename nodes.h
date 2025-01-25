#ifndef NODES_H
#define NODES_H

#define MAX_USERNAME_SIZE 30
#define MAX_PASSWORD_SIZE 15
#define MAX_CONTENT_SIZE 250

typedef struct user user_t;
typedef struct friend friend_t;
typedef struct post post_t;

// A linked list of users
struct user {
    char username[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    friend_t* friends;
    post_t* posts;
    user_t* next;
};

// A linked list of a user's friends
struct friend {
    char username[MAX_USERNAME_SIZE];
    post_t** posts;
    friend_t* next;
};

// A linked list of a user's posts
struct post {
    char content[MAX_CONTENT_SIZE];
    post_t* next;
};

#endif