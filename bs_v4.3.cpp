#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

#define EPS (1e-8)
#define SQ(a) ((a)*(a))
enum class Type{Circle,Line,Ray,Segment};

bool IsZero(double i)
{return fabs(i)<EPS;}
double RoundIfZero(double i)
{return IsZero(i)?0:i;}

class Point
{
public:
    double x,y;
    Point(){}
    Point(double x,double y):x(x),y(y){}
    void Print() const
    {cout<<"("<<RoundIfZero(x)<<","<<RoundIfZero(y)<<")"<<endl;}
};

class Element
{
private:
    void Normalize()
    {
        if(!IsZero(b))
        {a/=b;c/=b;b=1.0;}
        else if(!IsZero(a))
        {c/=a;a=1.0;b=0.0;}
    }
public:
    double a,b,c;
    Point P1,P2;
    Type type;
    bool IsDrawn=false;

    Element(double a,double b,double c,Type t)
    :a(a),b(b),c(c),type(t)
    {if(t==Type::Line)Normalize();}

    Element(const Point& p1,const Point& p2,Type t):
    P1(p1),P2(p2),type(t)
    {
        if(t==Type::Circle)
        {
            a=p1.x;
            b=p1.y;
            c=SQ(p1.x-p2.x)+SQ(p1.y-p2.y);
        }
        else
        {
            a=p2.y-p1.y;
            b=p1.x-p2.x;
            c=p1.x*p2.y-p1.y*p2.x;
            Normalize();
        }
    }

    void Print() const
    {
        double a_copy=RoundIfZero(a),b_copy=RoundIfZero(b);
        if(type==Type::Circle)
        {cout<<"(x-"<<a_copy<<")^2+(y-"<<b_copy<<")^2="<<sqrt(c)<<"^2"<<endl;}
        else{cout<<a_copy<<"x+"<<b_copy<<"y="<<RoundIfZero(c)<<endl;}
    }
};

class Graph
{
private:
    bool IsInRange(const Element& e,const Point& p) const
    {
        if(e.type==Type::Line){return true;}
        if(e.type==Type::Ray)
        {
            double dot_x=(e.P1.x-p.x)*(e.P1.x-e.P2.x);
            double dot_y=(e.P1.y-p.y)*(e.P1.y-e.P2.y);
            return dot_x+dot_y>-EPS;
        }
        if(e.type==Type::Segment)
        {
            double dot_x=(e.P1.x-p.x)*(e.P2.x-p.x);
            double dot_y=(e.P1.y-p.y)*(e.P2.y-p.y);
            return dot_x+dot_y<EPS;
        }
        return false;
    }
public:
    vector<int> pointsAdded;
    vector<Point> points;
    vector<Element> elements;
    vector<Point> goalPoints;
    vector<Element> goalElements;

    bool IsPointBelong(const Point& gp) const
    {
        for(const Point& p:points)
        {if(IsSamePoint(gp,p))return true;}
        return false;
    }
    bool IsSamePoint(const Point& p1,const Point& p2) const
    {return IsZero(p1.x-p2.x)&&IsZero(p1.y-p2.y);}
    bool IsSameElement(const Element& e1,const Element& e2) const
    {
        if(e1.type==e2.type)
            if(IsZero(e1.a-e2.a)&&IsZero(e1.b-e2.b)&&IsZero(e1.c-e2.c))
                return true;
        return false;
    }

    bool AddPoint(const Point& point)
    {
        for(const Point& p:points)
        {if(IsSamePoint(point,p))return false;}
        points.push_back(point);
        ++pointsAdded[elements.size()];
        return true;
    }
    bool AddElement(const Element& element)
    {
        for(const Element& e:elements)
        {if(IsSameElement(element,e))return false;}
        for(const Element& e:elements)
        {Intersect(element,e);}
        elements.push_back(element);
        return true;
    }

    void Intersect(const Element& e1,const Element& e2)
    {
        if(e1.type==Type::Circle)
        {
            if(e2.type==Type::Circle)
            {
                double a=2*(e1.a-e2.a);
                double b=2*(e1.b-e2.b);
                if(IsZero(a)&&IsZero(b)){return;}
                double c=SQ(e1.a)-SQ(e2.a)+SQ(e1.b)-SQ(e2.b)-e1.c+e2.c;
                Element RadicalAxis(a,b,c,Type::Line);
                Intersect(RadicalAxis,e1);
            }
            else{Intersect(e2,e1);}
        }
        else
        {
            if(e2.type==Type::Circle)
            {
                double denom=SQ(e1.a)+SQ(e1.b);
                double dist=e1.a*e2.a+e1.b*e2.b-e1.c;
                double delta=denom*e2.c-SQ(dist);

                if(IsZero(delta))
                {
                    double x=e2.a-e1.a*dist/denom;
                    double y=e2.b-e1.b*dist/denom;
                    AddPoint(Point(x,y));
                }

                if(delta>EPS)
                {
                    double root=sqrt(delta);
                    double x1=e2.a-(e1.a*dist+e1.b*root)/denom;
                    double y1=e2.b-(e1.b*dist-e1.a*root)/denom;
                    double x2=e2.a-(e1.a*dist-e1.b*root)/denom;
                    double y2=e2.b-(e1.b*dist+e1.a*root)/denom;
                    if(IsInRange(e1,Point(x1,y1)))
                    {AddPoint(Point(x1,y1));}
                    if(IsInRange(e1,Point(x2,y2)))
                    {AddPoint(Point(x2,y2));}
                }
            }
            else
            {
                double det=e1.a*e2.b-e1.b*e2.a;
                if(IsZero(det)){return;}
                double x=(e1.c*e2.b-e1.b*e2.c)/det;
                double y=(e1.a*e2.c-e1.c*e2.a)/det;
                if(IsInRange(e1,Point(x,y))&&IsInRange(e2,Point(x,y)))
                {AddPoint(Point(x,y));}
            }
        }
    }

    void PrintSolution() const
    {
        cout<<"Solution found!"<<endl;
        for(const Point& p:points){p.Print();}
        for(const Element& e:elements){e.Print();}
    }
};

class Manager
{
private:
    void Next(Graph& graph,const int Type)
    {
        ++Tool[graph.elements.size()];++Progress;
        if(!(Progress%1000000))cout<<"Progress:"<<Progress<<endl;
        for(int i=graph.elements.size();i>=Initial.elements.size();--i)
        {
            if(Tool[i]>2-!Type)
            {++Pair[i][1];Tool[i]=0;}
            if(Pair[i][1]>graph.points.size())
            {++Pair[i][0];Pair[i][1]=Pair[i][0]+1;}
            if(Pair[i][0]>graph.points.size()-1&&i>Initial.elements.size())
            {
                ++Tool[i-1];
                Pair[i][0]=1;Pair[i][1]=2;
                Undo(graph);
            }
            else break;
        }
    }

    void Undo(Graph& graph)
    {
        for(Element& ge:graph.goalElements)
        {
            if(graph.IsSameElement(ge,graph.elements.back()))
            {ge.IsDrawn=false;++GoalENotMet;}
        }
        graph.elements.pop_back();
        int E=graph.elements.size();
        graph.points.resize(graph.points.size()-graph.pointsAdded[E]);
        graph.pointsAdded[E]=0;
    }

    bool CanDraw(const Graph& graph,const Element& ge) const
    {
        if(ge.type==Type::Circle)
        {
            bool candraw=false;
            Point centre(ge.a,ge.b);
            for(const Point& p:graph.points)
                if(IsZero(SQ(ge.a-p.x)+SQ(ge.b-p.y)-ge.c)){candraw=true;}
            return graph.IsPointBelong(centre)&&candraw;
        }
        else
        {
            int pointsonline=0;
            for(const Point& p:graph.points)
                if(IsZero(ge.a*p.x+ge.b*p.y-ge.c)){++pointsonline;}
            return pointsonline>1;
        }
        return false;
    }

    bool Check(Graph& graph)
    {
        for(Point& gp:graph.goalPoints)
            if(!graph.IsPointBelong(gp)){return false;}
        for(Element& ge:graph.goalElements)
        {
            if(ge.IsDrawn){continue;}
            if(graph.IsSameElement(ge,graph.elements.back()))
            {ge.IsDrawn=true;--GoalENotMet;}
        }
        if(GoalENotMet==1)
            for(Element& ge:graph.goalElements)
                if(!ge.IsDrawn&&CanDraw(graph,ge))
                    return true;
        return !GoalENotMet;
    }
public:
    int GoalENotMet=0;
    int Progress=0;
    int InitialE=0;
    vector<int> Tool;
    vector<vector<int>> Pair;
    Graph Initial;
    
    bool BruteSearch(Graph& graph,int limit,const int Type)
    {
        Initial=graph;
        GoalENotMet=Initial.goalElements.size();
        Tool.assign(limit,0);
        Pair.assign(limit,{1,2});
        if(GoalENotMet){--limit;}

        cout<<"Searching..."<<endl;
        while(Pair[InitialE][1]<=Initial.points.size()&&Tool[InitialE]<3-!Type)
        {
            int E=graph.elements.size();
            Point& p1=graph.points[graph.points.size()-Pair[E][0]];
            Point& p2=graph.points[graph.points.size()-Pair[E][1]];
            if(Type==1)Tool[E]=2;
            if(Tool[E]==0)
            {
                if(!graph.AddElement(Element(p1,p2,Type::Circle)))
                {Next(graph,Type);continue;}
            }
            else if(Tool[E]==1)
            {
                if(!graph.AddElement(Element(p2,p1,Type::Circle)))
                {Next(graph,Type);continue;}
            }
            else if(Tool[E]==2)
            {
                if(!graph.AddElement(Element(p1,p2,Type::Line)))
                {Next(graph,Type);continue;}
            }
            if(Check(graph)){return true;}
            E=graph.elements.size();
            if((E==limit-GoalENotMet+2)||(E==limit&&(GoalENotMet==1||GoalENotMet==0)))
            {Undo(graph);Next(graph,Type);}
        }
        return false;
    }
};

int main()
{
    int limit,tool_type,goal_type,goal_num;
    vector<int> Initial(6);
    Graph graph;
    Manager manager;
    
    cout<<"brute_search v4.2"<<endl;
    cout<<"Enter E limit (exclude given Es):"<<endl;
    cin>>limit;
    cout<<"Select the tool (0=compass only,1=straightedge only,2=both):"<<endl;
    cin>>tool_type;
    cout<<"Enter total number of P,L,R,S,C respectively:"<<endl;
    cin>>Initial[1]>>Initial[2]>>Initial[3]>>Initial[4]>>Initial[5];
    manager.InitialE=Initial[2]+Initial[3]+Initial[4]+Initial[5];
    limit+=manager.InitialE;
    graph.pointsAdded.resize(limit);
    for(int i=0;i<Initial[1];++i)
    {
        double x,y;
        cout<<"Enter P"<<i+1<<endl;
        cin>>x>>y;
        graph.points.push_back(Point(x,y));
    }
    for(int i=0;i<Initial[2];++i)
    {
        double a,b,c;
        cout<<"Enter L"<<i+1<<endl;
        cin>>a>>b>>c;
        graph.AddElement(Element(a,b,c,Type::Line));
    }
    for(int i=0;i<Initial[3];++i)
    {
        double x1,y1,x2,y2;
        cout<<"Enter R"<<i+1<<endl;
        cin>>x1>>y1>>x2>>y2;
        graph.AddElement(Element(Point(x1,y1),Point(x2,y2),Type::Ray));
    }
    for(int i=0;i<Initial[4];++i)
    {
        double x1,y1,x2,y2;
        cout<<"Enter S"<<i+1<<endl;
        cin>>x1>>y1>>x2>>y2;
        graph.AddElement(Element(Point(x1,y1),Point(x2,y2),Type::Segment));
    }
    for(int i=0;i<Initial[5];++i)
    {
        double a,b,r;
        cout<<"Enter C"<<i+1<<endl;
        cin>>a>>b>>r;
        graph.AddElement(Element(a,b,SQ(r),Type::Circle));
    }

    cout<<"Enter the type of your goal (0=C,1=L,2=P):"<<endl;
    cin>>goal_type;
    cout<<"Enter the total number of your goal:"<<endl;
    cin>>goal_num;
    if(goal_type==0)
        for(int i=0;i<goal_num;++i)
        {
            double a,b,r;
            cout<<"Enter Goal C"<<i+1<<endl;
            cin>>a>>b>>r;
            graph.goalElements.push_back(Element(a,b,SQ(r),Type::Circle));
        }
    if(goal_type==1)
        for(int i=0;i<goal_num;++i)
        {
            double a,b,c;
            cout<<"Enter Goal L"<<i+1<<endl;
            cin>>a>>b>>c;
            graph.goalElements.push_back(Element(a,b,c,Type::Line));
        }
    if(goal_type==2)
        for(int i=0;i<goal_num;++i)
        {
            double x,y;
            cout<<"Enter Goal P"<<i+1<<endl;
            cin>>x>>y;
            graph.goalPoints.push_back(Point(x,y));
        }
    clock_t start=clock();
    if(manager.BruteSearch(graph,limit,tool_type))
    {
        graph.PrintSolution();
        clock_t end=clock();
        double time=(double)(end-start)/CLOCKS_PER_SEC;
        cout<<"Search Time:"<<time<<endl;
        system("pause");
        return 0;
    }
    cout<<"No solution found"<<endl;
    clock_t end=clock();
    double time=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<"Search Time:"<<time<<endl;
    system("pause");
    return 0;
}
