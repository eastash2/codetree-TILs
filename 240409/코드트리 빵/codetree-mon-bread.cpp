#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <queue>

typedef std::pair<int, int> coord;
const int max_int = std::numeric_limits<int>::max();
const int dxdy[4][2] = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
inline int abs(int n) {
    return n < 0 ? -n : n;
}
inline int min(int a, int b) {
    return a < b ? a : b;
}

coord get_start_camp(const std::vector<std::vector<bool> >& map, const std::vector<coord>& camp, const coord convini) {
    const int n = map.size();
    const int c_size = camp.size();
    std::vector<int> dist_per_camp(camp.size(), max_int);    
    for(int i = 0; i < c_size; i++) {
        coord c = camp[i];
        if(!map[c.first][c.second]) 
            continue;

        std::vector<std::vector<bool> > visited(n, std::vector<bool>(n, false));
        std::vector<std::vector<int> > distance(n, std::vector<int>(n, max_int));
        distance[c.first][c.second] = 0;

        std::queue<coord> q;
        q.push(c);
        while(!q.empty()) {
            coord now = q.front(); q.pop();
            if(visited[now.first][now.second])
                continue;
            visited[now.first][now.second] = true;
            if(now == convini) {
                break;
            }
            for(int j = 0; j < 4; j++) {
                int nnx = now.first + dxdy[j][0];
                int nny = now.second + dxdy[j][1];
                if(nnx < 0 || nnx >= n || nny < 0 || nny >= n || !map[nnx][nny])
                    continue;
            
                q.push(coord(nnx, nny));                
                distance[nnx][nny] = min(distance[nnx][nny], distance[now.first][now.second] + 1);
                 
            }
        }
        dist_per_camp[i] = distance[convini.first][convini.second];
    }
    int min_dist = max_int;
    int camp_ind = 0;
    for(int i = 0; i < c_size; i++) {
        if(min_dist > dist_per_camp[i]) {
            camp_ind = i;
            min_dist = dist_per_camp[i];
        }
    }
    return camp[camp_ind];
}

//coordinate with depth
coord move_one_man(const std::vector<std::vector<bool> >& map, const coord man, const coord convini) {
    const int n = map.size();   

    std::vector<int> dist_per_direction(4, max_int);
    
    for(int i = 0; i < 4; i++) {
        int nx = man.first + dxdy[i][0];
        int ny = man.second + dxdy[i][1];

        if(nx < 0 || nx >= n || ny < 0 || ny >= n || !map[nx][ny])
            continue;
        std::vector<std::vector<bool> > visited(n, std::vector<bool>(n, false));
        std::vector<std::vector<int> > distance(n, std::vector<int>(n, max_int));
        distance[man.first][man.second] = 0;
        visited[man.first][man.second] = true;
        distance[nx][ny] = 1;

        std::queue<coord> q;
        q.push(coord(nx, ny));
        while(!q.empty()) {
            coord now = q.front(); q.pop();
            if(visited[now.first][now.second])
                continue;
            visited[now.first][now.second] = true;
            if(now == convini) {
                break;
            }
            for(int j = 0; j < 4; j++) {
                int nnx = now.first + dxdy[j][0];
                int nny = now.second + dxdy[j][1];
                if(nnx < 0 || nnx >= n || nny < 0 || nny >= n || !map[nnx][nny])
                    continue;
            
                q.push(coord(nnx, nny));
                distance[nnx][nny] = min(distance[nnx][nny], distance[now.first][now.second] + 1);
                 
            }
        }
        dist_per_direction[i] = distance[convini.first][convini.second];
    }
    int min_dist = max_int;
    int direction = 0;
    for(int i = 0; i < 4; i++) {
        if(min_dist >= dist_per_direction[i]) {
            direction = i;
            min_dist = dist_per_direction[i];
        }
    }
    return coord(man.first + dxdy[direction][0], man.second + dxdy[direction][1]);
}
void debug(std::vector<std::vector<bool> > map, std::vector<coord> camp, std::vector<coord> convini) {
    std::cout << "////////////////\n";
    int n = map.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            bool drawn = false;
            for(coord c : camp) {
                if(c.first == i && c.second == j) {
                    std::cout << (map[i][j] ? "B " : "b ");
                    drawn = true;
                    break;
                }
            }
            if(!drawn) {
                for(coord c : convini) {
                    if(c.first == i && c.second == j) {
                        std::cout << (map[i][j] ? "C " : "c ");
                        drawn = true;
                        break;
                    }
                }   
            }
            if(!drawn)
                std::cout << (map[i][j] ? "o " : "x ");            
        }
        std::cout << "\n";
    }
    std::cout << "////////////////\n";
}
int main() {
    int n, m;
    std::cin >> n >> m;

    //false if cannot pass
    std::vector<std::vector<bool> > map(n, std::vector<bool>(n, true));

    std::vector<coord> camp;
    std::vector<coord> convini;

    for(int x = 0; x < n; x++) {
        for(int y = 0; y < n; y++) {
            int is_camp;
            std::cin >> is_camp;
            if(is_camp == 1) {
                camp.push_back(coord(x, y));
            }    
        }
    }
    for(int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        convini.push_back(coord(x - 1, y - 1));
    }

    std::vector<coord> man_move;
    int t = 0;
    int num_arrived = 0;
  //  debug(map, camp, convini);
    while(num_arrived < m) {
        //man moves
        const int num_man_started = man_move.size();
        for(int j = 0; j < num_man_started; j++)
            if(man_move[j] != convini[j])
                man_move[j] = move_one_man(map, man_move[j], convini[j]);

        //check convini
        for(int j = 0; j < num_man_started; j++) {
            if(man_move[j] == convini[j] && map[man_move[j].first][man_move[j].second]) {
                map[man_move[j].first][man_move[j].second] = false;
                num_arrived++;
            }
        }

        //goto basecamp
        if(t < m) {
            coord start_camp = get_start_camp(map, camp, convini[t]);
            map[start_camp.first][start_camp.second] = false;
            man_move.push_back(start_camp);
        }


        t++;
  //      debug(map, camp, convini);
    }
    std::cout << t;
    return 0;
}