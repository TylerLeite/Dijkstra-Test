#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "grid2d.h"

#define BIG_NUM 10000

#define NORTH      4
#define NORTHEAST  7
#define EAST       3
#define SOUTHEAST 15
#define SOUTH     12
#define SOUTHWEST 13
#define WEST       1
#define NORTHWEST  5

#define sz 10

bool canMove(Grid2d<char>layout, int x, int y, int direction){
    bool can_move = true;
    
    switch (direction){
        case NORTH:
            can_move &= layout.getAt(x, y-1) != '#';
            break;
        case NORTHEAST:
            can_move &= layout.getAt(x, y-1) != '#';
            can_move &= layout.getAt(x+1, y) != '#';
            can_move &= layout.getAt(x+1, y-1) != '#';
            break;
        case EAST:
            can_move &= layout.getAt(x+1, y) != '#';
            break;
        case SOUTHEAST:
            can_move &= layout.getAt(x, y+1) != '#';
            can_move &= layout.getAt(x+1, y) != '#';
            can_move &= layout.getAt(x+1, y+1) != '#';
            break;
        case SOUTH:
            can_move &= layout.getAt(x, y+1) != '#';
            break;
        case SOUTHWEST:
            can_move &= layout.getAt(x, y+1) != '#';
            can_move &= layout.getAt(x-1, y) != '#';
            can_move &= layout.getAt(x-1, y+1) != '#';
            break;
        case WEST:
            can_move &= layout.getAt(x-1, y) != '#';
            break;
        case NORTHWEST:
            can_move &= layout.getAt(x, y-1) != '#';
            can_move &= layout.getAt(x-1, y) != '#';
            can_move &= layout.getAt(x-1, y-1) != '#';
            break;
        default:
            break;
    }
    
    return can_move;
}

int main(int argc, char** argv){
    Grid2d<char> layout(sz, sz, '.', '#');
    Grid2d<bool> unvisited(sz, sz, true, false);
    Grid2d<int> dijkstra(sz, sz, BIG_NUM, -1);
    
    std::ifstream ifs("map");

    int start_x, start_y;
    int end_x, end_y;
    
    for (int x = 0; x < sz; x++){
        for (int y = 0; y < sz; y++){
            char tmp;
            ifs >> tmp;
            if (tmp == '\n') ifs >> tmp;
            layout.setAt(x, y, tmp);
            //std::cout << layout.getAt(x, y);
            if (layout.getAt(x, y) == 'S'){
                dijkstra.setAt(x, y, 0);
                start_x = x;
                start_y = y;
            } else if (layout.getAt(x, y) == 'E'){
                end_x = x;
                end_y = y;
            } else if (layout.getAt(x, y) == '#'){
                unvisited.setAt(x, y, false);
            }
        }
        
        //std::cout << std::endl;
    }
    
    while (unvisited.getAt(end_x, end_y)){
        int min = BIG_NUM + 1;
        int cur_x, cur_y;
        for (int x = 0; x < sz; x++){
            for (int y = 0; y < sz; y++){
                if (unvisited.getAt(x, y)){
                    if (dijkstra.getAt(x, y) < min){
                        cur_x = x;
                        cur_y = y;
                        min = dijkstra.getAt(x, y);
                    }
                }
            }
        }
        
        for (int m = -1; m < 2; m++){
            for (int n = -1; n < 2; n++){
                int x = cur_x + m;
                int y = cur_y + n;
                if (dijkstra.inBounds(x, y) && canMove(layout, cur_x, cur_y, m+2 + 4*(n+2))){
                    if (unvisited.getAt(x, y)){
                        int tenative = dijkstra.getAt(cur_x, cur_y) + 1;
                        if (tenative < dijkstra.getAt(x, y)){
                            dijkstra.setAt(x, y, tenative);
                        }
                    }
                }
            }
        }
        
        unvisited.setAt(cur_x, cur_y, false);
    }

    Grid2d<bool> path(sz, sz, false, false);
    int sanity = sz000;
    int x = end_x, y = end_y;
    path.setAt(end_x, end_y, true);
    int pathlen = 0;
    while (true){
        sanity--;
        if (sanity < 0) break;
        int cur_val = dijkstra.getAt(x, y);
        int cur_x = x, cur_y = y;

        //cardinal directions first
        for (int m = -1; m < 2; m++){
            for (int n = -1; n < 2; n++){
                if (m == 0 || n == 0){
                    if (dijkstra.inBounds(x+m, y+n) && canMove(layout, x, y, m+2 + 4*(n+2))){
                        if (dijkstra.getAt(x+m, y+n) < cur_val){
                            cur_val = dijkstra.getAt(x+m, y+n);
                            cur_x = x+m;
                            cur_y = y+n;
                        }
                    }
                }
            }
        }
        
        //now diagonals
        for (int m = -1; m < 2; m++){
            for (int n = -1; n < 2; n++){
                if (m != 0 && n != 0){
                    if (dijkstra.inBounds(x+m, y+n) && canMove(layout, x, y, m+2 + 4*(n+2))){
                        if (dijkstra.getAt(x+m, y+n) < cur_val){
                            cur_val = dijkstra.getAt(x+m, y+n);
                            cur_x = x+m;
                            cur_y = y+n;
                        }
                    }
                }
            }
        }

        if (cur_x == x && cur_y == y){
            break;
        }

        if (x == start_x){
            if (y == start_y){
                break;
            }
        }
        
        path.setAt(cur_x, cur_y, true);
        pathlen++;
        x = cur_x;
        y = cur_y;
    }

    if (pathlen == 0){
        std::cout << "No path found" << std::endl;
    } else {
        std::cout << "Path found, length: " << pathlen << std::endl;
    }
    for (int x = 0; x < sz; x++){
        for (int y = 0; y < sz; y++){
            if (x == start_x && y == start_y){
                std::cout << 'S';
            } else if (x == end_x && y == end_y){
                std::cout << 'E';
            } else if (path.getAt(x, y)){
                std::cout << '+';
            } else {
                std::cout << layout.getAt(x, y);
            }
        }
        std::cout << "\n";
    }

    std::cout << std::endl;
    
    return 0;
}
