#include <iostream>
#include <cfloat>
#include <vector>
#include <fstream>

using namespace std;

class Node{
    
    public:
    string name;
    Node *p;
    double d;
    Node(string n){
        name=n;
        p=NULL;
        d=DBL_MAX;   
    }
};
    
class Edge {
    public:
    Node *u;
    Node *v;
    int w;
    Edge(Node *x,Node *y,int m){
        u=x;
        v=y;
        w=m;
    }
};

class Pair{
    public:
    Pair(Node *x,Node *y){
        u=x; v=y;
    }
    Node* u;
    Node* v;
};

class Graph {
    public:
    vector<Node*> V;
    vector<Edge*> E;
    vector<Pair*> C;
    
    void print(){
        int size=V.size();
        for(int i=0;i<size;i++){
        cout<<V[i]->name<<":"<<V[i]->d;
        if(V[i]->p) cout << "  "<< V[i]->p->name<<endl;
        else  cout << "  NULL"<<endl; 
        
        }
        size= E.size();
        for(int i=0;i<size;i++)
            cout<< E[i]->u->name<<"---"<< E[i]->w <<"---"<< E[i]->v->name<<endl;
        cout <<"with the constraints:"<<endl;
        size=C.size();
            for(int i=0;i<size;i++)
                cout<< C[i]->u->name << "  " << C[i]->v->name << endl;
    }
};

void relax(Edge *e){
    
    if(e->v->d > e->u->d + e->w && e->u->d != DBL_MAX)
            {
                e->v->d =  e->u->d + e->w ;
                e->v->p=e->u;
                return;
            }        
            
    if(e->u->d > e->v->d + e->w && e->v->d != DBL_MAX)
            {
                e->u->d =  e->v->d + e->w ;
                e->u->p=e->v;
            }        
}

void print(vector< vector <string> > allsets){
cout<< "********************************************"<<endl; 	
		for(int i=0;i<allsets.size();i++){
				for(int j=0;j<allsets[i].size();j++)
					cout << allsets[i][j];
				cout<<endl;
		}
}

bool notin(vector<string> v, string e){
    
    int size=v.size();
    for(int i=0;i<size;i++)
        if(v[i]== e) return false;
        
    return true;
}

bool is_common(vector<string> x, vector<string> y)
{
	int x_size= x.size();
	for(int i=0;i<x_size;i++)
		if(! notin(y,x[i]))
			return true;
return false;
}

void uni(vector< vector<string> > &allsets,int i,int j){
    
    int size= allsets[j].size();
    for(int k=0;k<size;k++)
        if(notin(allsets[i],allsets[j][k]))
         allsets[i].push_back(allsets[j][k]);

	    allsets.erase(allsets.begin()+j);

    }

void uniSets(vector<vector<string> > &allsets ){
    
    for(int i=0;i<allsets.size();i++)
        {
           for(int j=0;j<allsets.size();j++)
			{
				if(i!=j && is_common(allsets[i],allsets[j]))
					{
						uni(allsets,i,j);
						j--;				
					}
        	}    
        }
}
void process_shortest_path(Graph G,Node *s,vector<vector<string> >&allsets,vector<Edge*>&result){
    
    int size=G.V.size();
    Node *x;
    vector<string> set;
    for(int i=0;i<size;i++)
        if(G.V[i]->name == s->name)
            x=G.V[i];
    while(x->p){
        
        set.push_back(x->name);
        int size2=G.E.size();
        for(int i=0;i<size2;i++)
         if((G.E[i]->u->name == x->name && G.E[i]->v->name == x->p->name) || (G.E[i]->u->name == x->p->name && G.E[i]->v->name == x->name) )
          result.push_back(G.E[i]);      
            x= x->p;
    }
    set.push_back(x->name);
    allsets.push_back(set);        
}

void initialize (Graph &G){
    
    int size=G.V.size();
    for(int i=0;i<size;i++)
    {
        G.V[i]->d = DBL_MAX;
        G.V[i]->p=NULL;    
    }
}

void BellmanFord(Graph &G, Pair *c,vector< vector<string> > &allsets,vector<Edge*> &result)
{
        c->u->d=0;
        int size= G.V.size();
        int size2=G.E.size();
    
        for(int i=0;i<size-1;i++)
            for(int j=0;j<size2;j++)
                relax(G.E[j]);

        process_shortest_path(G,c->v,allsets,result);
}

void make_sets_from_remainings (Graph G,vector< vector<string> > &allsets){
    
    int size=allsets.size();
    vector<string> inSets;
    for(int i=0;i<size;i++)
        {
            int size2=allsets[i].size();
            for(int j=0;j<size2;j++)
                inSets.push_back(allsets[i][j]);
        }
    size=G.V.size();
    for(int i=0;i<size;i++)
        {
            if(notin(inSets,G.V[i]->name))
                {
                    vector<string> temp;
                    temp.push_back(G.V[i]->name);
                    allsets.push_back(temp);
                }
        }
}

int findSet(Node *x,vector< vector<string> > allsets){
    
    int size=allsets.size();
    for(int i=0;i<size;i++)
            if(! notin(allsets[i],x->name))
                return i;
}


    

    Edge* extract_min( vector<Edge*> &vertices)
    {
       int min=0;
       Edge *minNode;
        int size= vertices.size();
        for(int i=1; i<size;i++)
            if(vertices[min]->w > vertices[i]->w)
                min =i;
        minNode = vertices[min];
        vertices.erase (vertices.begin()+min);
        return minNode;
    }
void BellmanFordForConstraints(Graph &G, vector<vector<string> > &allsets,vector<Edge*> &result){
        int size=G.C.size();
        for(int i=0;i<size;i++){
            initialize(G);
            BellmanFord(G,G.C[i],allsets,result);
        }
    }
void KruskalMST(Graph &G, vector<vector<string> > &allsets,vector<Edge*> &result){
    while(G.E.size())
        {
            Edge *e = extract_min(G.E);
            int i=findSet(e->u,allsets);
            int j=findSet(e->v,allsets);
            if(i!=j){
                result.push_back(e);
                uni(allsets,i,j);
            }
        }
}
Graph createGraph()
    {
         ifstream myfile;
    Graph G;
    int number_of_vertices, number_of_edges, number_of_const;
    string c;
    myfile.open ("hw3.inp");
    myfile >> number_of_vertices;
    myfile >> number_of_edges;
    myfile >> number_of_const;
    while((number_of_vertices--) > 0)
        {
            myfile >> c;
            Node *temp= new Node(c);
            G.V.push_back(temp);
        }
    
    while((number_of_edges--)>0)
        {
            string u,v;
            int w;
            int size= G.V.size();
            myfile >> u; myfile >> v; myfile >>w;
            Node *t1,*t2;
            for(int i=0;i<size;i++)
                {
                    Node *temp = G.V[i];
                    
                    if(temp->name == u)
                            t1=temp;
                    if(temp->name == v)
                            t2=temp;
                }
            Edge *e = new Edge(t1,t2,w);
            G.E.push_back(e);
        }
        
        while((number_of_const--)>0)
        {
            string u,v;
            int size= G.V.size();
            myfile >> u; myfile >> v;
            Node *t1,*t2;
            for(int i=0;i<size;i++)
                {
                    Node *temp = G.V[i];
                    
                    if(temp->name == u)
                            t1=temp;
                    if(temp->name == v)
                            t2=temp;
                }
            Pair *e = new Pair(t1,t2);
            G.C.push_back(e);
             
        }
        myfile.close();
        return G;
    }

void print_result(vector<Edge*> &result){
	 ofstream myfile;
 	 myfile.open ("hw3.out");
       int  size=result.size();
        for(int i=0;i<size;i++)
            myfile<< result[i]->u->name<<" "<<result[i]->v->name<<endl;
	myfile.close();
}
void print_empty(){
	 ofstream myfile;
 	 myfile.open ("hw3.out");
	myfile.close();
}
int main()
{
        Graph G= createGraph();
        vector< vector<string> > allsets;
        vector<Edge*> result;
		BellmanFordForConstraints(G,allsets,result);
		uniSets (allsets);
        make_sets_from_remainings (G,allsets);
        KruskalMST(G,allsets,result);
		if(allsets.size()==1)
        	print_result(result);
		else print_empty();
        return 0;
}

