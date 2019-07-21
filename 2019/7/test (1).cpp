#include<bits/stdc++.h>

using namespace std;

const int maxn = 105;
const int maxm = 10500;
#define inf 0x3f3f3f3f

class mp
{
public:
    int n, m;   //nΪ���� mΪ����
    int head[maxn]; //head[from]��ʾ��headΪ��������ڽӱ��ͷ������es�е�λ�ã���ʼʱ����Ԫ�س�ʼ��Ϊ-1
    int nodep;  //���ڽӱ��ָ���ͷ��head�����ж�λ�õļ�¼ָ�룬��ʼʱ��ʼ��Ϊ0
    int bus_num;
    int bus[maxn][maxn];
    int is_bus[maxn];
    int is_car[maxn];
    struct edge {  //
        int v, next;
        int w;
    }es[maxm];

    mp() {  // ���캯��
        for(int i = 0; i < maxn; i++) {
            head[i] = -1;
        }
        nodep = 0;
        memset(bus, -1 , sizeof(bus));
        memset(is_bus, 0 , sizeof(is_bus));
    }

    void add(int from, int to, int weight) // �ӱߺ���
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


        cin >> bus_num;  // ���빫����·��
        for(int i = 1; i <= bus_num; i++)
        {
            int cnt;
            cin >> cnt;  // ��ʾ��·��վ����
            while(cnt--)
            {
                int x;
                cin >> x;  // ˳������վ��
                bus[i][x] = cnt;
                is_bus[x] = 1;
            }
        }

        for(int i = 1; i <= n; i++)  // ÿһ���Ƿ��г��⳵, 0 1 ��ʾ
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
    int d[maxn]; //��Դ�ڵ�ľ���
    bool vis[maxn]; //�Ƿ���ʹ�
    int pre[maxn];  // ���ڱ���·��

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

    void dijkstra(int num)  //����num���������̾���
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
            //�����ڽӱ�
            for(int i = A.head[u]; i != -1; i = A.es[i].next) {  //��es�У���ͬfrom����ָ��Ķ���Ϊ��head[from]��ʼ��һ�����ʹ��nextѰ����ȥ��ֱ���ҵ���һ������
                                                            //������nextֵΪ-1
                int v = A.es[i].v;
                if(!vis[v] && d[v] > d[u] + A.es[i].w) { //�ɳ�(RELAX)����
                    d[v] = d[u] + A.es[i].w;
                    pre[v] = u;
                    pq.push(node(v, d[v]));
                }
            }
        }
    }

    void dijkstra_bus(int num)  //��������վ��num��������վ�����̾���
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
            //�����ڽӱ�
            for(int i = A.head[u]; i != -1; i = A.es[i].next) {  //��es�У���ͬfrom����ָ��Ķ���Ϊ��head[from]��ʼ��һ��
                int v = A.es[i].v;
                if(A.is_bus[v] == 0) continue;  // ���ǹ�����վ��������
                if(!vis[v] && d[v] > d[u] + A.es[i].w) { //�ɳ�(RELAX)����
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
            if(abs(A.bus[i][a] - A.bus[i][b]) == 1)  // ��i·��������a��b����·
                cout << i << "·";
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
        cout << "���з�����" << endl;
        dijkstra(s);
        cout << "·��Ϊ��";
        print_road(s ,t);
        cout << endl<<"��·�̣�"<< d[t] <<" Ԥ����ʱ��" << (int)d[t]/v <<"����"<<endl<< endl;
    }

    void bus(int s , int t)
    {
        double vw = 0.08, vb = 0.8, time = 0;  // �����ٶȺ͹����ٶ�
        cout << "����������"<< endl;
        int l = 0;
        if(!A.is_bus[s])  // ��㲻�ǹ�����վ
        {
            dijkstra(s);
            int t1 = -1, dis1 = inf;  // t1 ������Ĺ�����վ
            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_bus[i] && d[i] < dis1) // ��ǰ���ƹ���վ�㲢����С����֪������Ĺ���վ��
                {
                    t1 = i;
                    dis1 = d[i];
                }
            }
            cout << "����: ";
            print_road(s, t1);
            time += d[t1]/vw;
            l += d[t1];
            s = t1;
        }

        int t2 = -1, dis2 = inf;
        if(!A.is_bus[t])  // �յ㲻�ǹ�����վ
        {
            dijkstra(t);

            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_bus[i] && d[i] < dis2) // ��ǰ���ǹ���վ�㲢����С����֪������Ĺ���վ��
                {
                    t2 = i;
                    dis2 = d[i];
                }
            }
        }

        dijkstra_bus(s);
        if(t2 != -1)
        {
            cout << "����: ";
            print_bus_road(s, t2);
            l += d[t2];
            time += d[t2]/vb;
            dijkstra(t2);
            cout << "����: ";
            print_road(t2, t);
            l += d[t];
            time += d[t2]/vb;
        }
        else
        {
            cout << "����: ";
            print_bus_road(s, t);
            l += d[t];
            time += d[t]/vb;
        }
        cout << endl<<"��·�̣�"<< l <<" Ԥ����ʱ��" << (int)time <<"����"<< endl<< endl;;

    }

    void taix(int s ,int t)
    {
        double v = 1.2;
        cout << "���⳵������"<< endl;
        int l = 0;
        dijkstra(s);
        if(!A.is_car[s])  // ���û�г��⳵
        {

            int t1 = -1, dis1 = inf;  // t1 ������ĳ��⳵
            for(int i = 1; i <= A.n; i++)
            {
                if(A.is_car[i] && d[i] < dis1) // ��ǰ���г��⳵������С����֪������ĳ��⳵
                {
                    t1 = i;
                    dis1 = d[i];
                }
            }
            int tmp = t1;
            cout << "���⳵ ";
            while(1)// ��ӡ���⳵����ǰλ��·
            {
                cout << tmp << " ";
                if(tmp == s) break;
                tmp = pre[tmp];
            }
            print_road(s, t); // �ӵ��˺�ֱ��ȡĿ�ĵ�
            l = l+d[t1]+d[t];
            cout << endl<<"��·�̣�"<< l <<" Ԥ����ʱ��" << (int)l/v <<"����"<< endl<< endl;;
        }
        else
        {
            cout << "���⳵ ";
            print_road(s, t); // �ӵ��˺�ֱ��ȡĿ�ĵ�
            l = l+d[t];
            cout << endl<<"��·�̣�"<< l <<" Ԥ����ʱ��" << (int)l/v <<"����"<< endl<< endl;;
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

