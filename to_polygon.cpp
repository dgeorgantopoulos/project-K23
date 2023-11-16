#include "includes/functions.h"

//For the new file
#include <iostream>
#include <fstream> 

//For timing
#include <chrono>

using namespace std;


int main(int argc, char **argv)
{

    visible flag_visible;
    algorithm flag_algorithm;
    int initialization;
    unsigned long long area_ch;

    // Reading the points from the file and command line argumemt checks 
    vector<Point_2> vect;
    char *outputFile;
    bool arg_i = false, arg_o = false, arg_alg = false, arg_edge = false, arg_init = false;
    if(argc<7 || argc>11){
        cout << "Wrong arguments!" << endl;
        return -1;
    }
    for(int i = 1;i<argc;i++){
        if(!strcmp(argv[i],"-i")){
            if(argv[i+1] == nullptr){
                cout << "No point set input file given!" << endl;
                return -1;
            }
            vect = readPoints(argv[i+1]);
            area_ch = readArea(argv[i+1]);
            arg_i = true;
        }else if(!strcmp(argv[i],"-o")){
            if(argv[i+1] == nullptr){
                cout << "No output file given!" << endl;
                return -1;
            }
            outputFile = argv[i+1];
            arg_o = true;
        }else if(!strcmp(argv[i],"-algorithm")){
            if(argv[i+1] == nullptr){
                cout << "No algorithm given!" << endl;
                return -1;
            }
            if(!strcmp(argv[i+1],"incremental")) flag_algorithm = incremental;
            else if(!strcmp(argv[i+1],"convex_hull")) flag_algorithm = convex_hull;
            else if(!strcmp(argv[i+1],"onion")){
                cout << "In this project we have implemented only incremental and convex_hull!" << endl;
                return -1;
            }else{
                cout << "Wrong input on -algorithm !" << endl;
                return -1;
            }
            arg_alg = true;
        }else if(!strcmp(argv[i],"-edge_selection")){
            if(argv[i+1] == nullptr){
                cout << "No algorithm given!" << endl;
                return -1;
            }
            if(!strcmp(argv[i+1],"1")) flag_visible = random_vis;
            else if(!strcmp(argv[i+1],"2")) flag_visible = min_vis;
            else if(!strcmp(argv[i+1],"3")) flag_visible = max_vis;
            else{
                cout << "Wrong input on -edge_selection !" << endl;
                return -1;
            }
            arg_edge = true;            
        }else if(!strcmp(argv[i],"-initialization")){
            if(argv[i+1] == nullptr){
                cout << "No algorithm given!" << endl;
                return -1;
            }
            if(!strcmp(argv[i+1],"1a")) initialization = 1;
            else if(!strcmp(argv[i+1],"1b")) initialization = 2;
            else if(!strcmp(argv[i+1],"2a")) initialization = 3;
            else if(!strcmp(argv[i+1],"2b")) initialization = 4;  
            else{
                cout << "Wrong input on -initialization !" << endl;
                return -1;
            }
            arg_init = true;
        }
        else if(!strcmp(argv[i],"-onion_initialization")){
            cout << "Onion algorithm has not been implemented !" << endl;
            return -1;
        }
    }

    if(!arg_i || !arg_o || !arg_alg || !arg_edge){
        cout << "Wrong arguments !" << endl;
        return -1;
    }

    if(flag_algorithm == incremental){
        if(!arg_init){
            cout << "-initialization argument is mandatory for the Incremental algorithm!" << endl;
            return -1;
        }
    }else{
        if(arg_init){
            cout << "-initialization argument is only for the Incremental algorithm!" << endl;
            return -1;
        }
    }

    // Clock starts now
    auto start = chrono::high_resolution_clock::now();

    Polygon_2 polygonChain;
    if(flag_algorithm == incremental){
        polygonChain = incremental_alg(vect, initialization, flag_visible);
    }else if(flag_algorithm == convex_hull){
        polygonChain = convex_hull_alg(vect,flag_visible);
    }
// End of algorithm
auto stop = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);

ofstream outfile (outputFile);

outfile << "Polygonization" << endl;
for (veciterator it = vect.begin(); it != vect.end(); ++it){
    outfile << *it << endl;
}
for(auto it = polygonChain.edges_begin(); it != polygonChain.edges_end(); ++it){
    outfile << *it << endl;
}
outfile << "Algorithm: ";
if(flag_algorithm == incremental) outfile << "incremental_";
else if(flag_algorithm == convex_hull) outfile << "convex_hull_";
outfile << "edge_selection" << flag_visible+1;
if(flag_algorithm != convex_hull){
    outfile << "_";
    outfile << "initialization";
    switch(initialization){
        case 1:
            outfile << "1a" << endl;
            break;
        case 2:
            outfile << "1b" << endl;
            break;
        case 3:
            outfile << "2a" << endl;
            break;
        case 4:
            outfile << "2b" << endl;
            break;
    }
}else{outfile << endl;}

outfile << "area: " << abs(polygonChain.area()) << endl;
outfile << "ratio: " << ((double) abs(polygonChain.area()))/((double) area_ch)  << endl;
outfile << "construction time: " << duration.count() << endl;

outfile.close();

return 0;
}
