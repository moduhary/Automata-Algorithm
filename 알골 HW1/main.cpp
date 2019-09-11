#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <memory.h>

using namespace std;

int Selection(int* A, int p, int r, int i);
int Partition(int*& A, int p, int r);
int Checker(int* A, int rank_check, int num, int size);
int DSelect(int* A, int p, int r, int i);
int DPartition(int* &A, int p, int r, int size);
int Median(int* A, int p, int r);

int main(int argc, char* argv[]) {

    ifstream input;
    ofstream write;

    int datas;
    int rank;
    int rank_check, value, size, check, result1, result2;
    int index = 0;

    clock_t begin;
    clock_t end;
    double elapse;
    int exp = 0;
    char exp_type;

    (time(NULL));

    if(argc < 2 || argc > 4)
    {
        printf("usage: %s [number] -> generate random [number] integers to a single file.\n", argv[0]);
        printf("usage: %s [index_rank] [file] -> find [index_rank]-th smallest number in [file].\n", argv[0]);
        printf("usage: %s [index_rank] [file] [value]-> Check if [value] is [index_rank]-th smallest number in [file].\n", argv[0]);
    }

    printf("\n");

    int* dat;
    int* search;

    //write << "\n";

    if(argc == 2)
    {
        write.open("input0.txt", ofstream::out);
        size = atoi(argv[1]);

        for(index = 0; index < size; index++)
        {
            value = rand();
            write << value << " ";
        }
    }
    else {

        input.open(argv[2], ifstream::in);

        dat = (int*)calloc(10000000, 4);

        for(index = 0; ; index++)
        {
            input >> datas;

            if(input.eof())
                break;

            dat[index] = datas;
        }

        size = index;
        search = (int*)calloc((size_t)size, 4);
        memcpy(search, dat, sizeof(int)*size);

        rank = atoi(argv[1]);
        begin = clock();
        result1 = Selection(search, 0, size-1, rank);
        end = clock();

        elapse = double(end-begin)/CLOCKS_PER_SEC;

        while(elapse < 1 && exp < 3)
        {
            elapse = elapse*1000;
            exp++;
        }

        switch(exp)
        {
            case 0:
                exp_type = ' ';
                break;

            case 1:
                exp_type = 'm';
                break;

            case 2:
                exp_type = 'u';
                break;

            case 3:
                exp_type = 'n';
                break;

            default:
                exp_type = ' ';
                break;
        }

        printf("Randomized Search\n");
        printf("elapsed time: %lf %cs\n", elapse, exp_type);
        printf("rank %d, value: %d\n", rank, result1);
        printf("\n");

        printf("Randomized Search Check\n");
        printf("input: %d\n", result1);

        check = Checker(search, rank, result1, size);

        printf("checked_rank: %d\n", check);

        if(check == rank)
            printf("(Randomize) rank %d in input is %d -> true\n", rank, result1);
        else
            printf("(Randomize) rank %d in input is %d -> false\n", rank, result1);

        printf("\n");

        if(size <= 5000000) {
            memcpy(search, dat, sizeof(int)*size);
            exp = 0;
            rank = atoi(argv[1]);
            begin = clock();
            index = DSelect(search, 0, size - 1, rank);
            end = clock();
            result2 = search[index];

            elapse = double(end - begin) / CLOCKS_PER_SEC;

            while (elapse < 1 && exp < 3) {
                elapse = elapse * 1000;
                exp++;
            }

            switch (exp) {
                case 0:
                    exp_type = ' ';
                    break;

                case 1:
                    exp_type = 'm';
                    break;

                case 2:
                    exp_type = 'u';
                    break;

                case 3:
                    exp_type = 'n';
                    break;

                default:
                    exp_type = ' ';
                    break;
            }


            printf("Deterministic Search\n");
            printf("elapsed time: %lf %cs\n", elapse, exp_type);
            printf("rank %d, value: %d\n", rank, result2);
            printf("\n");

            printf("Deterministic Search Check\n");
            printf("input: %d\n", result2);

            check = Checker(dat, rank, result2, size);

            printf("checked_rank: %d\n", check);

            if (check == rank)
                printf("(Deterministic) rank %d in input is %d -> true\n", rank, result2);
            else
                printf("(Deterministic) rank %d in input is %d -> false\n", rank, result2);
        } else{
            printf("Input size is too big for deterministic search!\n");
        }

        free(dat);
        free(search);
    }

    write.close();
    input.close();

    return 0;
}

int Partition(int*& A, int p, int r)
{
    int num = A[r];
    int i = p - 1;
    int j = p;

    int temp = 0;

    while(j <= r-1)
    {
        if(A[j] <= num) {
            i++;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }

        j++;
    }

    temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp;

    return i+1;
}

int Selection(int* A, int p, int r, int i)
{
    if(p == r)
    {
        return A[p];
    }

    int q = Partition(A, p, r);
    int k = q - p + 1;

    if( i == k )
        return A[q];
    else if( i < k )
        return Selection(A, p, q-1, i);
    else
        return Selection(A, q+1, r, i-k);
}

int Checker(int* A, int rank_check, int num, int size)
{
    int rank_real = 1;
    int index = 0;

    for(index = 0; index < size; index++)
    {
        if(A[index] < num)
            rank_real++;

        //if(rank_real > rank_check)
        //return rank_real;
    }

    return rank_real;
}

int DSelect(int *A, int p, int r, int i) {

    int med, med_index, index;
    med = Median(A, p, r);

    med_index = DPartition(A, p, med, r);

    if(med_index > p + i)
        index = DSelect(A, p, med_index-1, i);
    else if(med_index < p + i)
        index = DSelect(A, med_index+1, r, i-med_index+p-1);
    else
        return med_index;

    return index;
}

int DPartition(int *&A, int p, int r, int end) {

    int num = r;
    int i = p - 1;
    int j = p;
    int pivot;

    int temp = 0;

    while(j <= end)
    {
        if(A[j] < num) {
            i++;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
        else if(A[j] == num) {
            pivot = j;

            temp = A[end];
            A[end] = A[j];
            A[j] = temp;

            if(A[j] < num) {
                i++;
                temp = A[i];
                A[i] = A[j];
                A[j] = temp;
            }
        }

        j++;
    }

    temp = A[i + 1];
    A[i + 1] = r;
    A[pivot] = temp;

    return i+1;
}

int Median(int *A, int p, int r)
{
    int pos = 0, j = 0, k, med;
    int size = r-p+1;
    int shrink = (size%5) ? size/5 + 1 : size/5;
    int med_arr[shrink];
    int least, prev_least, temp;

    if(size <= 25)
    {
        for(pos = p; pos < p + size; pos++)
        {
            least = pos;

            for(j = pos; j < p + size; j++)
            {
                if(A[pos] > A[j])
                    least = j;
            }

            if(pos != least)
            {
                temp = A[pos];
                A[pos] = A[least];
                A[least] = temp;
            }
        }

        return A[p+size/2-1];
    }

    for(pos = p; pos < p + size; pos = pos + 5)
    {
        for(j = pos; j < pos + 4; j++)
        {
            least = j;

            for(k = j; k < pos + 4; k++)
            {
                prev_least = least;
                least = (A[prev_least] < A[k + 1]) ? prev_least : k + 1;
            }

            temp = A[least];
            A[least] = A[j];
            A[j] = temp;
        }

        med_arr[(pos-p)/5] = A[pos+2];
    }

    med = Median(med_arr, 0, shrink-1);

    return med;
}
