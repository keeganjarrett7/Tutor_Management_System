/* This project extends my project 8 tutor list by adding two new major functions which improve how the tutors are stored in the linked list.
Name: Keegan Jarrett */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EMAIL_LEN 100
#define NAME_LEN 30
#define LEVEL_LEN 30

// node for the linked list of tutors
struct tutor{
    char first[NAME_LEN+1];
    char last[NAME_LEN+1];
    char email[EMAIL_LEN+1];
    int preferences[3];        // 0/1 for elementary, middle, high
    struct tutor *next;        // pointer to next tutor
};

// function prototypes from the assignment
struct tutor *add_to_list(struct tutor *list);
struct tutor *delete_from_list(struct tutor *list);
void search_list(struct tutor *list);
void print_list(struct tutor *list);
void clear_list(struct tutor *list);
int read_line(char str[], int n);

int main(void)
{
    char code;
    struct tutor *tutor_list = NULL;   // start with empty list

    printf("Operation Code: a for adding to the list, s for searching, d for deleting from the list, p for printing the list; q for quit.\n");

    for (;;) {
        printf("Enter operation code: ");
        if (scanf(" %c", &code) != 1) {
            break;
        }

        // clear rest of line so read_line works correctly
        while (getchar() != '\n')
            ;

        if (code == 'q') {
            // free all memory and exit
            clear_list(tutor_list);
            return 0;
        }

        switch (code) {
        case 'a':
            tutor_list = add_to_list(tutor_list);
            break;
        case 'd':
            tutor_list = delete_from_list(tutor_list);
            break;
        case 's':
            search_list(tutor_list);
            break;
        case 'p':
            print_list(tutor_list);
            break;
        default:
            printf("Illegal code\n");
            break;
        }

        printf("\n");
    }

    // just in case we ever drop out of loop
    clear_list(tutor_list);
    return 0;
}

// adds a new tutor to the list, keeping nodes in order by last and first name
struct tutor *add_to_list(struct tutor *list)
{
    char first[NAME_LEN+1];
    char last[NAME_LEN+1];
    char email[EMAIL_LEN+1];
    int p0, p1, p2;
    struct tutor *cur;
    struct tutor *new_node;

    // get info from user (matches the prompt)
    printf("Enter last name: ");
    read_line(last, NAME_LEN);

    printf("Enter first name: ");
    read_line(first, NAME_LEN);

    printf("Enter email address: ");
    read_line(email, EMAIL_LEN);

    printf("Enter preferences: ");
    scanf("%d %d %d", &p0, &p1, &p2);

    // clear leftover newline from scanf so read_line still works later
    while (getchar() != '\n')
        ;

    // check for duplicate (same last name AND same email)
    cur = list;
    while (cur != NULL) {
        if (strcmp(cur->last, last) == 0 &&
            strcmp(cur->email, email) == 0) {
            printf("tutor already exists.\n");
            return list;   // do not add a duplicate
        }
        cur = cur->next;
    }

    // make a new node
    new_node = malloc(sizeof(struct tutor));
    if (new_node == NULL) {
        // simple error message if memory fails
        printf("malloc failed\n");
        return list;
    }

    // copy data into the new node
    strcpy(new_node->first, first);
    strcpy(new_node->last, last);
    strcpy(new_node->email, email);
    new_node->preferences[0] = p0;
    new_node->preferences[1] = p1;
    new_node->preferences[2] = p2;

    // insert into the list in sorted order by last then first name
    if (list == NULL) {
        new_node->next = NULL;
        return new_node;
    }

    struct tutor *prev = NULL;
    cur = list;

    while (cur != NULL) {
        int last_cmp = strcmp(new_node->last, cur->last);

        if (last_cmp < 0) {
            // new last name comes before current last name
            break;
        } else if (last_cmp == 0) {
            int first_cmp = strcmp(new_node->first, cur->first);
            if (first_cmp < 0) {
                // same last name, but new first comes before current first
                break;
            }
            // if first_cmp >= 0, move forward so same last+first go after existing
        }

        prev = cur;
        cur = cur->next;
    }

    if (prev == NULL) {
        // go at beginning
        new_node->next = list;
        list = new_node;
    } else {
        // go between prev and cur (may be end if cur == NULL)
        new_node->next = cur;
        prev->next = new_node;
    }

    return list;
}

// removes a tutor node matching last name, first name, and email
struct tutor *delete_from_list(struct tutor *list)
{
    char first[NAME_LEN+1];
    char last[NAME_LEN+1];
    char email[EMAIL_LEN+1];
    struct tutor *cur, *prev;

    if (list == NULL) {
        // list is empty, nothing to delete
        printf("tutor does not exist");
        return NULL;
    }

    printf("Enter last name: ");
    read_line(last, NAME_LEN);

    printf("Enter first name: ");
    read_line(first, NAME_LEN);

    printf("Enter email address: ");
    read_line(email, EMAIL_LEN);

    cur = list;
    prev = NULL;

    while (cur != NULL) {
        if (strcmp(cur->last, last) == 0 &&
            strcmp(cur->first, first) == 0 &&
            strcmp(cur->email, email) == 0) {

            // found node to delete
            if (prev == NULL) {
                // deleting first node
                list = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            printf("tutor deleted");
            return list;
        }

        prev = cur;
        cur = cur->next;
    }

    // not found
    printf("tutor does not exist");
    return list;
}

// searches for tutors who can teach a certain level
void search_list(struct tutor *list)
{
    char level[LEVEL_LEN+1];
    int pref_index;
    int found = 0;
    struct tutor *cur;

    printf("Enter level: ");
    read_line(level, LEVEL_LEN);

    // decide which index in preferences[] to look at
    if (strcmp(level, "elementary") == 0)
        pref_index = 0;
    else if (strcmp(level, "middle") == 0)
        pref_index = 1;
    else if (strcmp(level, "high") == 0)
        pref_index = 2;
    else {
        // not a valid level string
        printf("not found\n");
        return;
    }

    // go through the whole list
    cur = list;
    while (cur != NULL) {
        if (cur->preferences[pref_index] == 1) {
            // print last name, first name, and email only
            printf("%s %s %s\n", cur->last, cur->first, cur->email);
            found = 1;
        }
        cur = cur->next;
    }

    if (!found) {
        printf("not found\n");
    }
}

// prints every tutor in the list and their preferences
void print_list(struct tutor *list)
{
    struct tutor *cur = list;

    while (cur != NULL) {
        // formatted one-line print to match project output
        printf("%-12s %-12s %-30s %4d %4d %4d\n",
               cur->last,
               cur->first,
               cur->email,
               cur->preferences[0],
               cur->preferences[1],
               cur->preferences[2]);
        cur = cur->next;
    }
}

// frees all nodes in the list so there are no memory leaks
void clear_list(struct tutor *list)
{
    struct tutor *cur;

    while (list != NULL) {
        cur = list;        // remember current node
        list = list->next; // move head to next node
        free(cur);         // free the old head
    }
}

// read_line function from the prompt
int read_line(char str[], int n)
{
  int ch, i = 0;

  while (isspace(ch = getchar()))
    ;
  str[i++] = ch;
  while ((ch = getchar()) != '\n') {
    if (i < n)
      str[i++] = ch;

   }
   str[i] = '\0';
   return i;
}
