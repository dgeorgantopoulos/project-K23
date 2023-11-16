#include "functions.h"

using namespace std;


Polygon_2 incremental_alg(vector<Point_2> vect, int initialization, visible flag_visible){
    Polygon_2 polygonChain;

    switch (initialization)
    {
    case 1: // Descending by x
        sort(vect.begin(), vect.end(), compareDescending_x);
        break;
    case 2: // Ascending by x
        sort(vect.begin(), vect.end());
        break;
    case 3: // Descending by y
        sort(vect.begin(), vect.end(), compareDescending_y);
        break;
    case 4: // Ascending by y
        sort(vect.begin(), vect.end(), compareAscending_y);
        break;
    }

    Polygon_2 convexHull,chp;

    // Initializing the first triangle consisting from the 3 "rightest" points
    for (auto it = vect.rbegin(); it != vect.rbegin() + 3; ++it)
    { 
        polygonChain.push_back(*it);
        convexHull.push_back(*it);
    }
    

    // Calculating convex hull
    vector<Point_2> result;
    const Polygon_2::Vertices &range = convexHull.vertices();
    CGAL::convex_hull_2(range.begin(), range.end(), back_inserter(result));

    for(auto it = result.begin(); it!=result.end();++it){
        chp.push_back(*it);
    }


    Point_2 pToBeInserted;
    Polygon_2 newConvexHull = convexHull;
    vector<Point_2> newResult;
    Polygon_2 newChp;

    vector<Segment_2> seg1,seg2, redEdges;

    Point_2 p1,p2;


    for (auto it = vect.rbegin() + 3; it != vect.rend(); ++it)
    {
        pToBeInserted = *it;
        newConvexHull.push_back(pToBeInserted);

        const Polygon_2::Vertices &newRange = newConvexHull.vertices();
        CGAL::convex_hull_2(newRange.begin(), newRange.end(), back_inserter(newResult));

        for(auto it = newResult.begin(); it!=newResult.end();++it){
            newChp.push_back(*it);
        }

        // Old convex hull
        for(const Segment_2& e : chp.edges()){
            seg1.push_back(e);
        }

        // New convex hull
        for(const Segment_2& e : newChp.edges()){
            seg2.push_back(e);
        }

        // Finding the old's non common edges with new convex hull, aka , finding red edges
        for(auto i = seg1.begin(); i!= seg1.end(); ++i){
            if(find(seg2.begin(),seg2.end(),*i)== seg2.end()){
                redEdges.push_back(*i);
            }
        }


        bool trivial = false;
        bool done = false;
        int count = 0;
        long double max_area = -1;
        long double min_area;
        bool min = false;
        Point_2 minmax_point;
        int erase_count = 2;
        long double current_area;

        Point_2 correctOne;

        // Checking if red edge is inside polygonChain
        for(auto it = redEdges.begin(); it != redEdges.end(); ++it){
            if(find(polygonChain.edges_begin(),polygonChain.edges_end(),*it) != polygonChain.edges_end()){
                p1 = it->source();
                switch(flag_visible){
                    case random_vis: // Random
                        for(auto it = polygonChain.vertices_begin(); it!= polygonChain.vertices_end(); ++it){
                            if(*it == p1){
                                polygonChain.insert(it+1,pToBeInserted);
                                break;
                            }    
                        }
                        break;
                    case min_vis: // Smallest area
                        for(auto it = polygonChain.vertices_begin() ; it != polygonChain.vertices_end(); ++it){
                            if(*it == p1){
                                polygonChain.insert(it+1,pToBeInserted);
                                correctOne = *(it+1);
                                current_area = abs(polygonChain.area());
                                if(!min){
                                    min_area = current_area;
                                    minmax_point = correctOne; // We keep the point that, if we insert pToBeInserted after, gives the smallest area
                                    min = true; 
                                }
                                if(current_area < min_area && min == true){
                                    min_area = current_area;
                                    minmax_point = correctOne;  // We keep the point that, if we insert pToBeInserted after, gives the smallest area
                                }

                                for(auto i = polygonChain.vertices_begin(); i != polygonChain.vertices_end(); ++i){
                                    if(*i == pToBeInserted){
                                        polygonChain.erase(i);
                                        break;
                                    }
                                }
                                erase_count++;
                                break;
                            }
                        }
                        break;
                    case max_vis: // Largest area
                        for(auto it = polygonChain.vertices_begin() ; it != polygonChain.vertices_end(); ++it){
                            if(*it == p1){
                                polygonChain.insert(it+1,pToBeInserted);
                                correctOne = *(it+1);
                                current_area = abs(polygonChain.area());

                                if(current_area > max_area){
                                    max_area = current_area;
                                    minmax_point = correctOne;  // We keep the point that, if we insert pToBeInserted after, gives the largest area
                                }

                                for(auto i = polygonChain.vertices_begin(); i != polygonChain.vertices_end(); ++i){
                                    if(*i == pToBeInserted){
                                        polygonChain.erase(i);
                                        break;
                                    }
                                }
                                erase_count++;
                                break;
                            }
                        }
                        break;
                }

                trivial = true;
                done = true;
                if(flag_visible == random_vis) break;  // If we are picking randomly, then we dont have to check the other red edges!            
            }
        }

        erase_count = 0;
        
        count = 0;
        bool red;
        int vert_count;
        // If we are picking randomly, we get inside here only if we didn't find anything above, in the "trivial" case
        while((!done && flag_visible == random_vis) || flag_visible != random_vis){ 
            if(flag_visible == random_vis && done == true) break;
            red = false;
            if(!trivial || flag_visible != random_vis){
                switch(flag_visible){
                    case random_vis: // Random here
                        for (auto it = redEdges.begin() + count; it != redEdges.end();++it){
                            p1 = it->source();
                            p2 = it->target();
                            break; 
                        }

                        vert_count = 0;
                        for (auto it = polygonChain.vertices_begin(); it != polygonChain.vertices_end(); ++it){

                            for(auto i1 = polygonChain.vertices_begin() + vert_count; i1 != polygonChain.vertices_end();++i1){
                                polygonChain.insert(i1,pToBeInserted);
                                break;
                            }


                            // Checking if it is a redEdge
                            for(auto i = redEdges.begin(); i != redEdges.end();++i){
                                if((i->source() == *it && i->target() == pToBeInserted) || (i->target() == *it && i->source() == pToBeInserted)){
                                    red = true;
                                }
                            }
                            if(red){ // Because we are checking everything, we might be unfortunate and pick a redEdge (that will be visible), so we eliminate this edge case 
                                red = false;
                                for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                    polygonChain.erase(i);
                                    break;
                                }
                                continue;
                            }

                            // We always have to check if what we try is simple (and therefore valid) or not (if not we continue searching)
                            if(polygonChain.is_simple()){
                                done = true;
                                break;
                            }
                            
                            for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                polygonChain.erase(i);
                                break;
                            }

                            vert_count++;
                        }
                            break;
                        case min_vis: //Smallest area
                            for (auto it = redEdges.begin(); it != redEdges.end();++it){
                                p1 = it->source();
                                p2 = it->target();

                                vert_count = 0;
                                int total_count = 0; 
                                for (auto it = polygonChain.vertices_begin(); it != polygonChain.vertices_end(); ++it){
                                    if(total_count == polygonChain.size()) break;
                                    total_count++;

                                    for(auto i1 = polygonChain.vertices_begin() + vert_count; i1 != polygonChain.vertices_end();++i1){
                                        correctOne = *i1;
                                        polygonChain.insert(i1,pToBeInserted);
                                        break;
                                    }


                                    // Checking if it is a redEdge
                                    for(auto i = redEdges.begin(); i != redEdges.end();++i){
                                        if((i->source() == *it && i->target() == pToBeInserted) || (i->target() == *it && i->source() == pToBeInserted)){
                                            red = true;
                                        }
                                    }
                                    if(red){
                                        red = false;
                                        for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                            polygonChain.erase(i);
                                            break;
                                        }
                                        continue;
                                    }
                                    // We always have to check if what we try is simple (and therefore valid but we still continue 
                                    // search for better solutions) or not 
                                    if(polygonChain.is_simple()){
                                        done = true;
                                        current_area = abs(polygonChain.area());
                                        if(!min){
                                            min_area = current_area;
                                            minmax_point = correctOne;
                                            min = true; 
                                        }
                                        if(current_area < min_area && min == true){
                                            min_area = current_area;
                                            minmax_point = correctOne;
                                        }                                                
                                    }

                                    int c = 0;
                                    for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                        c++;
                                        polygonChain.erase(i);
                                        break;
                                    }

                                    vert_count++;
                                }
                            }
                            break;

                        case max_vis: // Largest area
                            for (auto it = redEdges.begin(); it != redEdges.end();++it){
                                p1 = it->source();
                                p2 = it->target();
                                
                                vert_count = 0;
                                int total_count = 0;
                                for (auto it = polygonChain.vertices_begin(); it != polygonChain.vertices_end(); ++it){
                                    if(total_count == polygonChain.size()) break;
                                    total_count++;

                                    for(auto i1 = polygonChain.vertices_begin() + vert_count; i1 != polygonChain.vertices_end();++i1){
                                        correctOne = *i1;
                                        polygonChain.insert(i1,pToBeInserted);
                                        break;
                                    }


                                    // Checking if it is a redEdge
                                    for(auto i = redEdges.begin(); i != redEdges.end();++i){
                                        if((i->source() == *it && i->target() == pToBeInserted) || (i->target() == *it && i->source() == pToBeInserted)){
                                            red = true;
                                        }
                                    }
                                    if(red){
                                        red = false;
                                        for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                            polygonChain.erase(i);
                                            break;
                                        }
                                        continue;
                                    }
                                    // We always have to check if what we try is simple (and therefore valid but we still continue 
                                    // search for better solutions) or not 
                                    if(polygonChain.is_simple()){
                                        done = true;
                                        current_area = abs(polygonChain.area());
                                        if(current_area > max_area){
                                            max_area = current_area;
                                            minmax_point = correctOne;
                                        }                                                
                                    }
                                    
                                    for(auto i = polygonChain.vertices_begin() + vert_count; i != polygonChain.vertices_end(); ++i){
                                        polygonChain.erase(i);
                                        break;
                                    }

                                    vert_count++;
                                }
                            }
                            break;
                }
            }
            count++;
            if(flag_visible != random_vis) break; // We have checked the possibility if there wasn't a visibile edge behind the red edge
        }

        if(flag_visible != random_vis){
            for(auto it = polygonChain.vertices_begin(); it!= polygonChain.vertices_end(); ++it){
                if(*it == minmax_point){
                    polygonChain.insert(it,pToBeInserted);
                    break;
                }    
            }
        }                

        convexHull = newConvexHull;
        chp = newChp;
        result = newResult;

        newChp.clear();
        newResult.clear();
        seg1.clear();
        seg2.clear();
        redEdges.clear();    
    }

    return polygonChain;
}
