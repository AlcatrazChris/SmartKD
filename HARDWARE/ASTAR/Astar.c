#include "Astar.h"

bool kown[MAP_LENGTH][MAP_WIDTH];
bool able[MAP_LENGTH][MAP_WIDTH];
int   way[MAP_LENGTH][MAP_WIDTH];
int   dis[MAP_LENGTH][MAP_WIDTH];


// cq
const int size = MAP_LENGTH * MAP_WIDTH + 5;
int head, last, data[size];
int front() { return data[head]; }
int back() { return data[last - 1]; }
void push(int v) {
    if(last == size) last = 0;
    data[last++] = v;
} 
void pop() { if(++head == size) head = 0; }
bool empty() { return head == last; }

int code(int x, int y) { return x * MAP_WIDTH + y; }
void decode(int c, int* x, int* y)
    { *x = c / MAP_WIDTH; *y = c % MAP_WIDTH;  }


void Astar_Init() {
    memset(kown,  0, sizeof(kown));
    memset(able, -1, sizeof(able));
}

bool invalid(int x, int y) {
    return x < 0 || x >= MAP_LENGTH
        || y < 0 || y >= MAP_WIDTH
        || !able[x][y];
}

void update(int x, int y) {
    kown[x][y] = 1;
	//*
    if(INFRARED_FRONT && !invalid(x, y + 1))
        able[x][y + 1] = 0;
    if(INFRARED_LEFT  && !invalid(x - 1, y))
        able[x - 1][y] = 0;
    if(INFRARED_BACK  && !invalid(x, y - 1))
        able[x][y - 1] = 0;
    if(INFRARED_RIGHT && !invalid(x + 1, y))
        able[x + 1][y] = 0;
	//*/
}

void getNext(int x, int y, int* nx, int* ny, int dirc, int distance) {
    switch (dirc) {
    case DIRC_FORWARD: *nx = x; *ny = y + distance; break;
    case DIRC_BACK:    *nx = x; *ny = y - distance; break;
    case DIRC_LEFT:    *ny = y; *nx = x - distance; break;
    case DIRC_RIGHT:   *ny = y; *nx = x + distance; break;
    default: break;
    }
}

void Astar(int x, int y, int dx, int dy, int dirc) {
    dis[dx][dy] = 0;
    push(code(dx, dy));
    int nx, ny;
    while (!empty()) {
        decode(front(), &nx, &ny);
        pop();
        if (!invalid(nx + 1, ny) && dis[nx + 1][ny] == -1) {
            dis[nx + 1][ny] = dis[nx][ny] + 1;
            push(code(nx + 1, ny));
        }
        if (!invalid(nx - 1, ny) && dis[nx - 1][ny] == -1) {
            dis[nx - 1][ny] = dis[nx][ny] + 1;
            push(code(nx - 1, ny));
        }
        if (!invalid(nx, ny + 1) && dis[nx][ny + 1] == -1) {
            dis[nx][ny + 1] = dis[nx][ny] + 1;
            push(code(nx, ny + 1));
        }
        if (!invalid(nx, ny - 1) && dis[nx][ny - 1] == -1) {
            dis[nx][ny - 1] = dis[nx][ny] + 1;
            push(code(nx, ny - 1));
        }
    }

//    for (int i = 0; i < MAP_LENGTH; ++i) {
//        for (int j = 0; j < MAP_WIDTH; ++j) {
//            printf("%d ", dis[i][j]);
//        }
//        putchar('\n');
//    }

    getNext(x, y, &nx, &ny, dirc, 1);
    if (invalid(nx, ny) || dis[nx][ny] != dis[x][y] - 1) {
        for (int i = 0; i < 4; ++i) {
            if (i == dirc) continue;
            getNext(x, y, &nx, &ny, i, 1);
            if (!invalid(nx, ny) && dis[nx][ny] == dis[x][y] - 1) {
                dirc = i; break;
            }
        }
    }
    while (!invalid(nx, ny) && dis[nx][ny] == dis[x][y] - 1) {
        way[x][y] = dirc;
        x = nx; y = ny;
        getNext(nx, ny, &nx, &ny, dirc, 1);
    }
    if(!invalid(x, y)) way[x][y] = dirc + 1;
}

 void findWay(int x, int y, int dx, int dy, int* dirc, int* distance) {
		able[0][MAP_WIDTH - 1] = able[MAP_LENGTH - 1][MAP_WIDTH - 1] = 1;
    head = last = 0;
    memset(dis, -1, sizeof(dis));

    Astar(x, y, dx, dy, *dirc);

    *distance = 0;
    *dirc = way[x][y];
    while (kown[x][y] && *dirc == way[x][y]) {
        getNext(x, y, &x, &y, *dirc, 1);
        ++*distance;
    }
}
