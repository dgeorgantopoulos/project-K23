#include <iostream>
#include <string>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/convex_hull_2.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Point_2<K> Point_2;
typedef CGAL::Segment_2<K> Segment_2;
typedef std::vector<Point_2>::iterator veciterator;
typedef std::istream_iterator<Point_2> point2_iterator;

enum algorithm { incremental, convex_hull, onion};
enum visible { random_vis, min_vis, max_vis};

Polygon_2 convex_hull_alg(std::vector<Point_2> , visible);
Polygon_2 incremental_alg(std::vector<Point_2>, int, visible);

struct minTotal
{
    double min_distance;
    Segment_2 min_ch_edge;
    Point_2 min_point;
    double min_area;
};

std::vector<Point_2> readPoints(char *);
unsigned long long readArea(char *);

bool compareDescending_x(Point_2, Point_2);

bool compareAscending_y(Point_2 p1, Point_2 p2);

bool compareDescending_y(Point_2 p1, Point_2 p2);
