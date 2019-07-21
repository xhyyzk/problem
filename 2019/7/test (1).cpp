#include<bits/stdc++.h>

using namespace std;

const int maxn = 105;
const int maxm = 10500;
#define inf 0x3f3f3f3f

class mp
{
public:
    int n, m;   //n为点数 m为边数
    int head[maxn]; //head[from]表示以head为出发点的邻接表表头在数组es中的位置，开始时所有元素初始化为-1
    int nodep;  //在邻接表和指向表头的head数组中定位用的记录指针，开始时初始化为0
    int bus_num;
    int bus[maxn][maxn];
    int is_bus[maxn];
    int is_car[maxn];
    struct edge {  //
        int v, next;
        int w;
    }es[maxm];

    mp() {  // 构造函数
        for(int i = 0; i < maxn; i++) {
            head[i] = -1;
        }
        nodep = 0;
        memset(bus, -1 , sizeof(bus));
        memset(is_bus, 0 , sizeof(is_bus));
    }

    void add(int from, int to, int weight) // 加边函数
    {
        es[nodep].v = to;
        es[nodep].w = weight;
        es[nodep].next = head[from];
        head[from] = nodep++;
    }

    void load()
    {
        cin >> n >> m;
        int f, to, w;
        for(int i = 0; i < m; i++)
        {
            cin >> f >> to >> w;
            add(f, to, w);
            add(to, f, w);
        }


        cin >> bus_num;  // 输入公交线路数
        for(int i = 1; i <= bus_num; i++)
        {
            int cnt;
            cin >> cnt;  // 表示改路线站点数
            while(cnt--)
            {
                int x;
                cin >> x;  // 顺序输入站点
                bus[i][x] = cnt;
                is_bus[x] = 1;
            }
        }

        for(int i = 1; i <= n; i++)  // 每一个是否有出租车, 0 1 表示
        {
            int flag;
            scanf("%d",&flag);
            is_car[i] = flag ;
        }
    }

};

class solve
{
public:
    int d[maxn]; //到源节点的距离
    bool vis[maxn]; //是否访问过
    int pre[maxn];  // 用于保存路径

    mp A;
    solve(mp A)
    {
        this ->A = A;

        for(int i = 0; i < maxn; i++) {
            d[i] = inf;
            vis[i] = false;
            pre[i] = -1;
        }
    }

    struct node {
        int num;
        int dis;
        node (int a = 0, int b = 0) : num(a), dis(b) {}
        friend bool operator <(node a, node b) {
            return a.dis > b.dis;
        }
    };

    void dijkstra(int num)  //遍历num到各点的最短距离
    {
        for(int i = 0; i < maxn; i++) {
            d[i] = inf;
            vis[i] = false;
            pre[i] = -1;
        }
        priority_queue<node> pq;
        d[num] = 0;
        pq.push(node(num, 0));
        while(!pq.empty()) {
            node tmp = pq.top();
            pq.pop();
            int u = tmp.num;
            if(vis[u]) continue;
            vis[u] = 1;
            //遍历邻接表
            for(int i = A.head[u]; i != -1; i = A.es[i].next) {  //在es中，相同from出发指向的顶点为从head[from]开始的一项，逐项使用next寻找下去，直到找到第一个被输
                                                            //入的项，其next值为-1
                int v = A.es[i].v;
                if(!vis[v] && d[v] > d[u] + A.es[i].w) { //松弛(RELAX)操作
                    d[v] = d[u] + A.es[i].w;
                    pre[v] = u;
                    pq.push(node(v, d[v]));
                }
            }
        }
    }

    void dijkstra_bus(int num)  //遍历公交站点num到各公交站点的最短距离
    {
        for(int i = 0; i < maxn; i++) {
            d[i] = inf;
            vis[i] = false;
            pre[i] = -1;
        }
        priority_queue<node> pq;
        d[num] = 0;
        pq.push(node(num, 0));
        while(!pq.empty()) {
            node tmp = pq.top();
            pq.pop();
            int u = tmp.num;
            if(vis[u]) continue;
            vis[u] = 1;
            //遍历邻接表
            for(int i = A.head[u]; i != -1; i = A.es[i].next) {  //在es中，相同from出发指向的顶点为从head[from]开始的一项
                int v = A.es[i].v;
                if(A.is_bus[v] == 0) continue;  // 不是公交车站点则跳过
                if(!vis[v] && d[v] > d[u] + A.es[i].w) { //松弛(RELAX)操作
                    d[v] = d[u] + A.es[i].w;
                    pre[v] = u;
                    pq.push(node(v, d[v]));
                }
            }
        }
    }

    void print_road(int s , int t)
    {
        stack<int> path;
        int now = t;
        while(1) {
            path.push(now);
            if(now == s) {
                break;
            }
            now = pre[now];
        }
        while(!path.empty()) {
            now = path.top();
            path.pop();
            printf("%d ", now);
        }
    }

    void print_bus(int a, int b)
    {
        cout <<"(";
        for(int i = 1 ; i <= A.bus_num; i++)
        {
            if(abs(A.bus[i][a] - A.bus[i][b]) == 1)  // 第i路公交经过a到b这条路
                cout << i << "路";
        }
        cout << ")";
    }

    void print_bus_road(int s , int t)
    {
        stack<int> path;
        int now = t;
        while(1) {
            path.push(now);
            if(now == s) {
                break;
            }
            now = pre[now];
        }
        int rev = path.top();
        path.pop();
        printf("%d", rev);
        while(!path.empty()) {
            now = path.top();
            print_bus(rev, now);
            path.pop();
            printf("%d", now);
            rev = now;
        }
    }

    void walk(int s , int t) {
        double v = 0.08, time;
        cout << "步行方案：" << endl;
        dijkstra(s);
        cout << "路径为：";
        print_road(s ,t);
        cout << endl<<"总路程："<< d[t] <<" 预计用时：" << (int)d[t]/v <<"分钟"<<endl<< endl;
    }

    void bus(int s , int t)
    {
        double vw = 0.08, vb = 0.8, time = 0;  // 步行速度和公交速度
        cout << "公交方案："<< endl;
        int l = 0;
        if(!A.is_bus[s])  // 起点不是公交车站
        {
            dijkstra(s);
            int t1 = -1, dis1 = inf;  // t1 是最近的公交车站
            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_bus[i] && d[i] < dis1) // 当前点势公交站点并距离小于已知的最近的公交站点
                {
                    t1 = i;
                    dis1 = d[i];
                }
            }
            cout << "步行: ";
            print_road(s, t1);
            time += d[t1]/vw;
            l += d[t1];
            s = t1;
        }

        int t2 = -1, dis2 = inf;
        if(!A.is_bus[t])  // 终点不是公交车站
        {
            dijkstra(t);

            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_bus[i] && d[i] < dis2) // 当前点是公交站点并距离小于已知的最近的公交站点
                {
                    t2 = i;
                    dis2 = d[i];
                }
            }
        }

        dijkstra_bus(s);
        if(t2 != -1)
        {
            cout << "公交: ";
            print_bus_road(s, t2);
            l += d[t2];
            time += d[t2]/vb;
            dijkstra(t2);
            cout << "步行: ";
            print_road(t2, t);
            l += d[t];
            time += d[t2]/vb;
        }
        else
        {
            cout << "公交: ";
            print_bus_road(s, t);
            l += d[t];
            time += d[t]/vb;
        }
        cout << endl<<"总路程："<< l <<" 预计用时：" << (int)time <<"分钟"<< endl<< endl;;

    }

    void taix(int s ,int t)
    {
        double v = 1.2;
        cout << "出租车方案："<< endl;
        int l = 0;
        dijkstra(s);
        if(!A.is_car[s])  // 起点没有出租车
        {

            int t1 = -1, dis1 = inf;  // t1 是最近的出租车
            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_car[i] && d[i] < dis1) // 当前点有出租车并距离小于已知的最近的出租车
                {
                    t1 = i;
                    dis1 = d[i];
                }
            }
            int tmp = t1;
            cout << "出租车 ";
            while(1)// 打印出租车到当前位置路
            {
                cout << tmp << " ";
                if(tmp == s) break;
                tmp = pre[tmp];
            }
            print_road(s, t); // 接到人后直接取目的地
            l = l+d[t1]+d[t];
            cout << endl<<"总路程："<< l <<" 预计用时：" << (int)l/v <<"分钟"<< endl<< endl;;
        }
        else
        {
            cout << "出租车 ";
            print_road(s, t); // 接到人后直接取目的地
            l = l+d[t];
            cout << endl<<"总路程："<< l <<" 预计用时：" << (int)l/v <<"分钟"<< endl<< endl;;
        }

    }


};

int main()
{
    mp A;
    A.load();
    int s , t;
    cin >> s >> t;
    solve a(A);
    a.walk(s, t);
    a.bus(s,t);
    a.taix(s ,t);
    return 0;

}

/*
9 11
1 2 1
1 3 2
1 4 3
4 5 4
2 7 2
3 7 2
4 8 3
6 7 1
7 8 3
7 9 2
8 9 5
3
4 1 2 7 9
5 1 3 7 8 9
4 1 4 8 9
1 1 0 1 0 0 0 1 1
6 5
*/

