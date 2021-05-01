//IMPLEMENTING FINE GRAINED LOCKING
#include<iostream>
#include<sstream>
#include<fstream>
#include<thread>
#include<chrono>
#include<random>
#include<list>
#include<vector>
#include<iterator> 
#include<time.h>
#include<sys/time.h>
#include<semaphore.h>

using namespace std;

//INSTRUCTION FOR COMPILATION - g++ -std=c++11 -pthread <sample.cpp> -o out

list<long long int> *adj; //adjacency list
vector<long long int> result; //resutant color on each vertex
vector<bool> tempo; //available colors
vector<long long int> partition; //partition of vertices
vector<bool> intern; //to check if vertex is internal or external
//long long int* mutex;
//int mutex = 1;

sem_t* mutex;

//void signal(long long int* a)
//{
//	*a = *a + 1;
//}
//
//void wait(long long int* a)
//{
//	while((*a)<=0)
//	{
//		//busy wait
//	}
//	*a = *a - 1;
//}

void createGraph(long long int V)
{
	adj = new list<long long int>[V];
}

void addEdge(long long int v, long long int w)
{
	adj[v].push_back(w);
}

void greedyColoring(long long int V, long long int id)
{
	int checker;
	for(long long int i=0;i<V;i++)
	{
		if(partition[i]!=id)
			continue; //corresponding vertex does not belong to partition for thread
		if(intern[i]==true) //if the vertex is internal
		{
			list<long long int>::iterator it;
			for(it=adj[i].begin();it!=adj[i].end();++it) //checking if neighbours are coloured
			{
				if(result[*it] != -1)
					tempo[result[*it]] = true;
			}
			long long int j;
			for(j=0;j<V;j++) //checking for the first available color
			{
				if(tempo[j]==false)
					break;
			}
			result[i] = j;
			for(it=adj[i].begin();it!=adj[i].end();++it) //reset
			{
				if(result[*it] != -1)
					tempo[result[*it]] = false;
			}
		}
		else//vertex is external
		{
			checker = 0;
			list<long long int>::iterator it5;
			for(it5=adj[i].begin();it5!=adj[i].end();++it5) //locks acquired in ascending order to avoid deadlocks
			{
//				wait(&mutex[*it5]);
				if(i<*it5 && checker==0)
				{
					checker=1;
					sem_wait(&mutex[i]);
				}
				sem_wait(&mutex[*it5]);
				
			}
//			wait(&mutex[i]);
			if(checker==0)
				sem_wait(&mutex[i]);

			list<long long int>::iterator it;
			for(it=adj[i].begin();it!=adj[i].end();++it)
			{
				if(result[*it] != -1)
					tempo[result[*it]] = true;//which of the colors are already used by neighbours
			}
			long long int j;
			for(j=0;j<V;j++)
			{
				if(tempo[j]==false)
					break;
			}
			result[i] = j;
			for(it=adj[i].begin();it!=adj[i].end();++it)
			{
				if(result[*it] != -1)
					tempo[result[*it]] = false;
			}
			
			sem_post(&mutex[i]); //releasing locks
			
			for(it5=adj[i].begin();it5!=adj[i].end();++it5)
			{
//				signal(&mutex[*it5]);
				sem_post(&mutex[*it5]);
			}
//			signal(&mutex[i]);
		}	
	}
}

int main()
{
	long long int n,v,vid;
	string line;
	vector<vector<long long int>> temp;
	ifstream input;
	ofstream output;
	input.open("input_params.txt", ios::in);
	
	if(!input)
	{
		cout<<"ERROR. FILE DOESN'T EXIST"<<endl;
		return 0;
	}
	else
	{
		input>>n; //total number of threads
		input>>v; //total number of vertices in the graph
		srand(time(0));
		
		for(long long int i=0;i<v;i++)
		{
			input>>vid; //vertex id
			getline(input,line);
			istringstream iss(line);
			temp.push_back(vector<long long int>(istream_iterator<long long int>(iss),istream_iterator<long long int>()));
		}
	}
	std::thread arr[n];//ARRAY OF N THREADS
	srand(time(0));
	partition.resize(v);
	for(long long int i=0;i<v;i++)
	{
		partition[i] = rand()%n; //indicates vertices are partitioned into different threads randomly
	}
	
	createGraph(v);
	
	result.resize(v);
    tempo.resize(v);
    intern.resize(v);
//    mutex.resize(v);
	mutex = new sem_t [v];
	
	for(long long int i=0;i<v;i++)
	{
		tempo[i] = false; //all colors are available
		result[i] = -1; //no color assigned
		intern[i] = true; //all vertices are internal
		sem_init(&mutex[i],0,1);
//		mutex[i] = 1;
	}

	for (long long int i = 0; i < temp.size(); i++)
	{
		int record = partition[i];
        for (long long int j = 0; j < temp[i].size(); j++)
        {
            addEdge(i,temp[i][j]); //the edges in input are from 0 to v-1, and list starts from 0
            if(partition[temp[i][j]]!=record)
            	intern[i] = false; //differentiating between internal and external vertices
        }
    }
    
    for(int i=0;i<v;i++)
    {
    	adj[i].sort();//for acquiring locks in ascending order to avoid deadlocks
	}
    
//    struct timeval start, end;
//    gettimeofday(&start, NULL);
//    double m1 = start.tv_sec * 1000 + start.tv_usec / 1000;
    
    std::chrono::steady_clock::time_point start1; //FOR CALCULATING TIME
	std::chrono::steady_clock::time_point end1;
	
	start1 = std::chrono::steady_clock::now();
	for(long long int i=0;i<n;i++)
	{
		arr[i] = std::thread(greedyColoring,v,i);
	}

	for(int i = 0;i<n;i++) //ensuring termination of algorithm
	{
		arr[i].join();//waiting for thread completion
	}
	
	end1 = std::chrono::steady_clock::now();
	
	double time1 = (std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count());//time in microseconds
	
	output.open("output.txt", ios::out);
	if(!output)
	{
		cout<<"ERROR. FILE NOT CREATED"<<endl;
		return 0;
	}
	else
	{
		output<<"Fine-Grained Lock\nColours:\n";
		long long int max = 0;
		for(long long int i=0;i<v;i++)
		{
			if(result[i]+1>max)
				max = result[i]+1;
			output<<"v"<<i+1<<" - "<<result[i]+1<<", ";//vertex and colors are labelled from 0 to n-1, so +1
		}
		output<<"\nNo. of colours used: "<<max<<endl;
		output<<"Time taken by the algorithm: "<<time1<<endl;
	}
	
	
	
	return 0;
}
