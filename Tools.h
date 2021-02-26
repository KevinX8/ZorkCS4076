#ifndef TOOLS_H_
#define TOOLS_H_

struct Coordinate {
    int x;
    int y;
};
struct Wall {
    unsigned int right : 2;
    /*
    0 means there's a wall to the right
    1 means there's no wall to the right
    2 means there's a wall with an unlocked door to the right.
    3 means there's a wall wiht a locked door to the right.
    */
    unsigned int down : 2;
};

class Tools
{
    private:
        static int width;
        friend class Floor;
    public:
        static bool compareKeys(int, int);
        static int getCoordinateKey(Coordinate c);
        static Coordinate getKeyCoordinate(int key);
        Tools() {};
};
#endif
