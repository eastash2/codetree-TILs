#include <iostream>
#include <vector>

struct Node {
    int next, prev;
    bool is_head;
    Node() : next(-1), prev(-1), is_head(true) {}
};

int move_all(std::vector<int>& belt, std::vector<int>& belt_size, std::vector<Node>& present) {
    int m_src, m_dst;
    std::cin >> m_src >> m_dst;
    m_src--; m_dst--;

    if(belt_size[m_src] == 0) 
        return belt_size[m_dst];

    int src_head = belt[m_src];
    int src_tail = present[src_head].prev;   

    if(belt_size[m_dst] != 0) {
        int dst_head = belt[m_dst];
        int dst_tail = present[dst_head].prev;
        present[src_head].prev = dst_tail;
        present[dst_tail].next = src_head;
        present[src_tail].next = dst_head;
        present[dst_head].prev = src_tail;
        present[dst_head].is_head = false;
    }    
    belt[m_src] = -1;
    belt[m_dst] = src_head;
    belt_size[m_dst] += belt_size[m_src];
    belt_size[m_src] = 0;
    return belt_size[m_dst];
}

int move_front(std::vector<int>& belt, std::vector<int>& belt_size, std::vector<Node>& present) {
    int m_src, m_dst;
    std::cin >> m_src >> m_dst;
    m_src--; m_dst--;

    if(belt_size[m_src] == 0 && belt_size[m_dst] == 0)
        return 0;

    if(belt_size[m_src] == 0) {
        int front = belt[m_dst];
        present[present[front].next].prev = present[front].prev;
        present[present[front].prev].next = present[front].next;
        if(belt_size[m_dst] == 1) {
            belt[m_dst] = -1;    
        } else {
            belt[m_dst] = present[front].next;
            present[belt[m_dst]].is_head = true;
        }        
        belt[m_src] = front;
        present[front].next = front;
        present[front].prev = front;
        belt_size[m_src] = 1;
        belt_size[m_dst]--;        
    } else if(belt_size[m_dst] == 0) {
        int front = belt[m_src];
        present[present[front].next].prev = present[front].prev;
        present[present[front].prev].next = present[front].next;        
        if(belt_size[m_src] == 1) {
            belt[m_src] = -1;    
        } else {
            belt[m_src] = present[front].next;
            present[belt[m_src]].is_head = true;
        }  
        belt[m_dst] = front;        
        present[front].next = front;
        present[front].prev = front;
        belt_size[m_dst] = 1;
        belt_size[m_src]--;
    } else {
        int src_head = belt[m_src];
        int dst_head = belt[m_dst];
        belt[m_src] = dst_head;
        belt[m_dst] = src_head;
        if(belt_size[m_src] > 1) {
            present[present[src_head].next].prev = dst_head;
            present[present[src_head].prev].next = dst_head;
        }
        if(belt_size[m_dst] > 1) {
            present[present[dst_head].next].prev = src_head;        
            present[present[dst_head].prev].next = src_head;
        }
        
        
        int temp = present[src_head].next;
        present[src_head].next = present[dst_head].next;
        present[dst_head].next = temp;

        temp = present[src_head].prev;
        present[src_head].prev = present[dst_head].prev;
        present[dst_head].prev = temp;
        if(belt_size[m_src] == 1) {
            present[dst_head].next = dst_head;
            present[dst_head].prev = dst_head;
        }
        if(belt_size[m_dst] == 1) {
            present[src_head].next = src_head;
            present[src_head].prev = src_head;
        }
    }


    return belt_size[m_dst];
}

int move_half(std::vector<int>& belt, std::vector<int>& belt_size, std::vector<Node>& present) {
    int m_src, m_dst;
    std::cin >> m_src >> m_dst;
    m_src--; m_dst--;

    if(belt_size[m_src] <= 1)
        return belt_size[m_dst];

    int half = belt_size[m_src] / 2;
    int src_tail = belt[m_src];
    while(--half) {
        src_tail = present[src_tail].next;
    }
    int src_head = belt[m_src];
    int new_src_head = present[src_tail].next;
    present[new_src_head].prev = present[src_head].prev;
    present[present[src_head].prev].next = new_src_head;

    if(belt_size[m_dst] != 0) {
        int dst_head = belt[m_dst];
        int dst_tail = present[dst_head].prev;    
        present[dst_head].prev = src_tail;
        present[dst_head].is_head = false;
        present[src_head].prev = dst_tail;
        present[dst_tail].next = src_head;    
        present[src_tail].next = dst_head;
    } else {
        present[src_head].prev = src_tail;
        present[src_tail].next = src_head;
    }
    
    belt[m_dst] = src_head;
    belt[m_src] = new_src_head;
    present[new_src_head].is_head = true;
    belt_size[m_dst] += belt_size[m_src] / 2;
    belt_size[m_src] -= belt_size[m_src] / 2;
    return belt_size[m_dst];
}

void debug(const std::vector<int>& belt, const std::vector<int>& belt_size, const std::vector<Node>& present) {    
    std::cout << "////////////////////\n";

    for(int p : belt) {
        while(true) {
            if(p == -1) break;

            std::cout << p << " ";
            if(present[present[p].next].is_head)
                break;
            p = present[p].next;
        }
        std::cout << "\n";
    }
    std::cout << "////////////////////\n";
}

int main() {
    int q;
    std::cin >> q;
    q--;
    int n, m;
    std::cin >> n; //100
    std::cin >> n >> m;
    std::vector<int> belt(n, -1);
    std::vector<int> belt_size(n, 0);
    //0th element not used
    std::vector<Node> present(m + 1, Node());    
    for(int i = 1; i <= m; i++) {     
        int B_NUM;
        std::cin >> B_NUM;
        B_NUM--;       
        belt_size[B_NUM]++;
        if(belt[B_NUM] == -1) {
            belt[B_NUM] = i;
            present[i].next = i;
            present[i].prev = i;
        } else {
            present[i].next = belt[B_NUM];
            present[i].prev = present[belt[B_NUM]].prev;
            present[i].is_head = false;
            present[belt[B_NUM]].prev = i;
            present[present[i].prev].next = i;            
        }
        
    }
  //  debug(belt, belt_size, present);
    while(q--) {        
        int command;
        std::cin >> command;
        if(command == 200) {
            std::cout << move_all(belt, belt_size, present) << "\n";
        } else if(command == 300) {
            std::cout << move_front(belt, belt_size, present) << "\n";
        } else if(command == 400) {
            std::cout << move_half(belt, belt_size, present) << "\n";
        } else if(command == 500) {
            int p_num;
            std::cin >> p_num;
            int a, b;
            a = present[p_num].is_head ? -1 : present[p_num].prev;
            b = present[present[p_num].next].is_head ? -1 : present[p_num].next;
            std::cout << (a + b * 2) << "\n";
        } else if(command == 600) {
            int b_num;
            std::cin >> b_num;
            b_num--;
            if(belt[b_num] == -1) 
                std::cout << -3 << "\n";
            else
                std::cout << belt[b_num] + present[belt[b_num]].prev * 2 + belt_size[b_num] * 3 << "\n";
        }
    //    debug(belt, belt_size, present);
    }
    return 0;
}