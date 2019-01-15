//
//  main.cpp
//  Gallegos_Michael_PA4
//
//  Created by Michael Gallegos on 11/20/18.
//  Copyright © 2018 Michael Gallegos. All rights reserved.
//
/*
 Description:
 For this programming assignment, you will implement several different sorting algorithms and study their performance.
 You will implement (1) Bubble Sort, (2) Heap Sort, (3) Merge Sort, and (4) (deterministic) Quick Sort.
 We will test your implementations using a set of numbers in a text file named “numbers.txt”.
 Each line of the file will contain a number.
 The first number will be which sorting algorithm to use (1=Bubble Sort, 2=Heap Sort, 3=Merge Sort, 4=Quick Sort).
 The second number will be the number of remaining elements in the file, n.
 The first two numbers of the file (sorting algorithm and n) will NOT be sorted.
 However, the remaining n numbers will be sorted.
 Here is an example numbers.txt file.
 You should already have a heap working from the previous assignment.
 However, if you do not, here is an implementation of a heap.
 You should print the sorted set of numbers to the screen.
 Using your four implementations, you will also time how long it takes to sort n numbers.
 You should test your sorts on three different types of inputs: sorted, the reverse of sorted order, and random.
 You may use a random number generator as opposed to the file input above.
 Time how long sorting takes using the StopWatch class we provided (you don’t need to print the numbers when timing).
 You should measure the total time at different values of n.
 You will then show a graph of the sort time versus the number of elements for each implementation and type of input.
 */

#include <iostream>
#include "Timer.h"
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class EmptyStackException: public std::runtime_error{
    // Create a class for empty stack exception for later
public:
    EmptyStackException() : runtime_error("Empty Stack Exception"){}
};

void swap(long int &x, long int &y) {
    long int temp_x = x; //Create a temp variable to hold x, switch x, then apply y
    x = y; //This is from a 121 Lab I did
    y = temp_x;
}

/*
    Here I learned from a Youtube video about max heap bubbles.
    I will be using the sort by setting the max to the root, then sorting that to the end of the array.
    After this, remove the max node, re-build the heap, and use recursion to go again.
    Do this as many times as necessary to get a sorted array in ascending order.
    Now I have to actually implement this with code, but the Youtube video really helped clarify the process.
*/

long int rHeap(long int i) { //This returns the right of the node
    return 2*i+2;
};
long int lHeap(long int i) { //This returns the left of the node
    return 2*i+1;
};
void heap(long int arr[], long int n, long int top)
{
    long int currentMax = top; // Initialize currentMax as root
    long int l = lHeap(top); // left = 2*i + 1
    long int r = rHeap(top); // right = 2*i + 2
    // Set the current biggest number found to the left in case it's bigger than the current max being looked at
    if (l < n && arr[l] > arr[currentMax]) {
        currentMax = l;
    }
    // Now set the current max to the right node being looked at if it's larger.
    if (r < n && arr[r] > arr[currentMax]) {
        currentMax = r;
    }
    // Now this checks that the current max is NOT the root node currently being looked at.
    if (currentMax != top) { // If it's not, you'll want to swap it around and re-heap it
        swap(arr[top], arr[currentMax]);
        heap(arr, n, currentMax);
    }
}

void heapSort(long int arr[], long int n)
{
    //First build the heap, going downwards until i = 0
    for (long int i = (n / 2 - 1); i >= 0; i--){
        heap(arr, n, i);
    }
    // Now you're removing the root from the heap and throwing it at the end of the array, thus sorting it
    for (long int i=n-1; i>=0; i--) {
        //Swap the current root with the end of the array and re-heap it
        swap(arr[0], arr[i]);
        heap(arr, i, 0);
    }
}
/*
    Perfect, the heap sort works as it should, now onto Merge sort.
    First get the middle point (Where you want to divide the two portions).
    Then call a sort for the first half, then the second half, merge them together afterwards.
*/

void merge(long int arr[], long int left, long int mid, long int right)
{
    
    // The first step is creating a couple temp arrays with the data inside them
    // Cut them into two, left/right, then you can merge them together with comparisons
    // Use if statements to check when merging back together
    long int sizel = (mid + 1) - left; // Move one over
    long int sizer =  right - mid; // Size
    long int leftArr[sizel];
    long int rightArr[sizer];
    for (long int i = 0; i < sizel; i++)
        leftArr[i] = arr[left + i];
    for (long int i = 0; i < sizer; i++)
        rightArr[i] = arr[mid + 1+ i];
    //Now you want to get them merged back together.
    
    // Merge the temp arrays back into arr[l..r]
    long int l = 0; // Initial index of first subarray
    long int r = 0; // Initial index of second subarray
    long int k = left; // Initial index of merged subarray
    while (l < sizel && r < sizer)
    {
        if (leftArr[l] <= rightArr[r]) { // While the one from the left is less than or equal to the right portion, put it first
            arr[k] = leftArr[l];
            // Okay, now increment the left one and the array spot shifts
            l++;
            k++;
        }
        else { // Otherwise, go ahead and mess around and make the one from the right portion go first
            arr[k] = rightArr[r];
            // Afterwards, increment y and k one, so you can look at the next element on the right and the next spot in the array
            r++;
            k++;
        }
    }
    while(l < sizel) { // I used a for loop here but Xcode recommended using while
        // While you're not done with the left side by itself, make the array merge
        arr[k] = leftArr[l];
        k++;
        l++;
    }
    while(r < sizer) { // While you're not done with the right side by itself, merge
        arr[k] = rightArr[r];
        k++; // Add one to the location
        r++; // Mess one with the next right
    }
}

void mergeSort(long int arr[], long int left, long int right)
{
    if (left < right) {
        //Set a condition, or else the function will keep calling itself over and over. Xcode yelled at me over this...
        // Sort first and second halves, set a mid variable by taking the average
        long int mid = ((left + right) / 2);
        mergeSort(arr, left, mid); // Merges the left
        mergeSort(arr, mid + 1, right); //Merges the right
        merge(arr, left, mid, right); // Merge everything together!
    }
}

void quickSort(long int data[], long int left, long int right) {
    long int l = left;
    long int r = right;
    // Define the variables so we can edit them later
    // Setup a mid variable to set as your midpoint for the left/right arrays
    long int mid = data[(left + right) / 2];
    // Temp variable will be used when we want to make a new array
    // This array will be the broken up portion
    long int temp;
    // Use this temp later but first make it a variable outside of the loop
    // Now we want to break it up into parts! Wooho!
    while (l <= r) { // While the left portion is less than or equal to the right
        // Array(s) should look like this [Left Mid Right]
        while (data[l] < mid) // While you're still approaching the midpoint, increment the left part one
            l++;
        while (mid < data[r]) // While you're looking at the right side of the array, go down one to approach the mid
            r--;
        if (l <= r) {// Set this up in a separate loop
            temp = data[l]; // temp = array[left]
            swap(data[l], data[r]);
            data[r] = temp; // The right is now the temp, the left spot is now what's at the right, switching it up a little.
            l++; r--; // Decrement the left and right temp vars
        }
    };
    if (l < right) // If the current left is less than the original right, sort it again
        // This is the recursive step
        quickSort(data, l, right);
    if (left < r) // If the original left is less than the right, sort it out
        quickSort(data, left, r);
}

void printVector(long int arr[], long int size) {  // Learned this from techiedelight.com
    for (long int i = 0; i < size; i++)
        cout << arr[i] << endl;
}

int main(int argc, const char * argv[]) {
    
    /* First setup your input array. The first element is going to be the type, the second is going to be the number of elements.
     The rest of the is your values.
     For the sake of demonstration, I will use long ints. */
    
    // I used the below to generate the list of numbers to sort
    
//    ofstream ofs("/Users/Snowspeeder/Documents/School/CSCE 221/PA4/Gallegos_Michael_PA4/Gallegos_Michael_PA4/numberstxt.txt");
//    for (long int i = 80000; i > 0; i--)
//                //ofs << rand() % 500 + 1 << endl;
//                ofs << i + 2 << endl;
//            ofs.close();
    
    string line; // These will be used for pushing the input to a vector.
    vector<long int> inputVector;

//  ifstream myfileinput("/Users/Snowspeeder/Documents/School/CSCE 221/PA4/Gallegos_Michael_PA4/Gallegos_Michael_PA4/numberstxt.txt");
/*
The above line is commented out, but this is what I used to test the code.
It works when the file path is hard coded, but the code right below this comment asks for the file path as user input.
*/
            string nameOfFile;
            cout << "Please input the file path: ";
            cin >> nameOfFile;
            cout << endl;
            ifstream myfileinput(nameOfFile);
    // Now push the file to a vector.
    while(getline(myfileinput,line)){ // Make each vector element a line from the file
        inputVector.push_back (stol(line));
    }
    myfileinput.close(); // close the file
    //printVector(inputVector);
    long int dataVector[inputVector[1]];
    for (long int i = 0; i < inputVector[1]; i++){
        dataVector[i] = (inputVector[i+2]);
    }
    // printVector(dataVector);
    cout << "The list you inputted has " << inputVector[1] << " elements that will be sorted." << endl;
    // Time to start the timer!
    CStopWatch j;
    // Now setup the different usage cases
    if (inputVector[0] == 1) { // Bubble Sort
        long int swapOccur = 1; // Set a variable to indicate whether or not you need to keep going in the foor loop
        // Initially it's set to 1, to kick off the loop.
        for (long int i = 1; i <= inputVector[1] && swapOccur > 0; i++) {
            //Check if you've gone through all the elements AND there's nothing else to swap around
            swapOccur = 0; //Set it back to 0, it'll be set to 1 if there
            for (long int x=0; x < (inputVector[1] - 1); x++) { //Go until you've gone n-1 times
                if (dataVector[x+1] < dataVector[x]) { //If the elements need to be bubbled, swap them
                    swap(dataVector[x+1], dataVector[x]);
                    swapOccur++;
                }
            }
        }
        cout << "Time taken for Bubble Sort: " << j.GetElapsedSeconds() << " seconds." << endl;
        // printVector(dataVector, inputVector[1]);
    }
    
    else if (inputVector[0] == 2) { // Heap sort
        heapSort(dataVector, inputVector[1]);
        cout << "Time taken for Heap Sort: " << j.GetElapsedSeconds() << " seconds." << endl;
       // printVector(dataVector, inputVector[1]);
    }
    else if (inputVector[0] == 3) { // Merge sort
        long int start = inputVector[1];
        mergeSort(dataVector, 0, start - 1);
        cout << "Time taken for Merge Sort: " << j.GetElapsedSeconds() << " seconds." << endl;
        //printVector(dataVector, inputVector[1]);
    }
    else if (inputVector[0] == 4) { // Quick sort
        long int start = inputVector[1];
        quickSort(dataVector, 0, start - 1);
        //printVector(dataVector, inputVector[1]);
        cout << "Time taken for Quick Sort: " << j.GetElapsedSeconds() << " seconds." << endl;
    }
    else //Otherwise it's an invalid file
        throw EmptyStackException();
    //std::cout << "Hello, World!\n";
    return 0;
}
