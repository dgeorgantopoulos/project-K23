#include "functions.h"

using namespace std;

Polygon_2 convex_hull_alg(vector<Point_2> vect, visible flag_visible)
{
    Polygon_2 polygonChain, convexHull;

    // Pushing all the points in the main data structure in order to compute the first convex hull
    for (auto it = vect.begin(); it != vect.end(); ++it)
    {
        convexHull.push_back(*it);
    }

    vector<Point_2> result;
    const Polygon_2::Vertices &range = convexHull.vertices();
    CGAL::convex_hull_2(range.begin(), range.end(), back_inserter(result));

    Polygon_2 polygonChainCopy;
    for (auto it = result.begin(); it != result.end(); ++it)
    {
        polygonChain.push_back(*it);
    }

    vector<Point_2> interiorPoints;
    for (auto i = vect.begin(); i != vect.end(); ++i)
    {
        if (find(result.begin(), result.end(), *i) == result.end())
        {
            interiorPoints.push_back(*i);
        }
    }

    // We are using long double because the distances exceed 2^32 (unsigned int for example)
    long double min_distance;

    Segment_2 min_ch_edge;
    Point_2 min_point;
    bool visibility = true;
    bool finished;

    // for case 2,3
    vector<minTotal> tot;
    minTotal total;
    long double area;
    long double min_area;
    bool min_area_bool = true;
    minTotal min_total;
    long double max_area = -1;
    minTotal max_total;
    //

    while (interiorPoints.size() != 0) // While there are still points to be included in the polygon Chain
    {
        bool min_dist = false;
        tot.clear();
        switch (flag_visible)
        {
        case random_vis: // random
            for (auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it)
            {
                polygonChainCopy = polygonChain;
                for (auto i = interiorPoints.begin(); i != interiorPoints.end(); ++i) // Checking all the interior points that are left
                {
                    if (!min_dist) // If no min has been initialized
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);
                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false; // Not continue the outer loop because we want to erase it in the next for loop 
                                }
                                break;
                            }
                        }

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue; // Now that we have erased it, we can continue to the next one
                        }

                        // It is visible so we establish it as our first min
                        min_dist = true;
                        min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        min_ch_edge = *it;
                        min_point = *i;
                    }
                    if (sqrt(CGAL::squared_distance(*it, *i)) < min_distance)
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);

                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false; // Not continue the outer loop because we want to erase it in the next for loop 
                                }
                                break;
                            }
                        }

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue; // Now that we have erased it, we can continue to the next one
                        }

                        // It is visible and the distance is smaller so we can reinitialize it as our new min
                        min_dist = true;
                        min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        min_ch_edge = *it;
                        min_point = *i;
                    }
                }
            }

            finished = false;
            for (auto it1 = polygonChain.edges_begin(); it1 != polygonChain.edges_end(); ++it1)
            {
                if (*it1 == min_ch_edge)
                {
                    for (auto i = polygonChain.vertices_begin(); i != polygonChain.vertices_end(); ++i)
                    {
                        if (*i == it1->source())
                        {
                            polygonChain.insert(i + 1, min_point);
                            interiorPoints.erase(remove(interiorPoints.begin(), interiorPoints.end(), min_point), interiorPoints.end());
                            finished = true;
                            break;
                        }
                    }
                }
                if (finished)
                    break;
            }

            break;
        case min_vis: // min
            for (auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it)
            {
                polygonChainCopy = polygonChain;
                min_dist = false;
                for (auto i = interiorPoints.begin(); i != interiorPoints.end(); ++i)
                {
                    if (!min_dist)
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);
                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false;
                                }
                                break;
                            }
                        }

                        area = abs(polygonChainCopy.area());

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue;
                        }

                        min_dist = true;
                        total.min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        total.min_ch_edge = *it;
                        total.min_point = *i;
                        total.min_area = area;

                        tot.push_back(total);
                    }
                    if (sqrt(CGAL::squared_distance(*it, *i)) < min_distance)
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);
                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false;
                                }
                                break;
                            }
                        }

                        area = abs(polygonChainCopy.area());

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue;
                        }

                        min_dist = true;
                        total.min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        total.min_ch_edge = *it;
                        total.min_point = *i;
                        total.min_area = area;

                        tot.push_back(total);
                    }
                }
            }
            for (auto it = tot.begin(); it != tot.end(); ++it)
            {
                if (min_area_bool)
                {
                    min_area = it->min_area;
                    min_total = *it;
                    min_area_bool = false;
                }
                if (it->min_area < min_area)
                {
                    min_area = it->min_area;
                    min_total = *it;
                }
            }

            min_area_bool = true;

            finished = false;
            for (auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it)
            {
                if (*it == min_total.min_ch_edge)
                {
                    for (auto i = polygonChain.vertices_begin(); i != polygonChain.vertices_end(); ++i)
                    {
                        if (*i == it->source())
                        {
                            polygonChain.insert(i + 1, min_total.min_point);
                            // Finding and erasing for the list of interior points, the point we just added!
                            interiorPoints.erase(remove(interiorPoints.begin(), interiorPoints.end(), min_total.min_point), interiorPoints.end());
                            finished = true;
                            break;
                        }
                    }
                }
                if (finished)
                    break;
            }

            break;
        case max_vis: // max
            for (auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it)
            {
                polygonChainCopy = polygonChain;
                min_dist = false;
                for (auto i = interiorPoints.begin(); i != interiorPoints.end(); ++i)
                {
                    if (!min_dist)
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);
                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false;
                                }
                                break;
                            }
                        }

                        area = abs(polygonChainCopy.area());

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue;
                        }

                        min_dist = true;
                        total.min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        total.min_ch_edge = *it;
                        total.min_point = *i;
                        total.min_area = area;

                        tot.push_back(total);
                    }
                    if (sqrt(CGAL::squared_distance(*it, *i)) < min_distance)
                    {
                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == it->source())
                            {
                                polygonChainCopy.insert(iter + 1, *i);
                                if (!polygonChainCopy.is_simple())
                                {
                                    visibility = false;
                                }
                                break;
                            }
                        }

                        area = abs(polygonChainCopy.area());

                        for (auto iter = polygonChainCopy.vertices_begin(); iter != polygonChainCopy.vertices_end(); ++iter)
                        {
                            if (*iter == *i)
                            {
                                polygonChainCopy.erase(iter);
                                break;
                            }
                        }

                        if (!visibility)
                        {
                            visibility = true;
                            continue;
                        }

                        min_dist = true;
                        total.min_distance = sqrt(CGAL::squared_distance(*it, *i));
                        total.min_ch_edge = *it;
                        total.min_point = *i;
                        total.min_area = area;

                        tot.push_back(total);
                    }
                }
            }
            for (auto it = tot.begin(); it != tot.end(); ++it)
            {
                if (it->min_area > max_area)
                {
                    max_area = it->min_area;
                    max_total = *it;
                }
            }

            max_area = -1;

            finished = false;
            for (auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it)
            {
                if (*it == max_total.min_ch_edge)
                {
                    for (auto i = polygonChain.vertices_begin(); i != polygonChain.vertices_end(); ++i)
                    {
                        if (*i == it->source())
                        {
                            polygonChain.insert(i + 1, max_total.min_point);
                            // Finding and erasing for the list of interior points, the point we just added!
                            interiorPoints.erase(remove(interiorPoints.begin(), interiorPoints.end(), max_total.min_point), interiorPoints.end());
                            finished = true;
                            break;
                        }
                    }
                }
                if (finished)
                    break;
            }

            break;
        }
    }

    return polygonChain;
}
