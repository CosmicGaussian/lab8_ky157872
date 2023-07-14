#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// Swap two elements in the array
void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Heapify the subtree rooted at index i in the array of size n
void heapify(int arr[], int n, int i) {
    int largest = i;  // Initialize largest as root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr, i, largest);
        heapify(arr, n, largest);
    }
}

// Implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) {
    extraMemoryAllocated = 0;
  
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Heap sort
    for (int i = n - 1; i >= 0; i--) {
        swap(arr, 0, i);
        heapify(arr, i, 0);
    }
}

// Merge two subarrays of arr[].
// First subarray is pData[l..m]
// Second subarray is pData[m+1..r]
void merge(int pData[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];  // Temporary arrays

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

    // Merge the temporary arrays back into pData[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i];
            i++;
        } else {
            pData[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        pData[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        pData[k] = R[j];
        j++;
        k++;
    }
}

// Implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
    if (l < r) {
        extraMemoryAllocated = 0;
        int m = l + (r - l) / 2;

        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);

        merge(pData, l, m, r);
    }
}

// Parses input file to an integer array
int parseData(char *inputFileName, int **ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n, *data;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int *)malloc(sizeof(int) * dataSz);

        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i) {
            fscanf(inFile, "%d ", &n);
            data = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    for (i = sz; i < dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void) {
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = { "lab8input1.txt", "lab8input2.txt", "lab8input3.txt", "lab7input4.txt" };

    for (i = 0; i < 4; ++i) {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int *)malloc(sizeof(int) * dataSz);

        printf("---------------------------\n");
        printf("Dataset Size : %d\n", dataSz);
        printf("---------------------------\n");

        printf("Heap Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        heapSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        free(pDataCopy);
        free(pDataSrc);
    }

    return 0;
}