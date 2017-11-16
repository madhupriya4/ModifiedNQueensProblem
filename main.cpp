#include<iostream>
#include<vector>
#include<algorithm>
#include<deque>
#include<fstream>
#include<string>
#include<list>
#include<stdlib.h>
#include <cstdlib>
#include<time.h>
#include<math.h>
#include <limits.h>
#include <sstream>
#include<chrono>

using namespace std;

void input_board(vector< vector <int>>& vec,int&p,int& trees,int& method)
{

    ifstream myfile ("input.txt");
    string l;
    getline(myfile,l);
    if(l=="DFS")
        method=1;
    else if(l=="BFS")
        method=2;
    else
        method=3;

    getline(myfile,l);

    stringstream num(l);
    int n = 0;
    num >> n;

    getline(myfile,l);
    stringstream liz(l);
    liz >> p;


    vector <vector<int>> board (n, vector<int>(n));
    int i=0;

    while(getline(myfile,l))
    {
        for(int j=0;j<n;j++)
        {
            board[i][j]=(int)l.at(j)-48;
            if(board[i][j]==2)
                trees++;
        }
        i++;
    }

    vec=board;
    myfile.close();

}

void printSolution(vector <vector<int>>& board,ofstream & myfile)
{
    int n=board.size();

    myfile<<"OK"<<endl;
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
             int flag=0;
             if(board[i][j]==-1)
             {
                flag=1;
                board[i][j]=0;
             }

             myfile<<board[i][j];
             if(flag)
                board[i][j]=-1;
        }
        myfile<<endl;
    }
}

bool isSafe(vector<vector<int>>& board,int row,int col)
{
    int i,j,n=board.size();

    if(board[row][col])
        return false;

    for (j = col;j>=0 && board[row][j]!=2;j--)
        if (board[row][j]==1)
            return false;

    for (i=row,j=col; i>=0 && j>=0 && board[i][j]!=2; i--,j--)
        if (board[i][j]==1)
            return false;

    for (i=row,j=col; j>=0 && i < n && board[i][j]!=2; i++, j--)
        if (board[i][j]==1)
            return false;


    for (i = row;i>=0 && board[i][col]!=2;i--)
        if (board[i][col]==1)
            return false;


    for (i = row;i<n && board[i][col]!=2;i++)
        if (board[i][col]==1)
            return false;

    for (j = col;j<n && board[row][j]!=2;j++)
        if (board[row][j]==1)
            return false;

    for (i=row,j=col; i>=0 && j<n && board[i][j]!=2; i--,j++)
        if (board[i][j]==1)
            return false;

    for (i=row,j=col; j<n && i < n && board[i][j]!=2; i++, j++)
        if (board[i][j]==1)
            return false;

    return true;
}

void clearGraph(vector<vector <int>> &board)
{
    int n=board.size();
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
         {
             if(board[i][j]!=2)
                board[i][j]=0;
         }
}

int countConflicts(vector<pair<int,int>>lizards,vector<vector <int>> board,int p)
{
    int n=board.size();
    int conflicts=0;

    for(int w=0;w<lizards.size();w++)
    {
        int row=lizards[w].first;
        int col=lizards[w].second;

        board[row][col]=0;
        int i,j;


        for (j = col;j>=0 && board[row][j]!=2;j--)
            if (board[row][j]==1)
                conflicts++;

        for (i=row,j=col; i>=0 && j>=0 && board[i][j]!=2; i--,j--)
            if (board[i][j]==1)
                conflicts++;

        for (i=row,j=col; j>=0 && i < n && board[i][j]!=2; i++, j--)
            if (board[i][j]==1)
                conflicts++;

        for (i = row;i>=0 && board[i][col]!=2;i--)
            if (board[i][col]==1)
                conflicts++;

        for (i = row;i<n && board[i][col]!=2;i++)
            if (board[i][col]==1)
                conflicts++;

        for (j = col;j<n && board[row][j]!=2;j++)
            if (board[row][j]==1)
                conflicts++;

        for (i=row,j=col; i>=0 && j<n && board[i][j]!=2; i--,j++)
            if (board[i][j]==1)
                conflicts++;

        for (i=row,j=col; j<n && i < n && board[i][j]!=2; i++, j++)
            if (board[i][j]==1)
                conflicts++;

        board[row][col]=1;

    }
    return conflicts/2;
}
bool solveZooUtilSA(vector<vector<int>>& board,int p)
{
    try{
    srand(time(NULL));
    int n=board.size();

    vector<pair<int,int>> lizards;

    auto start_time=chrono::high_resolution_clock::now();

    for(int i=0;i<p;i++)
    {

        auto end_time=chrono::high_resolution_clock::now();

        if(std::chrono::duration<double, std::milli>(end_time-start_time).count()>275000)
            return false;

        int x = rand() % n;
        int y = rand() % n;
        if(board[x][y])
        {
            i--;
            continue;
        }
        board[x][y]=1;
        lizards.push_back(make_pair(x,y));

    }

    int conflicts=countConflicts(lizards,board,p);

    if(conflicts==0)
        return true;

    float temp=1;
    float d=1.5;
    while(conflicts!=0)
    {
        for(int i=0;i<p;i++)
        {
            double T=temp/log(n+d);
            d+=0.005;

            auto end_time=chrono::high_resolution_clock::now();

            if(std::chrono::duration<double, std::milli>(end_time-start_time).count()>275000)
                return false;

            int x=lizards[i].first;
            int y=lizards[i].second;

            int a = rand() % n;
            int b = rand() % n;

            if(board[a][b])
                continue;

            board[x][y]=0;
            board[a][b]=1;

            lizards[i]=make_pair(a,b);


            int newConflicts=countConflicts(lizards,board,p);

            if(newConflicts==0)
                return true;
            double prob=exp((double)(conflicts-newConflicts)/T);
            if(prob>=1)
                conflicts=newConflicts;
            else
            {
                double r = ((double) rand() / (RAND_MAX)) ;
                if(prob<=r)
                {
                    board[x][y]=1;
                    board[a][b]=0;
                    lizards[i]=make_pair(x,y);
                }
                else
                    conflicts=newConflicts;
            }
        }
        temp=temp*0.999;
    }
    return false;
    }
    catch(...)
    {
        return false;
    }

}
bool solveZooUtilDFS(vector<vector<int>>& board,int p)
{

    int n=board.size();
    deque <pair<int,int>> frontier;
    deque <pair<int,int>> explored;

    auto start_time=chrono::high_resolution_clock::now();

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            clearGraph(board);
            frontier.clear();
            explored.clear();
            frontier.push_back(make_pair(i,j));

            while(1)
            {
                auto end_time=chrono::high_resolution_clock::now();

                if(std::chrono::duration<double, std::milli>(end_time-start_time).count()>275000)
                    return false;

                if(frontier.empty() && !explored.empty() )
                {
                    pair<int,int> a=explored.back();
                    explored.pop_back();
                    board[a.first][a.second]=-1;


                    for(int u=a.second+1;u<n;u++)
                        for(int v=0;v<n;v++)
                            if(board[v][u]!=2 && board[v][u]!=1)
                            board[v][u]=0;

                    for(int w=a.first;w<n;w++)
                        frontier.push_back(make_pair(w,a.second));

                    for(int w=0;w<n;w++)
                        frontier.push_back(make_pair(w,(a.second+1)%n));
                }
                if(frontier.empty())
                    break;

                pair <int,int> w=frontier.front();
                frontier.pop_front();

                if(isSafe(board,w.first,w.second)&&w!=explored.front())
                {

                    explored.push_back(w);
                    board[w.first][w.second]=1;

                    if(explored.size()==p)
                        return true;
                }
                else
                    continue;

                for(int k=w.first+1;k<n;k++)
                    frontier.push_back(make_pair(k,(w.second)));

                for(int k=0;k<n;k++)
                    frontier.push_back(make_pair(k,(w.second+1)%n));

            }
        }
    }
    return false;
}

bool solveZooUtilBFS(vector<vector<int>>& board,int & p)
{

    int n=board.size();
    deque<list<pair<int,int>>> frontier;
    auto start_time=chrono::high_resolution_clock::now();


    int count=0,flag=1;
    for(int i=0;i<n && flag;i++)
        for(int j=0;j<n;j++)
        {

            if(count==p)
            {
                flag=0;
                break;
            }

            if(board[i][j])
                continue;


            count++;

            list <pair<int,int>> l;
            l.push_back(make_pair(i,j));
            frontier.push_back(l);
        }

    while(!frontier.empty())
    {
        auto end_time=chrono::high_resolution_clock::now();

        if(std::chrono::duration<double, std::milli>(end_time-start_time).count()>275000)
            return false;

        list <pair<int,int>> l=frontier.front();
        frontier.pop_front();

        if(l.size()==p)
            return true;


        list<pair<int,int>>::const_iterator iterator;
        for (iterator = l.begin(); iterator != l.end(); ++iterator)
        {
            pair<int,int> temp= *iterator;
            int i=temp.first,j=temp.second;
            board[i][j]=1;
        }



        pair<int,int> start=l.front();
        int i=start.first,j=start.second;


        int temp=(j+1)%n;

        for(int row=i,u=0;row<n;row++,u++)
        {
                for(int col=temp,w=0;w<n;col=(col+1)%n,w++)
                {

                    if(board[row][col])
                        continue;

                    if(isSafe(board,row,col))
                    {
                        board[row][col]=1;
                        l.push_front(make_pair(row,col));
                        if(l.size()==p)
                            return true;
                        frontier.push_back(l);
                        l.pop_front();
                        board[row][col]=0;


                    }
                }
                temp=0;
        }
        list<pair<int,int>>::const_iterator it;
        for (it = l.begin(); it != l.end(); ++it)
        {
            pair<int,int> temp= *it;
            int i=temp.first,j=temp.second;
            board[i][j]=0;
        }
   }
    return false;
}



void solveZoo(vector<vector<int>>& board,int& trees,int p,ofstream & myfile,int method)
{
    int n=board.size();
    if (p>n&&!trees)
    {
        myfile<<"FAIL"<<endl;
        return;
    }
    if(n==1 && !trees)
    {
        myfile<<p;
        return;
    }

    bool res=false;
    switch(method)

    {
        case 1: res=solveZooUtilDFS(board,p);
                break;
        case 2: res=solveZooUtilBFS(board,p);
                break;
        case 3: res=solveZooUtilSA(board,p);
                break;
        default: res=solveZooUtilDFS(board,p);
    }


    if(res==true)
        printSolution(board,myfile);
    else
       myfile<<"FAIL"<<endl;
    myfile.close();

}

int main()
{
    int p,trees=0,method=1;
    vector <vector<int>> board;

    input_board(board,p,trees,method);



    ofstream myfile;
    myfile.open ("output.txt");


    solveZoo(board,trees,p,myfile,method);

    myfile.close();
    return 0;
}

