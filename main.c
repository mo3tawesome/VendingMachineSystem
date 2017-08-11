#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct drink  // struct to store the values of any drink in the machine
{
    char name [10]; // name of the drink
    int price; // price of a single drink
    int qtyAvailable; // available quantity of that drink
};
char returns[50]; // used in the search function as the return value
struct drink array [3][2]; // matrix of drinks
char adminUsername[] = "admin";
char adminPassword[] = "2525";
int UserType; // stores first user input (purchase or admin)
int column; // user inputted column to choose from the matrix
int row;// user inputted row to choose from the matrix
int desiredQty; // desired quantity of the drink
int totalPrice=0; // total price of the drinks desired
int cash; // amount payed by the user
int change; // change for the user
char purchaseInfo [250]; // line that stores a single purchase's info to be written in a file
void startMenu(); // function to display choices at the begining
void fillqty(); // function to fill the quantities from the file
void fillprice(); // functionn to fill the price from the file

void printfile(char filename[50])  // this function can print any file with the entered name + .txt
{
    FILE *fptr; // file pointer
    fptr = fopen(filename,"r"); // open the file for read
    const size_t line_size = 250; /// assumes max line size of 250 chars
    char* line = malloc(line_size); //dynamic allocation of a string to hold the line of mac length
    while (fgets(line, line_size, fptr) != NULL)    //while there are still lines to get, get the line
    {
        printf("%s",line); //print the line
    }
    free(line); //free the string allocated from the memory
    fclose(fptr); // close the file
}
char* searchFileS(char filename[50], char keyword[50]) //searches for any keyword in any file
{

    FILE *fptr;
    fptr = fopen(filename,"r"); // open the file for read
    const size_t line_size = 250; ///assumed line size
    char* line = malloc(line_size); //allocation
    while (fgets(line, line_size, fptr) != NULL)    //get the line
    {
        char *ret;
        ret = strstr(line, keyword); // check if the line has the keyword
        if (ret != NULL)
        {
            strcpy(returns,line); //copy the line to returns if it has the keyword
        }
    }
    free(line); // free the dynamic allocation
    fclose(fptr); // close the file
    return returns; // return the line
}
void getCash()
{
    printf("\nplease enter cash in 5,10,20 RM:\t");
    scanf("%d", &cash);
    if ((cash%10&&cash%20&&cash%5)!=0) // if the amount inserted is not a multiple of 5,10 or 20
    {
        printf("\ninvalid currency");
        getCash(); // get the amount again
    }
}
void modifyFile(char filename[50], char lineToRemove[250], char lineToAdd[250]) // removes, adds or modify lines in a file
{
    FILE *fptr;
    fptr = fopen(filename,"r"); // open for read
    int deleteFlag=0; // flag for checking if there's something to delete
    const size_t line_size = 250; ///assumed max line size of 250 chars
    char* line = malloc(line_size); // dynamic allocation of the line
    const size_t file_size = 5000; ///assumed max file size of 250 chars
    char* file = malloc(file_size);
    file[0] = '\0'; // remove anything in the string that will hold the file
    while (fgets(line, line_size, fptr) != NULL)    //get the line
    {
        char *ret; // variable to hold the line to remove if it exists (holds null if it doesn't)
        ret = strstr(line, lineToRemove); // check if the line exists
        if (ret != NULL)
        {
            strcat(file, lineToAdd); //add the line to add in its place
            strcat(file, "\n"); //add a new line
            deleteFlag=1; // indicate that deletion has happened
        }
        else  //fill the rest of the file as it is
        {
            strcat(file, line);
        }
    }
    if (deleteFlag!=1)  // if there was nothing to remove
    {
        strcat(file, lineToAdd); // add the new line at the end
        strcat(file, "\n");
    }
    free(line); //free allocation
    fclose(fptr); //close file

    fptr = fopen(filename,"w"); // open the file for write (deletes what's already in the file)
    int w =0; // loop counter
    int len = strlen(file); //get file length
    for(w=0; w<len; w++)
    {
        fprintf(fptr,"%c",file[w]);//print the file
    }
    fclose(fptr); //close file

}
void chooseDrink()
{
    printf("\nEnter row number:\t");
    scanf("%d", &row);
    printf("\nEnter column number:\t");
    scanf("%d", &column);
    if (((row>3)||column>2)||((row<1)||column<1)) //if rows and columns are not within range
    {
        printf("\ninvalid row or column number, try again:\t");
        chooseDrink(); // take the inputs again
    }
    printf("\nEnter desired quantity:\t");
    scanf("%d", &desiredQty);
    if(desiredQty>(array[row-1][column-1].qtyAvailable)) //if desired quantity is more than the available
    {
        printf("\nthe desired quantity is larger than the available");
        printf("\nyou'll only get:\t %d",array[row-1][column-1].qtyAvailable); // print the available amount
        desiredQty=array[row-1][column-1].qtyAvailable; // set the desired to the available amount
    }
    totalPrice = (array[row-1][column-1].price)*desiredQty; //total price is the price of one times the amount
    printf("\ntotal price equals:\t%d",totalPrice);
    printf("\n\nEnter 1 to confirm, any other number to cancel:\t");
    int confirmation;
    scanf("%d", &confirmation);
    if(confirmation==1)
    {
        getCash();
    }
    else
    {
        system("cls"); //clear the screen
        startMenu();
    }
    while (totalPrice>cash) // if the amount entered is smaller than the price
    {
        printf("\n amount entered is too small, try again\t:");
        getCash(); //keep getting the price until it's bigger
    }
    change=cash-totalPrice; //change is the difference between payed and total price
    if (change>0) // if there's change tell the user
    {
        printf("\nPlease collect your change:\t%dRM",change);
    }
    array[row-1][column-1].qtyAvailable -= desiredQty; // the new available quantity is decreased by the amount taken
    printf("\n%d %s(s) have been dispatched from bin\n\n", desiredQty,array[row-1][column-1].name); //print the name and number
    purchaseInfo[0]='\0';//initialize the string to be empty
    char Str [5]; // string to hold the desired qty as a string
    sprintf(Str, "%d", desiredQty);//put the desired qty int in the string str
    strcat(purchaseInfo,Str); // add the quantity to the string to be witten to the file
    strcat(purchaseInfo,"   ");// add a space
    strcat(purchaseInfo,array[row-1][column-1].name); // add the name
    strcat(purchaseInfo,"   ");
    sprintf(Str, "%d", array[row-1][column-1].price); // put the price as a string to be added
    strcat(purchaseInfo,Str); // add the price
    strcat(purchaseInfo,"    ");
    sprintf(Str, "%d", totalPrice);
    strcat(purchaseInfo,Str); // add the total price
    strcat(purchaseInfo,"   ");
    sprintf(Str, "%d", array[row-1][column-1].qtyAvailable);
    strcat(purchaseInfo,Str); // add the available quantity
    strcat(purchaseInfo,"   ");
    strcat(purchaseInfo,"\n");
    //-------------- fill the files with the new values
    char returno [50];// stores the line that has the name of the chosen drink
    char newl [50]; // the new string to be written to the quantities file
    char tempo [5];//stores the quantity as a string
    strcpy(returno,searchFileS("quantities.txt",array[row-1][column-1].name)); // gets the line from qtys file
    sprintf(tempo, "%d", array[row-1][column-1].qtyAvailable); // put qty available as a string
    strcpy(newl,tempo); // add the qty to the string to be written to the file
    strcat(newl,"  ");//add a space
    strcat(newl,array[row-1][column-1].name); //add the name
    modifyFile("quantities.txt",returno,newl); // modify the file with the new quantity
    //--------------
    modifyFile("purchases.txt","dummy line",purchaseInfo); // add the new purchase to the purchase file

    startMenu(); // choose another operation
}
void adminPanel() // function to deal with admin choices
{
    int choice;
    printf("\nEnter 1 to check quantities, 2 to get report, 3 to change a price, 4 to logout:\t");
    scanf("%d", &choice);
    if(choice==1)
    {
        int choose;
        printf("\nformat:");
        printf("\nQuantity name\n");
        printfile("quantities.txt"); // print the quantitie file
        printf("\nEnter 1 to change quantities, any other number to choose another operation :\t");
        scanf("%d", &choose);
        if(choose==1)
        {
            char name [10]; // scan the name
            char qty [5]; // scan the new quantity
            char rem[50]; //the line to remove from file
            char neo[50]; // the new line to add to the file
            returns[0]='x'; // initialize returns to have the value x
            returns[1]='\0';
            while(returns[0]=='x') // keep trying until a valid name is inserted
            {
                printf("\nEnter drink name :\t");
                scanf("%s", name);

                searchFileS("quantities.txt",name); // searchFileS fills "returns" if the name was found
                if(returns[0]=='x') //if it wasn't fount it remains "x"
                {
                    printf("\ninvalid name");
                }
            }
            printf("\nEnter new quantity :\t");
            scanf("%s", qty);
            strcpy(rem,searchFileS("quantities.txt",name)); // copy strings and do the file stuff
            strcpy(neo,qty);
            strcat(neo,"  ");
            strcat(neo,name);
            modifyFile("quantities.txt",rem,neo);
            fillqty(); // fill the quantities from the file
            adminPanel();// get new choice
        }
        else
        { // if the choice was something else
            printf("\ninvalid\n\n");//print that it's an invalid choice
            adminPanel();//get a new choice
        }
    }
    else if (choice==2)
    {
        printfile("purchases.txt"); // print the purchases file
        adminPanel(); // get a choice
    }
    else if (choice==3) // same as quantities except with prices
    {
        int choose;
        printf("\nformat:");
        printf("\nPrice name\n");
        printfile("prices.txt");
        printf("\nEnter 1 to change a price, any other number to choose another operation :\t");
        scanf("%d", &choose);
        if(choose==1)
        {
            char name [10];
            char qty [5];
            char rem[50];
            char neo[50];
            returns[0]='x';
            returns[1]='\0';
            while(returns[0]=='x')
            {
                printf("\nEnter drink name :\t");
                scanf("%s", name);

                searchFileS("prices.txt",name);
                if(returns[0]=='x')
                {
                    printf("\ninvalid name");
                }
            }
            printf("\nEnter new price :\t");
            scanf("%s", qty);
            strcpy(rem,searchFileS("prices.txt",name));
            strcpy(neo,qty);
            strcat(neo,"  ");
            strcat(neo,name);
            modifyFile("prices.txt",rem,neo);
            fillprice();
            adminPanel();
        }
        else
        {
            printf("\ninvalid\n\n");
            adminPanel();
        }
    }
    else if (choice==4)
    {
        startMenu();
    }
    else
    {
        printf("\ninvalid\n\n");
        startMenu();
    }
}




    void startMenu()
    {
        printf("Enter 1 to make a purchase, 2 for admin mode:\t");
        scanf("%d", &UserType);
        if(UserType==1) // if the user wants to make a purchase
        {
            printf("choose a drink of the following drinks:\n\n");
            //print available drinks names
            int i=0; //loop counters
            int j=0;
            for(i=0; i<3; i++)
            {
                for(j=0; j<2; j++)
                {
                    printf("%s\t",array[i][j].name); //loop on the array of structs and print each name
                }
                printf("\n");
            }
            chooseDrink(); //choose from the drinks
        }
        else if(UserType==2) // if the user is an admin
        {
            char usern [20];
            char passw [20];
            printf("\nEnter username:\t");
            scanf("%s", usern);
            printf("\nEnter password:\t");
            scanf("%s", passw);
            if((strcmp(usern,adminUsername)==0)&&(strcmp(passw,adminPassword)==0))
            {
                adminPanel(); // if username and password matches go to the admin panel
            }
            else
            {
                printf("\ninvalid\n\n");
                startMenu(); // if they don't match go to the start menu
            }
        }
        else // if the user doesn't wanna make a purchase or is an admin
        {
            printf("\ninvalid input, please try again\n\n");
            startMenu();
        }
    }
    void initFile() // function to initialize the purchases file if it doesn't exist
    {
        FILE *fptr; // file pointer
        fptr = fopen("purchases.txt", "r"); // open the file for read
        if (!fptr) // if the file doesn't exist
        {
            fptr = fopen("purchases.txt","w"); // open for write (create the file)
            char string [250]; //string to be written in the file
            string[0]='\0'; // empty the string
            strcat(string,"soldQty name price totPrice leftQty\n"); // put the first line which is the format
            modifyFile("purchases.txt","dummy line",string); // write it to file
        }
        fclose(fptr); //close file
    }
    void initvalfile(char* stringa) // initialize price or quantities file
    {//same way as before
        FILE *fptr;
        fptr = fopen(stringa, "r");
        if (!fptr)
        {
            fptr = fopen(stringa,"w"); // open for read

            int i,j;
            for(i=0; i<3; i++)
            {
                for(j=0; j<2; j++)
                {
                    char string [250];
                    string[0]='\0';
                    strcat(string,"20");//put the value "20"
                    strcat(string,"  ");
                    strcat(string,array[i][j].name); // put the name
                    strcat(string,"\n");
                    modifyFile(stringa,"dummy line",string);
                }
            }
        }
        fclose(fptr); //close file
    }

    void fillqty() // fills the quantities from the quantities file
    {
        char returno [50]; // gets the return of the search function
        int c=0; // loop counter
        int w,i,j; // loop counters
        char temp[5];//stores the quantity from the file
        temp[2]='\0'; // initialize to avoid errors
        for(i=0; i<3; i++)
        {
            for(j=0; j<2; j++)
            {
                strcpy(returno,searchFileS("quantities.txt",array[i][j].name)); //get the line that has the name
                while (returno[c]!=' ') //loop until the first word is over (the quantity word) and let (c) hold that value
                {
                    c++;
                }
                for(w=0; w<c; w++) // loop from from 0 to c
                {
                    temp[w]=returno[w]; //fill the temp string from the begining with the quantity
                }
                array[i][j].qtyAvailable = atoi(temp);//put the quantity as an int in the qty available
            }
        }
    }
    void fillprice()//sane as quantity filling
    {
        char returno [50];
        int c=0;
        int w,i,j;
        char temp[5];
        temp[2]='\0';
        for(i=0; i<3; i++)
        {
            for(j=0; j<2; j++)
            {
                strcpy(returno,searchFileS("prices.txt",array[i][j].name));
                while (returno[c]!=' ')
                {
                    c++;
                }
                for(w=0; w<c; w++)
                {
                    temp[w]=returno[w];
                }
                array[i][j].price = atoi(temp);
            }
        }
    }
int main()
    {
        /// manually filling the drinks matrix with the drinks' names
        // first drink
        strcpy(array[0][0].name, "Coke");
        // 2nd drink
        strcpy(array[0][1].name, "Fanta");
        // 3rd drink
        strcpy(array[1][0].name, "Sprite");
        // 4th drink
        strcpy(array[1][1].name, "DrPepper");
        // 5th drink
        strcpy(array[2][0].name, "Dasani");
        //6th drink
        strcpy(array[2][1].name, "Schwepps");
        initFile();//initialize the purchases file
        initvalfile("quantities.txt");//initialize the quantities file
        initvalfile("prices.txt");//initialize the price file
        fillqty(); // fill the structs from the qties file
        fillprice();// fill the structs from the price file
        startMenu(); // go to the start menu
        return 0;
    }
