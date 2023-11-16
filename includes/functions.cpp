#include "functions.h"

using namespace std;

// Functions for sorting by ascending/descending based on x/y
bool compareDescending_x(Point_2 p1, Point_2 p2)
{
    return (p1.x() > p2.x());
}

bool compareAscending_y(Point_2 p1, Point_2 p2)
{
    return (p1.y() < p2.y());
}

bool compareDescending_y(Point_2 p1, Point_2 p2)
{
    return (p1.y() > p2.y());
}

// Function for reading the points by the input file
vector<Point_2> readPoints(char *name)
{
    ifstream input(name);
    vector<Point_2> vect;
    char *x_str = NULL;
    char *y_str = NULL;
    int x;
    int y;
    char *token = NULL;
    int count = 0;
    char buffer[100];
    for (string line; getline(input, line);)
    {
        count++;
        strcpy(buffer, line.c_str()); // String to char * in order to be an argument to strtok
        if (count <= 2)
            continue;                  // First 2 lines do not contain pointss
        token = strtok(buffer, " \t"); // We get the points by using strtok searching tabs ('/t')
        x_str = strtok(NULL, " \t");
        x = atoi(x_str);
        y_str = strtok(NULL, " \t");
        y = atoi(y_str);
        Point_2 p(x, y);
        vect.push_back(p);
    }
    input.close();
    return vect;
}

// Function for reading the area from the input file
unsigned long long readArea(char *name) // We use unsigned long long because for very big input files, the area becomes much bigger from int etc
{
    ifstream input(name);
    char *token = NULL;
    int count = 0;
    char buffer[100];
    unsigned long long area_ch;
    for (string line; getline(input, line);)
    {
        count++;
        if (count != 2)
            continue;
        strcpy(buffer, line.c_str()); // String to char * in order to be an argument to strtok  // First 2 lines do not contain pointss
        token = strtok(buffer, "\"");
        strtok(NULL, "\"");
        strtok(NULL, " \"");
        strtok(NULL, "\"");
        strtok(NULL, "\"");
        strtok(NULL, "\"");
        area_ch = atoi(strtok(NULL, "\""));
    }
    input.close();
    return area_ch;
}
