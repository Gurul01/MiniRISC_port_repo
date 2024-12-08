static unsigned int *leds = 0x80;

static void swap(int* xp, int* yp);
static void bubbleSort(int arr[], int n);

static void __main(){
    while(1)
    {
        // 55 is the 3rd biggest number so it will be the arr[5]
        int arr[] = { 64, 34, 25, 12, 22, 11, 90, 55 }; 
        int n = sizeof(arr) / sizeof(arr[0]);
        bubbleSort(arr, n);

        *leds = arr[5];
    }
}

static void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// An optimized version of Bubble Sort
static void bubbleSort(int arr[], int n){
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }

        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == 0)
            break;
    }
}