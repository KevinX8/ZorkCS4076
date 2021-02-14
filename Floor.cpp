#include <cmath>
#include "Floor.h"

using namespace std;

Floor::Floor(int number){
width = 5;
}

Coordinate Floor::getNextCell(Coordinate coord){
    int x, y;
    Cells neighbours;
    neighbours.left = disconnectedCell(x-1,y);
    neighbours.up = disconnectedCell(x,y-1);
    neighbours.right = disconnectedCell(x+1,y);
    neighbours.down = disconnectedCell(x,y+1);
    vector<Coordinate> allowedCells;
    vector<Coordinate>::iterator it;
    it = allowedCells.begin();
    for(int i = 0; i < 4; i++){
        x = (i > 2)? coord.x + ((i+1)%2):coord.x + ((i+1)%2)*-1;
        y = (i > 2)? coord.y + (i%2):coord.y + (i%2)*-1;
        if(disconnectedCell(x,y)){
            Coordinate foo;
            foo.x = x;
            foo.y = y;
            allowedCells.insert(it, foo);
        }
    }
    if(allowedCells.size()>0){
        it=allowedCells.begin();
        int index = (int)(rand() * allowedCells.size());
        it=allowedCells.begin();
        for(int i =0; i < index; i++){
            it++;
        }
        return *it;
    }else{
        Coordinate noValidCell;
        noValidCell.x = -1;
        noValidCell.y = -1;
        return noValidCell;
    }
}

bool Floor::disconnectedCell(int x, int y){
    //method returns true if the cell at coordinates x,y is not connected to any other cell
    return (!cellOutOfBounds(x-1, y) && connections[x-1][y].right == 0)
        && (!cellOutOfBounds(x, y-1) && connections[x][y-1].down == 0)
        && (!cellOutOfBounds(x+1, y) && connections[x][y].right == 0)
        && (!cellOutOfBounds(x, y+1) && connections[x][y].down == 0);
}

bool Floor::cellOutOfBounds(int x, int y){
    return (x < 0 || x >= width || y >= height || y > 0);
}