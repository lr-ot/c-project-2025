#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct film {
    int filmID;
    char filmName[200];
    char filmDirector[100];
    char filmReleaseDate[20];
    char filmRating[10];
    double costPerRental;
    int numOfMonthlyViews;
    int currentlyRentedID;
    char rentalExpiration[20];
    struct film *next;
};
 
struct film *AddEntry(struct film *);
void BrowseEntries(struct film*);
struct film *EditEntry(struct film *);
struct film *DeleteEntry(struct film *);
void SearchForTitle(struct film*);
void TallyMonthlyEarnings(struct film*);
struct film *ModifyValues(struct film *);
void DisplayData(struct film*);
struct film *MemoryCleanUp(struct film *);


int main () {
    struct film *start = NULL;
    int choice = 0;

    while (choice != 7) {
        int ch;
        
        //prompt user for choice
        puts("---Film Tracker----------------------------------------");
        puts("| 1. Add new film entry   | 4. Browse film entries    |");
        puts("| 2. Edit film entry      | 5. Search film by title   |");
        puts("| 3. Delete film entry    | 6. Tally monthly earnings |");
        puts("|                         | 7. Exit                   |");
        puts("-------------------------------------------------------");
        puts("Enter your choice: ");
        scanf("%d", &choice);
        while ( ( ch = getchar() ) != '\n' && ch != EOF );   

        //case statement to call function for choice made
        switch (choice) {
            case 1: start = AddEntry(start); break;
            case 2: start = EditEntry(start); break;
            case 3: start = DeleteEntry(start); break;
            case 4: BrowseEntries(start); break;
            case 5: SearchForTitle(start); break;
            case 6: TallyMonthlyEarnings(start); break;
            case 7: start = MemoryCleanUp(start); exit(0);
        }
    }

    //clean up memory
    start = MemoryCleanUp(start);

    puts("finished");  

    return 0;
}

struct film *AddEntry(struct film *start) {
    struct film *move = NULL, *new = NULL;
    int ch;
    int newID = 1;

    puts("---\nAdd new film entry");  

    //allocate new node
    new = (struct film*)malloc(sizeof(struct film));
    if (new == NULL) {
        puts("memory error");
        start = MemoryCleanUp(start);
        exit(1);
    }

    if (start == NULL) {
        //assign address in new to start
        start = new;
    } else {
        //loop through the list until we reach the last node.
        //allocate new memory, tie into the list
        move = start;
        while (move->next != NULL) {
            move = move->next;
        }
        //move is now positioned at the last node of the existing list
        newID = move->filmID + 1;
        //tie it into the existing list
        move->next = new;
    }

    //set ID to new node
    new->filmID = newID;
    new->next = NULL;

    //modify values
    ModifyValues(new);
            
    return start;
}

void BrowseEntries(struct film* start){
    struct film *move = start;

    puts("---\nBrowse film entries");  

    //return if node is empty
    if (move == NULL) {
        printf("Film entry data not found\n");
        return;
    }
    
    //loop through linked list until it finds the terminal node
    while (move->next != NULL) {
        //print data
        DisplayData(move);
        move = move->next;
    }

    //print data at last node
    DisplayData(move);
    move = move->next;

    puts("---\nPress ENTER to continue");  
    getchar();    

}

struct film *EditEntry(struct film *start) {
    struct film *move = start;
    int ch;
    int idToEdit = 0;

    puts("---\nEdit film entry");  

    //return if node is empty
    if (move == NULL) {
        printf("Film entry data not found\n");
        return start;
    }

    //prompt for film ID to edit
    puts("Enter ID of entry to edit:"); 
    scanf("%d", &idToEdit);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );
    
    while (move != NULL && move->filmID != idToEdit) {
        move = move->next;
    }

    //edit node if position found
    if (move != NULL) {
        DisplayData(move);
        ModifyValues(move);
    } else {
        //otherwise print error message
        printf("Film entry data not found\n");
    }

    return start;
}

struct film *DeleteEntry(struct film *start) { 
    struct film *move = start, *prev = NULL;
    int ch;
    int idToDelete = 0;

    puts("---\nDelete film entry");  

    //return if node is empty
    if (move == NULL) {
        printf("Film entry data not found\n");
        return start;
    }

    //prompt for film ID to delete
    puts("Enter ID of entry to delete:"); 
    scanf("%d", &idToDelete);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //if start node needs to be deleted
    if (start->filmID == idToDelete) {
        start = move->next;
        free(move);
        printf("Entry %d deleted.\n", idToDelete);
        return start;
    }

    //traverse list until given position
    while (move != NULL && move->filmID != idToDelete) {
        prev = move;
        move = move->next;
    }

    //delete node if position found
    if (move != NULL) {
        prev->next = move->next;
        free(move);
        printf("Entry %d deleted.\n", idToDelete);
    } else {
        //otherwise print error message
        printf("Film entry data not found\n");
    }

    return start;
}

void SearchForTitle(struct film* start) { 
    struct film *move = start;
    char filmSearchQuery[200];
    int ch;
    int i = 0;

    puts("---\nSearch film by title");  

    //return if node is empty
    if (move == NULL) {
        printf("Film entry data not found\n");
        return;
    }

    //prompt for film title to search
    puts("Enter title of film to search:"); 
    fgets(filmSearchQuery, 200, stdin);
    filmSearchQuery[strcspn(filmSearchQuery, "\n")] = '\0';

    while (move != NULL) {
        //if string of search query is contained within film name
        if ( strncmp(move->filmName, filmSearchQuery, strlen(filmSearchQuery)) == 0) {
            //display data at node
            DisplayData(move);
            //add to number of matches found
            i++;
        }
        move = move->next;
    }

    //print number of matches found
    printf("---\n%d match(es) found\n", i);
    puts("---\nPress ENTER to continue");  
    getchar();   

}

void TallyMonthlyEarnings(struct film* start){
    struct film *move = start;
    double monthlyTotal = 0.00;

    puts("---\nTally monthly earnings");  

    if (move == NULL) {
        printf("Film entry data not found\n");
        return;
    }

    //loop through linked list until it finds the terminal node
    while (move->next != NULL) {
        //add amount at node to monthly total
        monthlyTotal += (move->costPerRental * move->numOfMonthlyViews);
        move = move->next;
    }

    //add amount at last node to monthly total
    monthlyTotal += (move->costPerRental * move->numOfMonthlyViews);
    move = move->next;

    //print monthly earnings
    printf("Monthly earnings: $%.2lf\n", monthlyTotal);
}

struct film *ModifyValues(struct film *new){
    int ch;

    //display film ID
    printf("\nFilm ID: %d\n", new->filmID);

    //get film name
    puts("Enter film name:"); 
    fgets(new->filmName, 200, stdin);
    new->filmName[strcspn(new->filmName, "\n")] = '\0';

    //get film director
    puts("Enter film director:");
    fgets(new->filmDirector, 100, stdin);
    new->filmDirector[strcspn(new->filmDirector, "\n")] = '\0';
    
    //get film rating
    puts("Enter film rating:"); 
    fgets(new->filmRating, 20, stdin);
    new->filmRating[strcspn(new->filmRating, "\n")] = '\0';
    
    //get film release date
    puts("Enter film release date:");
    fgets(new->filmReleaseDate, 20, stdin);
    new->filmReleaseDate[strcspn(new->filmReleaseDate, "\n")] = '\0';

    //get cost per rental
    new->costPerRental = 5.99;

    //get number of monthly views
    puts("Enter number of monthly views:"); 
    scanf("%d", &new->numOfMonthlyViews);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //get id of customer currently renting
    puts("Enter ID of customer currently renting:"); 
    scanf("%d", &new->currentlyRentedID);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //get rental expiration date
    puts("Enter rental expiration date:");
    fgets(new->rentalExpiration, 20, stdin);
    new->rentalExpiration[strcspn(new->rentalExpiration, "\n")] = '\0';

    return new;
}

void DisplayData(struct film *move) {
    //print film data
    printf("---\n");
    printf("Film ID: %d\n", move->filmID);
    printf("Film Name: %s\n", move->filmName);
    printf("Film Director: %s\n", move->filmDirector);
    printf("Film Rating: %s\n", move->filmRating);
    printf("Film Release Date: %s\n", move->filmReleaseDate);
    printf("Cost Per Rental: $%.2lf\n", move->costPerRental);
    printf("Number of monthly views: %d\n", move->numOfMonthlyViews);    
    printf("Currently Rented By Customer ID: %d\n", move->currentlyRentedID);
    printf("Rental Expiration Date: %s\n", move->rentalExpiration);
}

struct film *MemoryCleanUp(struct film *start) {
    //loop through the LL until it finds the terminal node
    struct film* move = start;

    while (move->next != NULL) {
        struct film* last = move;
        move = move->next;
        free(last);
    }

    //free last node
    free(move);
    puts("all free");

    return NULL;
}