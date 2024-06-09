//! BookBase project from INHA unoversity students
// 1. Zhanybekova Saikal, SID: 12235621
// 2. Shamshieva Merim, SID: 12235612


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_USERS 100
#define MAX_BOOKS 100
#define MAX_STR_LEN 100

//! USER STRUCTURE
typedef struct {
    char first_name[50];
    char last_name[50];
    char email[50];
    char username[30];
    char password[30];
    char passwordCheck[30];
    char birthdate[11];
}User;


//! BOOK STRUCTURE
typedef struct {
    char title[MAX_STR_LEN];
    char author[MAX_STR_LEN];
    int year;
}Book;


//! USER ACCOUNT STRUCTURE
typedef struct {
    User user;

    Book favBooks[MAX_BOOKS];
    int favBooksCount;

    Book readedBooks[MAX_BOOKS];
    int readedBooksCount;

    Book toReadBooks[MAX_BOOKS];
    int toReadBooksCount;

}UserAccount;

UserAccount UserAccounts[MAX_USERS];
int userCount=0;

//! FUNCTIONS LIST
void SignUp();
int LogIn();
void ViewPersonalAccount(UserAccount *account);
void ViewFavBooksList(UserAccount *account);
void AddToFavorites(UserAccount *account);
void ViewReadedBooksList(UserAccount *account);
void MarkAsReaded(UserAccount *account);
void ViewToReadBooksList(UserAccount *account);
void MarkToReadLater(UserAccount *account);
void capitalizeFirstLetter(char *str) {
    if (str[0] != '\0') {
        str[0] = toupper(str[0]);
    }
}
void saveUserData();
void loadUserData();
void saveBooksData();
void loadBooksData();


//! SAVE AND LOAD USER DATA START
void saveUserData() {
    FILE *usersFile = fopen("users.dat", "wb");
    if (usersFile == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
//? fwrite(pointer_to_data, size_of_each_element in bytes, number_of_elements that i want to write in file, file_pointer);
    fwrite(&userCount, sizeof(int), 1, usersFile);
    fwrite(UserAccounts, sizeof(UserAccount), userCount, usersFile);

    fclose(usersFile);
}

void loadUserData() {
    FILE *usersFile = fopen("users.dat", "rb");
    if (usersFile == NULL) {
        printf("No existing user data found.\n");
        return;
    }

    fread(&userCount, sizeof(int), 1, usersFile);
    fread(UserAccounts, sizeof(UserAccount), userCount, usersFile);

    fclose(usersFile);
}
//! SAVE AND LOAD USER DATA END

//! SAVE AND LOAD BOOK DATA START

void saveBooksData(UserAccount *account, char *bookfile, Book *books, int count) {
    FILE *booksFile = fopen(bookfile, "wb");
    if (booksFile == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, booksFile);
    fwrite(books, sizeof(Book), count, booksFile);

    fclose(booksFile);
}

void loadBooksData(UserAccount *account, char *bookfile, Book *books, int *count) {
    FILE *booksFile = fopen(bookfile, "rb");
    if (booksFile == NULL) {
        printf("No existing book data found.\n");
        return;
    }

    fread(count, sizeof(int), 1, booksFile);
    fread(books, sizeof(Book), *count, booksFile);

    fclose(booksFile);
}
//! SAVE AND LOAD BOOK DATA END



//! SIGNUP FUNCTION START
void SignUp(){
    if(userCount>=MAX_USERS){
        printf("User limit reached\n");
        return;
    }

    UserAccount newUserAccount;
    printf("Please enter the information required\n\n");

    printf("Enter first name: ");
    scanf(" %49s", newUserAccount.user.first_name);
    capitalizeFirstLetter(newUserAccount.user.first_name);

    printf("Enter last name: ");
    scanf("%49s", newUserAccount.user.last_name);
    capitalizeFirstLetter(newUserAccount.user.last_name);
    while (1) {
        printf("Enter email: ");
        scanf("%49s", newUserAccount.user.email);

        int emailExists = 0;
        for (int i = 0; i < userCount; ++i) {
            if (strcmp(newUserAccount.user.email, UserAccounts[i].user.email) == 0) {
                emailExists = 1;
                break;
            }
        }

        if (emailExists) {
            printf("\tEmail already exists. Please use a different one\n");
        } else {
            break;
        }
    }

    while (1) {
    printf("Enter username (max 30): ");
    scanf("%29s", newUserAccount.user.username);

    int usernameExists = 0;
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(newUserAccount.user.username, UserAccounts[i].user.username) == 0) {
            usernameExists = 1;
            break;
        }
    }

    if (usernameExists) {
        printf("\tUsername already exists. Please choose a different one\n");
    } else {
        break;
    }
}

    while (1) {
        printf("Enter password (at least 8 characters): ");
        scanf("%29s", newUserAccount.user.password);

        if (strlen(newUserAccount.user.password) < 8) {
            printf("\tPassword is too short, please enter at least 8 characters\n");
        } else {
            break;
        }
    }

    printf("Enter password again: ");
    scanf("%29s", newUserAccount.user.passwordCheck);
    if (strcmp(newUserAccount.user.password, newUserAccount.user.passwordCheck)!=0){ //? !=0 because for strcmp 0=same, 1 first more than second, -1 first less then second
        printf("\n\tPasswords do not match. Please try again\n");
        return;
    }

    do {
        printf("Enter birthdate (DD.MM.YYYY): ");
        scanf("%10s", newUserAccount.user.birthdate);
        if (strlen(newUserAccount.user.birthdate) !=10 ||
            newUserAccount.user.birthdate[2] != '.' ||
            newUserAccount.user.birthdate[5] != '.') {
            printf("\tBirthdate must be in the format DD.MM.YYYY\n");
        }
    }
    while (strlen(newUserAccount.user.birthdate) !=10 ||
            newUserAccount.user.birthdate[2] != '.' ||
            newUserAccount.user.birthdate[5] != '.');

//? Assign the values for fav readed and to read books as it is new user
    newUserAccount.favBooksCount = 0;
    newUserAccount.readedBooksCount = 0;
    newUserAccount.toReadBooksCount = 0;

//? increment the user count and display message
    UserAccounts[userCount++] = newUserAccount;
    printf("\n\tRegistration Successful!");
}
//! SIGNUP FUNCTION END



//! LOGIN FUNCTION START
int LogIn(){
    char username[30];
    char password[30];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%30s", password);

    for (int i=0; i<userCount; i++){
        if(strcmp(UserAccounts[i].user.username, username) ==0 &&
        strcmp(UserAccounts[i].user.password, password) ==0){
            printf("\n\tLogin Successful!");
            printf("\n\nWelcome to Book Base %s!\nFor Menu press 0\n ", username);
            return i;        //? indicate which position in the array(userAccounts) corresponds to the user account that successfully login
        }
    }
    printf("\n\tInvalid username or password\n");
    return -1;
}
//! LOGIN FUNCTION END



// ! DELETE ACCOUNT START
void deleteAccount(UserAccount *account) {
    loadUserData();

    int loggedInUserIndex = -1;
    for (int i = 0; i < userCount; ++i) {
        if (&UserAccounts[i] == account) {
            loggedInUserIndex = i;
            break;
        }
    }
    if (loggedInUserIndex != -1) {
//? shifting and deleting
        for (int i = loggedInUserIndex; i < userCount - 1; ++i) {
            UserAccounts[i] = UserAccounts[i + 1];
        }
        userCount--;
        saveUserData();
        printf("Account successfully deleted\n");
    } else {
        printf("Failed to delete account\n");
    }
}
// ! DELETE ACCOUNT END



//! VIEW PERSONAL ACCOUNT FUNCTION START
void ViewPersonalAccount(UserAccount *account){
//? personal info
    printf("\tFirst Name: %s\n", account->user.first_name);
    printf("\tLast Name: %s\n", account->user.last_name);
    printf("\tUsername: %s\n", account->user.username);
    printf("\tEmail: %s\n", account->user.email);
    printf("\tBirth Date: %s\n\n", account->user.birthdate);


//? favorite books display
    ViewFavBooksList(account);

//? readed books display
    ViewReadedBooksList(account);

//? to read books display
    ViewToReadBooksList(account);

//! change personal info
    {printf("\n\n%s, do you want to change your personal info? if yes Y, no N\nPlease enter your choice (Y/N): ", account->user.first_name);
    char change_choice;
    scanf(" %c", &change_choice);
    change_choice = toupper(change_choice);

    if (change_choice=='Y'){
    printf("\n1. Change first name");
    printf("\n2. Change last name");
    printf("\n3. Change email");
    printf("\n4. Change username");
    printf("\n5. Change birthdate");

    int choice;
    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);


    switch(choice){
        case 1:
            printf("\nPlease enter your first name: ");
            scanf("%49s", account->user.first_name);
            capitalizeFirstLetter(account->user.first_name);
            break;
        case 2:
            printf("\nPlease enter your last name: ");
            scanf("%49s", account->user.last_name);
            capitalizeFirstLetter(account->user.last_name);
            break;
        case 3:
            printf("\nPlease enter your email: ");
            scanf("%49s", account->user.email);
            break;
        case 4:
            printf("\nPlease enter your username: ");
            scanf("%29s", account->user.username);
            break;
        case 5:
            printf("\nPlease enter your birthdate: ");
            scanf("%29s", account->user.birthdate);
            break;
        default:
            printf("\nInvalid choice. Please enter a number from 1 to 5.\n");
            break;
                }
            }
            else if (change_choice == 'N') {
                return;
            }
            else {
                printf("\nInvalid choice. Please enter Y or N.\n");
            }
        }
}
//! VIEW PERSONAL ACCOUNT FUNCTION END




//! NEWBOOK INFO FUNCTION START
void newBookInfo_func(Book *newBook) {
    printf("Enter book title: ");
    scanf(" %[^\n]", newBook->title);

    printf("Enter book author: ");
    scanf(" %[^\n]", newBook->author);

    do {
        printf("Enter year of book publication (4 digits): ");
        scanf("%d", &newBook->year);
        if (newBook->year < 1000 || newBook->year > 2024) {
            printf("Error: Year must be exactly 4 digits and less than 2024\n");
        }
    } while (newBook->year < 1000 || newBook->year > 2024);
}
//! NEWBOOK INFO FUNCTION END



//! ADD TO FAVOURITE BOOKS FUNCTION START
void AddToFavorites(UserAccount *account) {
    if (account->favBooksCount >= MAX_BOOKS) {
        printf("Favourite books limit reached\n");
        return;
    }
   Book newBook;
   newBookInfo_func(&newBook);

//? check if the book is already in favBooks
    for (int i = 0; i < account->favBooksCount; ++i) {
        if (strcmp(newBook.title, account->favBooks[i].title) == 0 &&
            strcmp(newBook.author, account->favBooks[i].author) == 0 &&
            newBook.year == account->favBooks[i].year) {
            printf("\tAlready in favourites\n");
            return;
        }
    }

//? check if the book is already in readedBooks
    int i;
    for (i = 0; i < account->readedBooksCount; ++i) {
        if (strcmp(newBook.title, account->readedBooks[i].title) == 0 &&
            strcmp(newBook.author, account->readedBooks[i].author) == 0 &&
            newBook.year == account->readedBooks[i].year) {
            //? if book already in readedBooks list then, it will stop
            return;
        }
    }

    if (i == account->readedBooksCount) {
        //? check if i is equal to the number of books in readedBooks list, if it returns 1(true) then it will add the favBook to readedBooks and increment
        account->readedBooks[account->readedBooksCount++] = newBook;
        printf("\tBook added to readed books and ");
    }

//? add the book to favBooks
    account->favBooks[account->favBooksCount++] = newBook;
    printf("to favourites\n");
}
//! ADD TO FAVOURITE BOOKS FUNCTION END



//! MARK AS READED FUNCTION START
void MarkAsReaded(UserAccount *account) {
    if (account->readedBooksCount >= MAX_BOOKS) {
        printf("Readed books limit reached\n");
        return;
    }

   Book newBook;
   newBookInfo_func(&newBook);

//? Check if the book is already in readedBooks
    for (int i = 0; i < account->readedBooksCount; ++i) {
        if (strcmp(newBook.title, account->readedBooks[i].title) == 0 &&
            strcmp(newBook.author, account->readedBooks[i].author) == 0 &&
            newBook.year == account->readedBooks[i].year) {
            printf("\tAlready marked as readed\n");
            return;
            //? if book already in readedBooks list then, it will stop
        }
    }

//? check if the book is in toReadBooks
    for (int i = 0; i < account->toReadBooksCount; ++i) {
        if (strcmp(newBook.title, account->toReadBooks[i].title) == 0 &&
            strcmp(newBook.author, account->toReadBooks[i].author) == 0 &&
            newBook.year == account->toReadBooks[i].year) {
            //? if book found in toReadBooks we will remove it and add to readedBooks

            printf("\tBook '%s' by %s, published in %d, moved from to read later list to readed list\n",
                   newBook.title, newBook.author, newBook.year);

//? removing book from toReadBooks
            for (int j = i; j < account->toReadBooksCount - 1; ++j) {
                account->toReadBooks[j] = account->toReadBooks[j + 1];
            }
            account->toReadBooksCount--;
            break;
        }
    }

//? Add the book to readedBooks
    account->readedBooks[account->readedBooksCount++] = newBook;
    printf("\tBook marked as readed\n");
}
//! MARK AS READED FUNCTION END



//! MARK TO READ LATER FUNCTION START
void MarkToReadLater(UserAccount *account) {
    if (account->toReadBooksCount >= MAX_BOOKS) {
        printf("To read books limit reached\n");
        return;
    }

   Book newBook;
   newBookInfo_func(&newBook);

//? Check if the book is already in readedBooks
    for (int i = 0; i < account->readedBooksCount; ++i) {
        if (strcmp(newBook.title, account->readedBooks[i].title) == 0 &&
            strcmp(newBook.author, account->readedBooks[i].author) == 0 &&
            newBook.year == account->readedBooks[i].year) {
            printf("\tAlready marked as readed\n");
            return;
            //? if book already in readedBooks list then, it will stop
        }
    }

//? Check if the book is already in toReadBooks
    for (int i = 0; i < account->toReadBooksCount; ++i) {
        if (strcmp(newBook.title, account->toReadBooks[i].title) == 0 &&
            strcmp(newBook.author, account->toReadBooks[i].author) == 0 &&
            newBook.year == account->toReadBooks[i].year) {
            printf("\tAlready in read later list\n",
                   newBook.title, newBook.author, newBook.year);
            return;
        }
    }

//? add the book to toReadBooks
    account->toReadBooks[account->toReadBooksCount++] = newBook;
    printf("\tBook added to read later list\n");
}
//! MARK TO READ LATER FUNCTION END



//! VIEW FAVOURITE BOOKS LIST START
void ViewFavBooksList(UserAccount *account){
    if(account->favBooksCount==0){
        printf("There is no book in \"Favourite Books\" list");
        return;
    }
// ? favorite books display
    printf("\nFavorite Books:");
    for (int i=0; i<account->favBooksCount; i++){
        printf("\n\t\"%s\" by %s, published in:%d", account->favBooks[i].title, account->favBooks[i].author, account->favBooks[i].year);
    }
}
//! VIEW FAVOURITE BOOKS LIST END



//! VIEW READED BOOKS LIST START
void ViewReadedBooksList(UserAccount *account){
    if(account->readedBooksCount==0){
      printf("\n\nThere is no book in \"Readed Books\" list");
        return;
    }
//? readed books display
    printf("\n\nReaded Books:");
    for (int i=0; i<account->readedBooksCount; i++){
        printf("\n\t\"%s\" by %s, published in:%d", account->readedBooks[i].title, account->readedBooks[i].author, account->readedBooks[i].year);
    }
}
//! VIEW READED BOOKS LIST END



//! VIEW TO READ BOOKS LIST START
void ViewToReadBooksList(UserAccount *account){
    if(account->toReadBooksCount==0){
        printf("\n\nThere is no book in \"To Read Later Books\" list\n");
        return;
    }
// ? to read books display
    printf("\n\nTo Read Later Books:");
    for (int i=0; i<account->toReadBooksCount; i++){
        printf("\n\t\"%s\" by %s, published in:%d", account->toReadBooks[i].title, account->toReadBooks[i].author, account->toReadBooks[i].year);
    }
}
//! VIEW TO READ BOOKS LIST END



//! DISPLAY MENU FUNCTION START
void displayMenu() {
    printf("\n\nMenu:");
    printf("\n1. Sign Up");
    printf("\n2. Log In");
    printf("\n3. View Personal Account");
    printf("\n4. View Favorites Books List");
    printf("\n5. View Readed Books List");
    printf("\n6. View To Read Later Books List");
    printf("\n7. Add to Favorites");
    printf("\n8. Mark as Readed");
    printf("\n9. Mark to Read Later");
    printf("\n10. Log Out");
    printf("\n11. Delete Account");
    printf("\n12. Exit");
}
//! DISPLAY MENU FUNCTION END



//! MAIN FUNCTION START
int main() {
    int choice;
    int loggedInUserIndex = -1;
    loadUserData();
    printf("\n\t\tWelcome to Book Base! Please Sign Up(1) or Log In(2)");

    do {
        if (loggedInUserIndex == -1) {
            displayMenu();
            printf("\n\nEnter your choice: ");
            scanf("%d", &choice);
        } else {
            printf("\nEnter your choice: ");
            scanf("%d", &choice);
        }
        switch (choice) {
            case 0:
                displayMenu();
                break;
            case 1:
                SignUp();
                saveUserData();
                break;
            case 2:
                if (loggedInUserIndex == -1) {
                loggedInUserIndex = LogIn();
                }
                else {
                    printf("\tYou already logged in\n");
                }
                break;
            case 3:
                if (loggedInUserIndex != -1) {
                    ViewPersonalAccount(&UserAccounts[loggedInUserIndex]);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 4:
                if (loggedInUserIndex != -1) {
                    ViewFavBooksList(&UserAccounts[loggedInUserIndex]);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 5:
                if (loggedInUserIndex != -1) {
                    ViewReadedBooksList(&UserAccounts[loggedInUserIndex]);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 6:
                if (loggedInUserIndex != -1) {
                    ViewToReadBooksList(&UserAccounts[loggedInUserIndex]);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 7:
                if (loggedInUserIndex != -1) {
                    AddToFavorites(&UserAccounts[loggedInUserIndex]);
                    saveBooksData(&UserAccounts[loggedInUserIndex], "fav_books.dat", UserAccounts[loggedInUserIndex].favBooks, UserAccounts[loggedInUserIndex].favBooksCount);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 8:
                if (loggedInUserIndex != -1) {
                    MarkAsReaded(&UserAccounts[loggedInUserIndex]);
                    saveBooksData(&UserAccounts[loggedInUserIndex], "readed_books.dat", UserAccounts[loggedInUserIndex].readedBooks, UserAccounts[loggedInUserIndex].readedBooksCount);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 9:
                if (loggedInUserIndex != -1) {
                    MarkToReadLater(&UserAccounts[loggedInUserIndex]);
                    saveBooksData(&UserAccounts[loggedInUserIndex], "to_read_books.dat", UserAccounts[loggedInUserIndex].toReadBooks, UserAccounts[loggedInUserIndex].toReadBooksCount);
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 10:
                if (loggedInUserIndex != -1) {
                    loggedInUserIndex=-1;
                    printf("\n\tLogged out successfully!");
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;
            case 11:
                if (loggedInUserIndex != -1) {
                    deleteAccount(&UserAccounts[loggedInUserIndex]);
                    loggedInUserIndex=-1;
                    saveUserData();
                } else {
                    printf("\tYou need to log in first\n");
                }
                break;

            case 12:
            saveUserData();
                printf("Program finished\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number from 1 to 11\n");
        }
    }
    while (choice != 12);
        return 0;
    }
//! MAIN FUNCTION END
