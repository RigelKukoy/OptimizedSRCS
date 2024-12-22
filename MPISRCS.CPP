// SOURCE CODE
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>
using namespace std;
using namespace std::chrono;
void CF(vector<int> &Count, vector<int> &CM, vector<int> &Map, int mx)
{
    int j = 1;
    CM[0] = 0;
    for (int i = 1; i <= mx; ++i)
    {
        if (Count[i] > 0)
        {
            CM[j] = CM[j - 1] + Count[i];
            Count[i] = j;
            Map[j] = i;
            j++;
        }
    }
}
void BN(vector<int> &A, vector<int> &BK, vector<int> &Count, vector<int> &CM, vector<int> &Map, vector<int> &P, int N)
{
    fill(P.begin(), P.end(), 0);
    for (int i = 0; i < N; ++i)
    {
        int a = static_cast<int>(sqrt(A[i]));
        int dx = Count[a];
        BK[P[dx] + CM[dx - 1]] = A[i] - Map[dx] * Map[dx];
        P[dx]++;
    }
}
void GS(vector<int> &A, vector<int> &BK, vector<int> &C, vector<int> &Map, vector<int> &CM,
        int d, int &m)
{
    int start = CM[d - 1];
    int end = CM[d];

    int k = *max_element(BK.begin() + start, BK.begin() + end);
    fill(C.begin(), C.end(), 0);
    for (int i = start; i < end; ++i)
    {
        C[BK[i]]++;
    }
    for (int i = 0; i <= k; ++i)
    {
        while (C[i] > 0)
        {
            A[m++] = i + Map[d] * Map[d];
            C[i]--;
        }
    }
}
void SRCS(vector<int> &A)
{
    int N = A.size();
    if (N <= 1)
        return;
    int mx = 0;
    for (int i = 0; i < N; ++i)
    {
        int sqrtA = static_cast<int>(sqrt(A[i]));
        mx = max(mx, sqrtA);
    }
    vector<int> Count(mx + 1, 0);
    vector<int> Map(mx + 1, 0);
    for (int i = 0; i < N; ++i)
    {
        int sqrtA = static_cast<int>(sqrt(A[i]));
        Count[sqrtA]++;
    }
    vector<int> CM(mx + 1, 0);
    CF(Count, CM, Map, mx);
    vector<int> P(mx + 1, 0);
    vector<int> BK(N);
    BN(A, BK, Count, CM, Map, P, N);
    int m = 0;
    vector<int> C(*max_element(BK.begin(), BK.end()) + 1, 0);
    for (int i = 1; i <= mx; ++i)
    {
        if (P[i] > 0)
        {

            GS(A, BK, C, Map, CM, i, m);
        }
    }
}
void parallel_merge(vector<int> &local_arr, int rank, int size, MPI_Comm comm)
{
    int local_size = local_arr.size();
    for (int step = 1; step < size; step *= 2)
    {
        if (rank % (2 * step) == 0)
        {
            if (rank + step < size)
            {
                int recv_size;
                MPI_Status status;
                MPI_Probe(rank + step, 0, comm, &status);
                MPI_Get_count(&status, MPI_INT, &recv_size);
                vector<int> recv_arr(recv_size);
                MPI_Recv(recv_arr.data(), recv_size, MPI_INT, rank + step, 0, comm,
                         MPI_STATUS_IGNORE);
                vector<int> temp(local_size + recv_size);
                merge(local_arr.begin(), local_arr.end(), recv_arr.begin(), recv_arr.end(),
                      temp.begin());
                local_arr.swap(temp);
                local_size = local_arr.size();
            }
        }
        else
        {
            int dest = rank - step;
            MPI_Send(local_arr.data(), local_size, MPI_INT, dest, 0, comm);
            break;
        }
    }
}
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    vector<int> arr;
    int N = 0;

    if (rank == 0)
    {
        // Open the input file
        ifstream inputFile("data1.txt");
        if (!inputFile)
        {
            cerr << "Error opening input file!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        else
        {
            cout << "Input file opened successfully." << endl;
        }
        // Read data from the file
        int num;
        while (inputFile >> num)
        {
            arr.push_back(num);
        }
        inputFile.close();
        if (arr.empty())
        {
            cerr << "Input file is empty or contains no valid data!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        else
        {
            cout << "Input file read successfully. Number of elements: " << arr.size() << endl;
        }
        N = arr.size();
    }
    // Broadcast the size of the array to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Adjust the array size for non-root processes
    if (rank != 0)
    {
        arr.resize(N);
    }
    // Distribute the data among processes
    vector<int> local_arr(N / size);
    MPI_Scatter(arr.data(), N / size, MPI_INT, local_arr.data(), N / size, MPI_INT, 0,
                MPI_COMM_WORLD);
    // Apply SRCS algorithm on local data

    auto start = steady_clock::now();
    SRCS(local_arr);
    // Perform parallel merge
    parallel_merge(local_arr, rank, size, MPI_COMM_WORLD);
    if (rank == 0)
    {
        auto end = steady_clock::now();
        duration<double> elapsed_seconds = end - start;
        // Debug output: Display the sorted array

        // Print the exact execution time
        cout << "Execution time (sorting and merging): " << fixed << setprecision(9) << elapsed_seconds.count() << "s" << endl;
    }
    MPI_Finalize();
    return 0;
}