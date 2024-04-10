#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <limits>

typedef std::pair<int, int> Pos;

int N, M, K;
int bonus;
int num_alive;
std::vector<std::vector<int> > board;
std::vector<std::vector<int> > recent_attack;
std::vector<std::vector<bool> > modified;
void init() {
    std::cin >> N >> M >> K;
    bonus = N + M;

    board = std::vector<std::vector<int> >(N, std::vector<int>(M, 0));
    recent_attack = std::vector<std::vector<int> >(N, std::vector<int>(M, 0));
    modified = std::vector<std::vector<bool> >(N, std::vector<bool>(M, false));

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {            
            int input;
            std::cin >> input;            
            if(input != 0) {
                num_alive++;
            }
            board[i][j] = input;   
        }
    }
    return;
}

Pos get_attacker() {
    Pos min_pos = Pos(-1, -1);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(board[i][j] == 0) continue;
            int x_min = min_pos.first;
            int y_min = min_pos.second;
            if(min_pos == Pos(-1, -1)) {            
                min_pos = Pos(i, j);
                continue;
            }
            if(board[x_min][y_min] > board[i][j]) {            
                min_pos = Pos(i, j);
                continue;
            }
            if(board[x_min][y_min] < board[i][j])
                continue;
            if(recent_attack[x_min][y_min] < recent_attack[i][j]) {            
                min_pos = Pos(i, j);
                continue;
            }
            if(recent_attack[x_min][y_min] > recent_attack[i][j])
                continue;
            if(x_min + y_min < i + j) {            
                min_pos = Pos(i, j);
                continue;
            }
            if(x_min + y_min > i + j)
                continue;
            if(y_min < j) {            
                min_pos = Pos(i, j);
                continue;
            }
        }
    }
    return min_pos;
}

Pos get_defender() {
    Pos max_pos = Pos(-1, -1);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(board[i][j] == 0) continue;
            int x_min = max_pos.first;
            int y_min = max_pos.second;
            if(max_pos == Pos(-1, -1) || board[x_min][y_min] < board[i][j]) {            
                max_pos = Pos(i, j);
                continue;
            }
            if(board[x_min][y_min] > board[i][j])
                continue;
            if(recent_attack[x_min][y_min] > recent_attack[i][j]) {            
                max_pos = Pos(i, j);
                continue;
            }
            if(recent_attack[x_min][y_min] < recent_attack[i][j])
                continue;
            if(x_min + y_min > i + j) {            
                max_pos = Pos(i, j);
                continue;
            }
            if(x_min + y_min < i + j)
                continue;
            if(y_min > j) {            
                max_pos = Pos(i, j);
                continue;
            }
        }
    }
    return max_pos;
}
const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {1, 0, -1, 0};
const int max_int = std::numeric_limits<int>::max();

bool try_laser_attack(Pos attacker, Pos defender) {
    //BFS from defender to attacker to acquire distance between defender and attacker    
    //from attacker, trace shortest-distance nodes
    std::vector<std::vector<int> > distance(N, std::vector<int>(M, max_int));
    std::vector<std::vector<bool> > visited(N, std::vector<bool>(M, false));
    std::queue<Pos> q;
    q.push(defender);
    distance[defender.first][defender.second] = 0;
    while(!q.empty()) {
        Pos now = q.front(); q.pop();        
        int x = now.first;
        int y = now.second;
        if(visited[x][y]) 
            continue;
        visited[x][y] = true;

        for(int i = 0; i < 4; i++) {
            int nx = (x + dx[i] + N) % N;
            int ny = (y + dy[i] + M) % M;
            if(board[nx][ny] == 0) 
                continue;

            q.push(Pos(nx, ny));
            distance[nx][ny] = distance[x][y] + 1 < distance[nx][ny] ? distance[x][y] + 1 : distance[nx][ny];
        }
    }

    Pos now = attacker;
    int atk = board[attacker.first][attacker.second];
    while(now != defender) {
        int min_dist = max_int;     
        Pos next = Pos(-1, -1);   
        for(int i = 0; i < 4; i++) {
            int nx = (now.first + dx[i] + N) % N;
            int ny = (now.second + dy[i] + M) % M;
            if(min_dist > distance[nx][ny]) {
                min_dist = distance[nx][ny];
                next = Pos(nx, ny);
            }
        }
        if(next == Pos(-1, -1))
            return false;
        int nx = next.first;
        int ny = next.second;

        board[nx][ny] -= (next == defender ? atk : atk / 2);
        modified[nx][ny] = true;
        if(board[nx][ny] <= 0) {
            num_alive--;
            board[nx][ny] = 0;
        }
        now = next;
    }
    return true;
}

void bomb_attack(Pos attacker, Pos defender) {
    const int dxdy[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, 
                            {0, -1},           {0, 1}, 
                            {1, -1},  {1, 0},  {1, 1}};
    const int atk = board[attacker.first][attacker.second];            
    for(int i = 0; i < 8; i++) {
        int nx = (defender.first + dxdy[i][1] + N) % N;
        int ny = (defender.second + dxdy[i][0] + M) % M;
        if(board[nx][ny] == 0) 
            continue;
        if(attacker == Pos(nx, ny))
            continue;
        board[nx][ny] -= atk / 2;
        modified[nx][ny] = true;
        if(board[nx][ny] <= 0) {
            num_alive--;
            board[nx][ny] = 0;
        }

    }
    int x = defender.first;
    int y = defender.second;
    board[x][y] -= atk;
    if(board[x][y] <= 0) {
        num_alive--;
        board[x][y] = 0;
    }
}
void debug() {
    std::cout << "///////////////////\n";
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "///////////////////\n";
}
int main() {
    init();

 //   debug();
    for(int turn = 1; turn <= K; turn++) {
        if(num_alive == 1)
            break;

        Pos attacker = get_attacker();         
    //    std::cout << attacker.first << ", " << attacker.second << " attackes \n";
        Pos defender = get_defender();
    //    std::cout << defender.first << ", " << defender.second << " defends \n";
        
        recent_attack[attacker.first][attacker.second] = turn;
        board[attacker.first][attacker.second] += bonus;
        if(!try_laser_attack(attacker, defender))
            bomb_attack(attacker, defender);      

        modified[defender.first][defender.second] = true;
        modified[attacker.first][attacker.second] = true;
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                if(modified[i][j]) 
                    modified[i][j] = false;
                else if(board[i][j] != 0)
                    board[i][j]++;

            }
        }
  //      debug();
    }

    int max_val = 0;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if(board[i][j] > max_val) {
                max_val = board[i][j];                
            }
        }
    }
    std::cout << max_val;
    return 0;
}