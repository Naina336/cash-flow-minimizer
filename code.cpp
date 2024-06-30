#include <bits/stdc++.h>
using namespace std;

class bank{
    public:
    string name;
    int netAmount;
    set<string> types;
}; 

int getMinIndex(bank listNetAmounts[],int numBanks){
    int min=INT_MAX, minIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listNetAmounts[i].netAmount == 0) continue;
        
        if(listNetAmounts[i].netAmount < min){
            minIndex = i;
            min = listNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(bank listNetAmounts[],int numBanks){
    int max=INT_MIN, maxIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listNetAmounts[i].netAmount == 0) continue;
        
        if(listNetAmounts[i].netAmount > max){
            maxIndex = i;
            max = listNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int,string> getMaxIndex(bank listNetAmounts[],int numBanks,int minIndex,bank input[],int maxNumTypes){
    int max=INT_MIN;
    int maxIndex=-1;
    string matchingType;
    
    for(int i=0;i<numBanks;i++){
        if(listNetAmounts[i].netAmount == 0) continue;
        
        if(listNetAmounts[i].netAmount < 0) continue;
        
        //TODO 
        //see complexity of intersection
        
        vector<string> v(maxNumTypes);
        vector<string>::iterator ls=set_intersection(listNetAmounts[minIndex].types.begin(),listNetAmounts[minIndex].types.end(), listNetAmounts[i].types.begin(),listNetAmounts[i].types.end(), v.begin());
        
        if((ls-v.begin())!=0 && max<listNetAmounts[i].netAmount ){
            max=listNetAmounts[i].netAmount;
            maxIndex=i;
            matchingType = *(v.begin());
        } 
    }
    
    //if there is NO such max which has a common type with any remaining banks then maxIndex has -1
    // also return the common payment type
    return make_pair(maxIndex,matchingType);
}

void printAns(vector<vector<pair<int,string>>> ansGraph, int numBanks,bank input[]){
    
    cout<<"\nThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<numBanks;i++){
        for(int j=0;j<numBanks;j++){
            
            if(i==j) continue;
            
            if(ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0){
                
                if(ansGraph[i][j].first == ansGraph[j][i].first){
                    ansGraph[i][j].first=0;
                    ansGraph[j][i].first=0;
                }
                else if(ansGraph[i][j].first > ansGraph[j][i].first){
                    ansGraph[i][j].first -= ansGraph[j][i].first; 
                    ansGraph[j][i].first =0;
                    
                    cout<<input[i].name<<" pays Rs" << ansGraph[i][j].first<< "to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;
                }
                else{
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;
                    
                    cout<<input[j].name<<" pays Rs "<< ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;
                    
                }
            }
            else if(ansGraph[i][j].first != 0){
                cout<<input[i].name<<" pays Rs "<<ansGraph[i][j].first<<" to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;
                
            }
            else if(ansGraph[j][i].first != 0){
                cout<<input[j].name<<" pays Rs "<<ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;
                
            }
            
            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout<<"\n";
}

void minimizeCashFlow(int numBanks,bank input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){
    
    //Find net amount of each bank has
    bank listNetAmounts[numBanks];
    
    for(int b=0;b<numBanks;b++){
        listNetAmounts[b].name = input[b].name;
        listNetAmounts[b].types = input[b].types;
        
        int amount = 0;
        //incoming edges
        //column travers
        for(int i=0;i<numBanks;i++){
            amount += (graph[i][b]);
        }
        
        //outgoing edges
        //row traverse
        for(int j=0;j<numBanks;j++){
            amount += ((-1) * graph[b][j]);
        }
        
        listNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int,string>>> ansGraph(numBanks,vector<pair<int,string>>(numBanks,{0,""}));//adjacency matrix
    
    
    //find min and max net amount
    int numZeroNetAmounts=0;
    
    for(int i=0;i<numBanks;i++){
        if(listNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts!=numBanks){
        
        int minIndex=getMinIndex(listNetAmounts, numBanks);
        pair<int,string> maxAns = getMaxIndex(listNetAmounts, numBanks, minIndex,input,maxNumTypes);
        
        int maxIndex = maxAns.first;
        
        if(maxIndex == -1){
            
            (ansGraph[minIndex][0].first) += abs(listNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(listNetAmounts, numBanks);
            (ansGraph[0][simpleMaxIndex].first) += abs(listNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());
            
            listNetAmounts[simpleMaxIndex].netAmount += listNetAmounts[minIndex].netAmount;
            listNetAmounts[minIndex].netAmount = 0;
            
            if(listNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
            
        }
        else{
            int transactionAmount = min(abs(listNetAmounts[minIndex].netAmount), listNetAmounts[maxIndex].netAmount);
            
            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;
            
            listNetAmounts[minIndex].netAmount += transactionAmount;
            listNetAmounts[maxIndex].netAmount -= transactionAmount;
            
            if(listNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
        
    }
    
    printAns(ansGraph,numBanks,input);
    // cout<<"HI\n";
}

//correct
int main() 
{ 
    cout<<"\n\t\t\t\t********************* Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n\n";
    cout<<"This system minimizes the number of transactions among multiple banks in the different corners of the world that use different modes of payment.There is one world bank (with all payment modes) to act as an intermediary between banks that have no common mode of payment. \n\n";
    cout<<"Enter the number of banks participating in the transactions.\n";
    int numBanks;cin>>numBanks;
    
    bank input[numBanks];
    unordered_map<string,int> indexOf;//stores index of a bank
    
    cout<<"Enter the details of the banks and transactions as stated:\n";
    cout<<"Bank name ,number of payment modes it has and the payment modes.\n";
    cout<<"Bank name and payment modes should not contain spaces\n";
    
    int maxNumTypes;
    for(int i=0; i<numBanks;i++){
        if(i==0){
            cout<<"World Bank : ";
        }
        else{
            cout<<"Bank "<<i<<" : ";
        }
        cin>>input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cin>>numTypes;
        
        if(i==0) maxNumTypes = numTypes;
        
        string type;
        while(numTypes--){
            cin>>type;
            
            input[i].types.insert(type);
        }   
        
    }
    
    cout<<"Enter number of transactions.\n";
    int numTransactions;
    cin>>numTransactions;
    
    vector<vector<int>> graph(numBanks,vector<int>(numBanks,0));//adjacency matrix
    
    cout<<"Enter the details of each transaction as stated:";
    cout<<"Debtor Bank , creditor Bank and amount\n";
    cout<<"The transactions can be in any order\n";
    for(int i=0;i<numTransactions;i++){
        cout<<(i)<<" th transaction : ";
        string s1,s2;
        int amount;
        cin >> s1>>s2>>amount;
        
        graph[indexOf[s1]][indexOf[s2]] = amount;
    }
     
    //settle
    minimizeCashFlow(numBanks,input,indexOf,numTransactions,graph,maxNumTypes);
    return 0; 
} 






