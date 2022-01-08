
#include <iostream>
#include <limits.h>
#include <queue>
#include <string.h>
using namespace std;
 

// Số đỉnh trong đồ thị đã cho
#define V 6
 


  
  
bool bfs(int rGraph[V][V], int s, int t, int parent[])
{
    // Tạo một mảng đã thăm và đánh dấu tất cả các đỉnh
    bool visited[V];
    memset(visited, 0, sizeof(visited));
 
   // Tạo hàng đợi, đỉnh nguồn xếp hàng và đánh dấu nguồn
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
 
  
// Vòng lặp BFS tiêu chuẩn
    while (!q.empty()) {
        int u = q.front();
        q.pop();
 
        for (int v = 0; v < V; v++) {
            if (visited[v] == false && rGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}
 

// Trả về luồng lớn nhất từ s đến t trong đồ thị đã cho
int fordFulkerson(int graph[V][V], int s, int t)
{
    int u, v;
 
  
// Tạo đồ thị phần dư và điền vào đồ thị
    int rGraph[V]
              [V]; 

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = graph[u][v];
 
    int parent[V]; 
    int max_flow = 0; 
 
    
    while (bfs(rGraph, s, t, parent)) {
     
// Tìm dung lượng dư nhỏ nhất của các cạnh dọc
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
 
       // cập nhật dung lượng dư của các cạnh và đảo ngược các cạnh dọc theo đường dẫn
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
 
     // Thêm luồng đường dẫn vào luồng tổng thể
        max_flow += path_flow;
    }
 
   // Trả về quy trình tổng thể
    return max_flow;
}
 

int main()
{
   // Tạo một biểu đồ được hiển thị trong ví dụ trên
    int graph[V][V]
        = { { 0, 16, 13, 0, 0, 0 }, { 0, 0, 10, 12, 0, 0 },
            { 0, 4, 0, 0, 14, 0 },  { 0, 0, 9, 0, 0, 20 },
            { 0, 0, 0, 7, 0, 4 },   { 0, 0, 0, 0, 0, 0 } };
 
    cout << "The maximum possible flow is "
         << fordFulkerson(graph, 0, 5);
 
    return 0;
}