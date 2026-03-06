#include <iostream>
#include <vector>
using namespace std;


int n = 5; //  Number of processes (T0, T1, T2, T3, T4)
int r = 4; //  Number of resources (A,B,C,D)

bool checkSafeState(vector<bool> Finish){
    // If Finish[i] == true for all i, then the system is in a safe state
    for(int i = 0; i < n; i++ ) {
        if(!Finish[i]) return false;
    }
    return true;
}

bool lessEqual(vector<int> a, vector<int> b){
    // Let a and b be vectors of lenght n. We say that a<=b if and only if a[i]<=b[i] for all i = 1, 2, ..., n.
    for(int i = 0; i < r; i++){
        if(a[i] > b[i]) return false;
    }
    return true;

}

bool safetyAlgorithm(vector<int> Available, vector<vector<int>>& Allocation,
                    vector<vector<int>>& Need, vector<int>& safeSeq) {

    //  Let Work be a vector of lenght m. Initialize Work = Available
    vector<int> Work = Available;

    // Let Finish be a vector of length n. Initialize Finish[i]=false for i=0,1,...,n-1
    vector<bool> Finish(n, false);
    
    bool keep;
    do{
        // Find an index i such that both Finish[i]=false and Need[i] <= Work. If no such i exists, go to checkSafeState.
        keep = false;
        int i = 0;
        for(; i < n ; i++){
            if (Finish[i] == false && lessEqual(Need[i], Work)){
                keep = true;
                break;
            }
        }
        if(keep){
            // Work = Work + Allocation[i]
            for(int j = 0; j < r; j++) Work[j] += Allocation[i][j];

            Finish[i] = true;

            // Construct the safe sequence - Order in wich the threads may complete
            safeSeq.push_back(i);
        }
    } while(keep);

    return checkSafeState(Finish);
}

bool resourceRequest(int thread, vector<int>& request, vector<int>& Available,
                    vector<vector<int>>& Allocation, vector<vector<int>>& Max,
                    vector<vector<int>>& Need) {
    
    // If Request <= Need[i] continue. Otherwise, raise an error condition, since the thread has exceeded its maximun capacity.
    if(!lessEqual(request, Need[thread])){
        cout << "Error: request exceeds Need for T" << thread << endl;
        return false;
    }

    // If Request <= Available, continue. Otherwise, the thread must wait, since the resources are not available.
    if(!lessEqual(request, Available)){
        cout << "Error: request exceeds Available for T" << thread << endl;
        return false;
    }

    // Make the system pretend to have allocated the requested resources to thread Ti by modifing the state as follows:
    // Available = Available - Request;
    for (int j = 0; j < r; j++) Available[j] -= request[j];

    // Allocation[i] = Allocation[i] + Request;
    for (int j = 0; j < r; j++) Allocation[thread][j] += request[j];

    // Need[i] = Need[i] - Request;
    for (int j = 0; j < r; j++) Need[thread][j] -= request[j];

    
    // If the resulting resource-allocation state is safe, the transaction is completed, and thread Ti is allocated its resources
    vector<int> safeSeq;
    if (safetyAlgorithm(Available, Allocation, Need, safeSeq)) {
        for (int i = 0; i < n; i++)
            cout << "T" << safeSeq[i] << (i < n - 1 ? " -> " : "\n");
        return true;
    }
    // However, if the new state is unsafe, then Ti must wait for Request, and the old resource-allocation state is restored
    
    for (int j = 0; j < r; j++) {
        Available[j] += request[j];
        Allocation[thread][j] -= request[j];
        Need[thread][j] += request[j];
    }

    
    return false;
}


void cli(vector<int> Available, vector<vector<int>>& Allocation,
        vector<vector<int>>& Need, vector<vector<int>>& Max ){

    int op;
    do {
        cout << "\n1. Safety Algorithm\n2. Resource Request\n0. Exit\nOption: ";
        cin >> op;
        if (op == 1) {
            vector<int> safeSeq;
            if (safetyAlgorithm(Available, Allocation, Need, safeSeq)) {
                cout << "System is SAFE. Sequence: ";
                for (int i = 0; i < n; i++)
                    cout << "T" << safeSeq[i] << (i < n - 1 ? " -> " : "\n");
            } else {
                cout << "System is UNSAFE"<< endl;
            }
        } else if (op == 2) {
            int thread;
            cout << "Thread (0-" << n-1 << "): "; 
            cin >> thread;

            // Let Request be the request vector for thread T_i
            vector<int> Request(r);
            cout << "Request (" << r << " values): ";

            // The thread wants Request[j] instances of resource type j
            for (int j = 0; j < r; j++) 
                cin >> Request[j];
            
            if(resourceRequest(thread, Request, Available, Allocation, Max, Need))
                cout << "Is a safe sequence. Request granted" << endl;
            else
                cout << "Request denied: would leave system in unsafe state" << endl;
        }
    } while (op != 0);
    cout << "Exiting..." << endl;
}
int main() {
    
    // Available: A vector of lenght m indicates the number of available resources of each type
    vector<int> Available = {2, 2, 2, 3};

    // Max: An nxm matrix defines the maximun demand of each thread
    vector<vector<int>> Max = {{4, 3, 1, 6}, // T0 
                                    {2, 4, 2, 4}, // T1
                                    {3, 6, 5, 1}, // T2
                                    {2, 6, 2, 3}, // T3
                                    {3, 1, 1, 2}}; // T4

    // Allocation: An n x m matrix that defines the number of resources of each type currently allocated to each thread
    vector<vector<int>> Allocation = {{1, 2, 0, 2}, // T0
                                      {0, 1, 1, 2}, // T1
                                      {1, 2, 4, 0}, // T2 
                                      {1, 2, 0, 1}, // T3
                                      {1, 0, 0, 1}}; // T4

    // Need: An n x m matrix indicates the remaining resource need of each thread
    vector<vector<int>> Need(n, vector<int>(r));
    
    // Need[i][j] = Max[i][j] - Allocation[i][j]
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < r; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }


    // Testing for different Available sequences:
    vector<vector<int>> testCases = {
        {2, 2, 2, 3},
        {4, 4, 1, 1},
        {3, 0, 1, 4},
        {1, 5, 2, 2}
    };

    for(vector<int> test : testCases){
        cout << "\nTesting with Available = [";
        for(int i = 0; i < r; i++)
            cout << test[i] << (i < r-1 ? ", " : "]\n");
        cli(test, Allocation, Need, Max);
    }  
    
    return 0;
}
