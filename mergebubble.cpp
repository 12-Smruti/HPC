#include<iostream>
#include<omp.h>

using namespace std;

//================ SWAP FUNCTION =================
void swapValues(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

//================ PARALLEL BUBBLE SORT =================
void bubbleSort(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        int first = i % 2;

        #pragma omp parallel for shared(arr, first)
        for(int j = first; j < n - 1; j += 2)
        {
            if(arr[j] > arr[j + 1])
            {
                swapValues(arr[j], arr[j + 1]);
            }
        }
    }
}

//================ MERGE FUNCTION =================
void merge(int arr[], int low, int mid, int high)
{
    int i = low;
    int j = mid + 1;
    int k = 0;

    int temp[1000];

    while(i <= mid && j <= high)
    {
        if(arr[i] < arr[j])
        {
            temp[k] = arr[i];
            i++;
        }
        else
        {
            temp[k] = arr[j];
            j++;
        }

        k++;
    }

    while(i <= mid)
    {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while(j <= high)
    {
        temp[k] = arr[j];
        j++;
        k++;
    }

    // Copy back to original array
    for(i = low, k = 0; i <= high; i++, k++)
    {
        arr[i] = temp[k];
    }
}

//================ PARALLEL MERGE SORT =================
void mergeSort(int arr[], int low, int high)
{
    if(low < high)
    {
        int mid = (low + high) / 2;

        // Parallel sections
        #pragma omp parallel sections
        {

            #pragma omp section
            {
                mergeSort(arr, low, mid);
            }

            #pragma omp section
            {
                mergeSort(arr, mid + 1, high);
            }
        }

        merge(arr, low, mid, high);
    }
}

//================ PRINT ARRAY =================
void printArray(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }

    cout << endl;
}

//================ MAIN FUNCTION =================
int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int arr1[100], arr2[100];

    cout << "Enter array elements:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> arr1[i];

        // Copy same array for merge sort
        arr2[i] = arr1[i];
    }

    //================ BUBBLE SORT =================
    bubbleSort(arr1, n);

    cout << "\nSorted Array using Parallel Bubble Sort:\n";
    printArray(arr1, n);

    //================ MERGE SORT =================
    mergeSort(arr2, 0, n - 1);

    cout << "\nSorted Array using Parallel Merge Sort:\n";
    printArray(arr2, n);

    return 0;
}
