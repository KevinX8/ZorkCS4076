#ifndef TOOLS_H_
#define TOOLS_H_

struct Coordinate {
    int x;
    int y;
};
class Tools
{
    private:
        static int width;
        friend class Floor;
    public:
        static bool compareCoordinates(Coordinate, Coordinate);
        static int getCoordinateKey(Coordinate c);
        static Coordinate getKeyCoordinate(int key);
        Tools() {};
};
#endif