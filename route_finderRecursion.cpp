// Name:       Kyle Koivuneva
// email:      kkoiv395@mtroyal.ca
// Course:     COMP 1633-001
// Instructor: Paul Pospisil
// Assignment: 3
// Due Date:   March 5th, 2020 
//
//
// Purpose:  This program computes the number of diffrent paths through an
//           nxn grid where each intersection is visited only once, starting
//           at 1,1 and ending at n,0
//
// Details:
//
//   Input:  The program asks for the size of the square grid to be analyzed
//
//
//   Output: The program outputs: 1) the number of unique, complete paths.
//           2) The fastest path through the grid, as a list of 
//           coordinates. 3) The time the most optimal path takes to
//           reach the end.
//                   
//
//   Asumptions and Limitations:
//           Grid size must be entered as a positive number, zero or
//           greater.



#include <iostream>
using namespace std;



struct path{
    
    path *next;       //Each "path" will contain the coordinates
    int x;            //so that a list of type path will be a list 
    int y;            //containing the complete path to the finish

};

typedef path *path_ptr;

void grid_builder(bool* gptr[], int size, int indicie);
void fill(bool* gptr[], int size, int indicie, int start);
void delete_grid(bool* gptr[], int size, int indicie);
void grid_traverse(bool* gptr[], int x, int y, int endx, int endy, int size, 
		   bool& done, int& paths, path_ptr &head,path_ptr
		   &best_time);
bool check_grid(bool* gptr[], int size, int x, int y);
bool can_move(bool* gptr[], int x, int y, int size);
void path_maker(path_ptr &head, int x, int y);
void delete_path(path_ptr head);
void print_path(path_ptr head);
void store_path(path_ptr head, path_ptr &best_time);
void copy_list(path_ptr head, path_ptr &best_time);
void delete_list(path_ptr &best_time);
int total_time(path_ptr head);
void path_navigator(path_ptr head, path_ptr &current);



int main(){

    path_ptr head = NULL;        //Head will contain the complete path list
    path_ptr best_time = NULL;   //best_time will be a copy of head so head
                                 //can be reused
    int paths = 0;               
    int x = 0;                   
    int y = 0;                   //X and y keep track of location, paths 
                                 //keeps track of total complete paths
    bool **gptr;                 //2D array
    
    bool done = false;          
    int size = 0;               


    cout << "enter a grid size: ";   //Size of array taken
    cin >> size;
    
    int end_y = size-1;         //end is size-1 since indicies start at 0
 
    gptr = new bool* [size];        //gptr points to an array of pointers
    
    grid_builder (gptr, size, 0);

    grid_traverse(gptr, x, y, 0, end_y, size, done, paths, head ,best_time);

    //output details printed
    cout <<"For a " << size << " by " << size << " grid" << endl;
    cout << "Possible paths = " << paths << endl;
    cout << "shortest path time = " << total_time(best_time)<<
	" minutes" << endl;
    print_path(best_time);

    delete_list(best_time);        //memory allocated to 2d array returned
    delete_grid(gptr, size, 0);
    delete[] gptr;

    
    return 0;
}



//------------------------------------------------------------
// function name: delete_grid
//
// Purpose: deletes the array's pointers and everything they point to
//
// Inputs:  the 2d array, the size of it, and a counter which iterates
//          through the 2d array
//
// Output:  the 2d array will be reduced to a 1d array where all 
//          entries are pointers.
//
// Recursive details: 
//          base case: done iterating through the array and deleting
//                     all arrays to which the pointers point to.
//
//          recursive case: have not iterated throutgh the list
//                          of pointers
//------------------------------------------------------------
void delete_grid(bool* gptr[], int size, int indicie){

    if (indicie < size) {
	
	delete[] gptr[indicie];
	delete_grid(gptr, size, indicie+1);
    }

    return;
}


//------------------------------------------------------------
// function name: grid_builder
//
// Purpose: given an array of pointers, creates an array for each pointer 
//          so that the array is n by n
//
// Inputs:  an array of pointers.
//
// Output:  an array of pointers pointing to an array of type bool.
//
// Recursive details:
//          base case: all pointers in the array are now pointing to arrays
//
//          Recursive case: not all pointers are pointiing to arrays.
//------------------------------------------------------------
void grid_builder(bool* gptr[], int size, int indicie){

    if (indicie < size)  {
	
	gptr[indicie] = new bool[size];  
      	fill (gptr, size, indicie, 0);
	grid_builder(gptr, size, indicie+1);  //goes to next element 
    }
    
    return;
}
//------------------------------------------------------------
// function name: fill
//
// Purpose: fills the 2d array so that each bool value = false
//
// Inputs:  the 2d array, the size of it, and a counter which iterates
//          through the 2d array
//
// Output:  the 2d array will have all false entries
//
// Recursive details: 
//          base case: at size -1 ie the max size of the
//          array.
//
//          recursive case: at indicie < size, so not at the max
//          indicies of the array.
//------------------------------------------------------------
void fill(bool* gptr[], int size, int indicie, int start){


    if (start < size) {
	
	gptr[indicie][start] = false;       //sets the point to false
	fill (gptr, size, indicie, start+1);
    }
    
    return;
}




//------------------------------------------------------------
// function name: grid_traverse
//
// Purpose: finds every single path through the grid, starting at 0,0
//          ending at size-1 , 0
//
// Inputs:  the 2d array, the size of it, current x position, current
//          y position, the end point, if the end has been reached, a 
//          counter which counts the number of paths found, 
//
// Output:  the number of unique paths, a path list for each unique path
//          
// Recursive details: 
//          base case: current x,y is at the end x and end y point
//                     with all intersections visited, however even then
//                     the program will recurse until every single possible
//                     combination has been attempted.
//
//          recursive case: current x,y is not at end x and end y.
//                          or, there is at least one path which has
//                          not been visited. Even then, the function
//                          will continue until every combo has been 
//                          attempted
//------------------------------------------------------------

void grid_traverse(bool* gptr[], int x, int y, int endx, int endy, int size,
		   bool& done, int& paths, path_ptr &head,  
		   path_ptr &best_time){
//Its row , collumn, so y,x, but its square so... collumn, row x,y  

    path_maker(head, x, y);    //current location is marked as visited, 
    gptr[x][y] = true;         //and the location is added to the path list
   
    
     if (x == endx && endy == y){  //If at destination, stop, so that    
	 done = true;              //the x and y does not go beyond the finish
      }
    
    else{

	//moving in each direction is tested to be valid or invalid. 
	//after trying a direction, if the next move or subsequent do not work
	//that move will not be re-attempted since it was already tried
	if (can_move(gptr, x+1, y, size))       
	{
	    grid_traverse(gptr, x+1, y, endx, endy, size, done ,paths, 
			  head ,best_time);

	}
	if (can_move(gptr, x, y+1, size))    
	{
	    grid_traverse(gptr, x, y+1, endx, endy, size, done ,paths,
			  head ,best_time);
	    	    
	}

	if (can_move(gptr, x-1, y, size))    
	{
	    grid_traverse(gptr, x-1, y, endx, endy, size, done ,paths,
			  head ,best_time);

	}
	if (can_move(gptr, x, y-1, size))    
	{
	    grid_traverse(gptr, x, y-1, endx, endy, size, done ,paths,
			  head ,best_time);
	    
	}
	
    }

//if at the end and all intersections visited, thats a complete path
     if (check_grid(gptr, size, 0, 0) && done){

	 paths++;
	 store_path(head,best_time);   //path is stored.
	 print_path(head);
	 cout << endl;
     }
//if all movement options are invalid or the finish has been encountered
//backtracking must take place, so the point is set to not visited, and the
//point on the path list, deleted, as long as the list is non empty
     done = false;	      
     gptr[x][y] = false;      
                              
     if (head!= NULL)         
	 delete_path(head);
     
     return;
} 

//------------------------------------------------------------
// function name: can_move
//
// Purpose: returns if movement is allowed to the point specified
//
// Inputs:  the 2d array of booleans, the size of the array, 
//          an x,y representing the intended movement option
//
// Output:  a true or false, if movement is possible
//
// Checks:  If the x,y is outside of the 2d array,
//          and checks if x,y is a point already visited

bool can_move(bool* gptr[], int x, int y, int size){  
    
    bool possible = false;
    
    if (0 <= x && x < size && 0 <= y && y < size && !gptr[x][y])  
	possible = true;
    
    return possible;
}



//------------------------------------------------------------
// function name: check_grid
//
// Purpose: checks if the 2d array is all true
//
// Inputs:  the 2d array and its size
//
// Output:  a true or false, if the the grid is all true or
//          at least one false
//
// Recursive details: 
//          base case: current x,y is at size-1, size-1, meaning
//                     all points on the 2d array have been checked
//
//          recursive case: current x,y is not at size-1, size-1

bool check_grid(bool* gptr[], int size, int x, int y) { 

    bool complete = true;
    
    if (y >= size);       //At max indicies, base case
    
    else {
		
	if (x == size) {  //See if y needs to be incremented
	    y++;
	    x = 0;
	}
	
	if (y < size)     
	    complete = gptr[x][y];   //checks if point is valid
	
	//re-runs the function, with x being incremented.
	if (complete == true){       
	    complete = check_grid(gptr, size, x+1, y);
	    
	}
    }
    return complete;
}


//------------------------------------------------------------
// function name: path_navigator
//
// Purpose: returns a pointer, pointing the the last element in a 
//          linked list
//
// Inputs:  the linked list and a pointer
//
// Output:  the pointer will point to the last element in the list
//
// Recursive details: 
//          base case: head is at the end of the list
//
//          recursive case: head is not at the end of the list

void path_navigator(path_ptr head, path_ptr &current){
    
    if (head->next !=NULL)
	path_navigator(head -> next, current);	
    
    else
	current = head;
}

//------------------------------------------------------------
// function name: path_maker
//
// Purpose: adds a "path" aka a point to the end of a linked list
//
// Inputs:  the linked list and the coordinate which is to be added
//
// Output:  a linked list with a new node containing the data, which 
//          points to NULL

void path_maker(path_ptr &head, int x, int y){

    path_ptr temp = NULL;
    path_ptr current = NULL;
    

    if (head==NULL){        
	temp = new path;       //if list is empty, inserts at head.
	head = temp;
	temp -> next = NULL;
    }    
    else 
    {
	current = head;
	path_navigator(head, current);  //sets a pointer to end of list
	temp = new path;
	current -> next = temp;         //new node made, and set to
	temp->next = NULL;              //point to NULL
	
    }
    
    temp -> x = x;     //data inserted
    temp -> y = y;
   
    return;
}

//------------------------------------------------------------
// function name: print_path
//
// Purpose: prints out a linked list.
//
// Recursive case: not at end of list
//
// Base case: at end of list
//
// Extra details: all calculations have been done as x,y. This function 
//                prints y,x to comply with the rows, collumns notation                

void print_path(path_ptr head){
        
    if (head != NULL){ 

	cout << '(' <<  head -> x << "," << head -> y << ") ";
	print_path(head->next);
    }

    return;
}

//------------------------------------------------------------
// function name: delete_path
//
// Purpose: deletes the last coordinate set of the path
//
// Inputs:  the linked list
//
// Output:  a linked list where the last node is gone, and the previous
//          points to NULL
//
// Recursive case: not at end of list
//
// Base case: at end of list


void delete_path(path_ptr head){

    path_ptr prev = NULL;       //Previous tracks the last node, so that it 
                                //can be set to NULL
 
    if (head->next ==NULL) {

	delete head;         //If list has one element, head set to NULL after
                                //deletion
	if(prev ==NULL)
	    head = NULL;        
 

	else	
	    prev->next = NULL;  //if base case is found, and the list had more 
	                    //then one element, the prevous node points to NULL 
    }
    else{
	
	if(head->next != NULL &&head->next->next == NULL)
	    
	    prev = head;         //previous equals to head if the base case is not
                                 //within two elements of head
	
	delete_path(head->next);
	if (head == prev)        //caps list with a NULL
	    
	    prev->next = NULL;
    }   
    
    return;
}

//------------------------------------------------------------
// function name: copy_list
//
// Purpose: given two linked lists, the second being empty, copies the
//          first to the secound
//
// Inputs:  the two linked lists
//
// Output:  two identical linked lists
//
// Recursive case: not at end of the complete list
//
// Base case: at end of list

void copy_list(path_ptr head, path_ptr &best_time){
    
    if (head -> next == NULL){ 
	                           //Base case, list capped with a NULL 
	best_time ->next = NULL;   
    }
    
    else {
	
	if (best_time ==NULL) {   //if the list is empty, adds the first node

	    path_ptr temp = new path;
	    best_time = temp;
	    temp->x = head->x;    //data transfered
	    temp->y = head->y;
	}
	
	head = head -> next;
	path_ptr temp = new path;  //if the list already has an element, 
	best_time -> next = temp;  //next element added to end of the list
	temp->x = head->x;
	temp->y = head->y;

	copy_list(head , best_time->next);  
	
    }
    
    return;
}



//------------------------------------------------------------
// function name: store_path
//
// Purpose: given two path lists, compares their speed, deletes the first
//          and stores the secound in another path list. This is done since
//          the travers_grid function deletes the list when it backtracks,
//          therefore, when a complete path is found, in needs to be stored  
//
// Inputs:  the two linked lists
//
// Output:  two identical lists

void store_path(path_ptr head, path_ptr &best_time) {
    
    int current_fastest = 0;
    int next_route = 0;

    current_fastest = total_time(best_time);  //checks the time of the list
    next_route = total_time(head);
    
    //if the path list was faster than the current fastest list,
    //it becomes the fastest list.
    if (next_route < current_fastest || current_fastest == 0) {

	delete_list(best_time);     //fastest list is deleted

	copy_list(head, best_time);  //fastest list becomes a copy of the 
                                     //current list
    }

    return;
}

//------------------------------------------------------------
// function name: total_time
//
// Purpose: given a path list, computes the time it takes to take
//          the path
//
// Inputs:  a linked list
//
// Output:  the time it takes to get through the list
//
// Recursive case: not at end of the path list
//
// Base case: at end of list
//
//Calculation details: time is dependent of the direction of travel
//                     along with the row/collumn. to compute time,
//                     the speed is equaled to v = d/t, where d =10
//                     and v = the current y if traveling in the x 
//                     direction or the current x if traveling in the
//                     y direction, + 1 since indicies start at 0, and 
//                     times 10 due to the diagram.

int total_time(path_ptr head) {  
    
    int time = 0;

    int old_x = 0;
    int old_y = 0;
    int new_x = 0;
    int new_y = 0;
    
    if (head == NULL || head->next == NULL)  //base case, time is 0              
	time = 0;                            //since list is empty
    else {
	
	old_x =	head -> x;      //old x,y are the previous
	old_y =	head -> y;
	
	new_x = head -> next -> x;  //new x,y are the current
	new_y = head -> next -> y;
    
  	
	if ((new_x - old_x) !=0) {  //if moving in x direction
	    time = 60/(old_y + 1);
	}

	else  {  

	    time = 60/(old_x + 1); //else must be y direction
	}
	
	time += total_time(head->next);   //time of next point is taken, 
                                          //and all is summed
    }
    
    return time;
}

//------------------------------------------------------------
// function name: delete_list
//
// Purpose: given a linked list, deletes the entire thing,
//          and sets the head of the list to NULL
//
//Recursive case: not at end of linked list.
//base case: end of linked list

void delete_list(path_ptr &best_time){  


    if(best_time!=NULL) {
	path_ptr temp = best_time;
	delete_list(best_time -> next);  //a temp pointer is set to point
	delete temp;                     //at each node
    }
                                        
    best_time = NULL;  //each temp pointer is deleted, and the head capped
                       //to Null
    return;
}
