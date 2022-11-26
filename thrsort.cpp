/**
1. Brynn McGovern
   2370579
   bmcgovern@chapman.edu
   CPSC 380-01
   Assignment 3 - Threaded Sorting
2. A program to sort an array of numbers using two threads, then a third thread
  joins the two threads together. The program creates an array of random numbers and sorts it.
*/
#include <iostream>
#include <pthread.h>

// number of elements in array
#define MAX 20
using namespace std;

//special structure to make merge sort easier
typedef struct Array{
  int low;
  int high;
}Array;

// array for unsorted numbers then sorted numbers
int unsortedList[MAX];

/**
  mergeThreads()
  @param int low, int mid, int high
  @return void
  Function takes in int parameters for splitting the array of numbers
  Splits array into two separate arrays and sorts those separately, then joins them
*/
void mergeThreads(int low, int mid, int high)
{
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 =  high - mid;

    int leftSide[n1], rightSide[n2];
    for (i = 0; i < n1; i++){
      leftSide[i] = unsortedList[low + i];
    }

    for (j = 0; j < n2; j++){
      rightSide[j] = unsortedList[mid+ 1+ j];
    }


    i = 0;
    j = 0;
    k = low;
    while (i < n1 && j < n2)
    {
      if (leftSide[i] <= rightSide[j])
      {
         unsortedList[k] = leftSide[i];
         i++;
       }
      else
      {
         unsortedList[k] = rightSide[j];
         j++;
      }
      k++;
    }
    while (i < n1)
    {
      unsortedList[k] = leftSide[i];
      i++;
      k++;
    }

    while (j < n2)
    {
      unsortedList[k] = rightSide[j];
      j++;
      k++;
   }
}

/**
  mergeSort()
  @param void* arg
  @return void*
  This function uses special structure Array, then creates two threads using
  low and high values from Array. It uses mergeThreads to sort and merge the threads,
  then joins the threads.
*/
void* mergeSort(void* arg)
{
  Array a = *((Array*)arg);
  int low = a.low;
  int high = a.high;

  if (low < high)
  {
      int mid = low+(high-low)/2;

      pthread_t thread1;
      Array a1 = {low, mid};

      pthread_t thread2;
      Array a2 = {mid+1, high};

      pthread_create(&thread1, NULL, mergeSort, &a1);
      pthread_create(&thread2, NULL, mergeSort, &a2);


      pthread_join(thread1, NULL);
      pthread_join(thread2, NULL);
      mergeThreads(low, mid, high);

  }
}

/**
  main()
  Randomizes numbers for the unsorted array and displays those values.
  Then it creates a single thread to join the two threads from mergeSort().
  Lastly it displays the sorted array.
*/
int main()
{
    for (int i = 0; i < MAX; i++){
        unsortedList[i] = rand() % 100;
    }
    cout << "Unsorted array: ";

    for(int i = 0; i < MAX; i++){

      cout << unsortedList[i] << " ";

    }
    cout << endl;
    Array arr = {0, MAX};

    pthread_t t;
    pthread_create(&t, NULL, mergeSort, &arr);
    pthread_join(t, NULL);

    cout << "Sorted array: ";
    for (int i = 0; i < MAX; i++)
        cout << unsortedList[i] << " ";
    cout << endl;

    return 0;
}
