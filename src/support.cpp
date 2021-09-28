/*
 * Support.cpp
 * Author: Ryan Stewart
 * Date: 10/3/19
 * Description: This source file includes the function defintions from the ppm class and the rnumgen class
*/

#include "support.h"
#include <utility>

using namespace std;

const char ETX = 0x3;

// vector<pair<int,int>> ppm::
// bool ppm::_match(pair<int,int> xy) {
// 
// }

//PPM functions//
void ppm::read(const char* p6_file){ 
    ifstream fin;
    string p6;
    int depth, w, h;

    fin.open(p6_file);
    
    //error opening file check 
    if(!fin.is_open()){ cerr << "Error: cannot open " << p6_file << "\n"; exit(0); }

    fin >> p6;

    //header check
    if(p6 != "P6"){ cerr << "Error: file not in P6 format\n"; exit(0); }
    
    //store width, height, and max values 
    if(!(fin >> w >> h >> depth)){
      cerr << "Error\n";
    }

    //max value check
    if(depth != 255){ cerr << "Error: max value/depth is too large\n"; exit(0); }
    
    //skip the annoying newline 
    fin.get();

    //create new image
    image_creation(w,h);

    // reads in all the pixels using the RGB struct 
    for(int j=0; j<h; j++){
        for(int i=0; i<w; i++){
            RGB &pixl = (*image)[i][j];
            pixl.r = fin.get();
            pixl.g = fin.get();
            pixl.b = fin.get();
        }
    }
    
    // int pxl_ct = 0, marker_ct = 0;
    // int lpos, rpos;
    // for(int j=0; j<h; j++){
    //     for(int i=0; i<w; i++){
    //         RGB &pxl = (*image)[i][j];
    //         pxl.r = fin.get();
    //         pxl.g = fin.get();
    //         pxl.b = fin.get();
    //         if((int)pxl.r == 0 && (int)pxl.g == 0 && (int)pxl.b == 0) {
    //           pxl_ct++;
    //           lpos = pxl_ct==1 ? i : lpos;
    //           if(pxl_ct == 7) {
    //             rpos = i;
    //             for(int k=lpos; k<=rpos; k++)
    //             {
    //               ((*image)[k][j]).marker = true;
    //               ((*image)[j][k]).marker = true;
    //             }
    //             pxl_ct = 0;
    //             marker_ct++;
    //             if(marker_ct == 2) {
    //               cout << "\nmarker_ct == 2\n\tlpos: " << lpos << ", rpos: " << rpos << endl;
    //               // save location bc this is the side w 2 markers
    //             }
    //           }
    //         }
    //         else {
    //           pxl_ct = 0;
    //         }
    //         if(i==(w-1) && marker_ct == 1) {
    //           cout << "\nmarker_ct == 1\n\tj: " << j << ", i: " << i << endl;
    //           // this is a side w 1 marker
    //         }
    //     }
    //     marker_ct = 0;
    // }

    //close file after use
    fin.close();
}

void ppm::write(const char* p6_file){ 
    ofstream fout;
    
    //open file
    fout.open(p6_file);
  
    //error check to see if file was able to be opened
    if(!fout.is_open()){ cerr << "error: file could not be opened\n"; }

    //add header for P6 format
    fout << "P6\n" << get_Ncols() << " " << get_Nrows() << "\n255\n";
    
    //uses width and height of ppm file to rewrite pixels into new p6 ppm file
    for(int j=0; j<get_Nrows(); j++){
        for(int i=0; i<get_Ncols(); i++){
            RGB &pixl = (*image)[i][j];
            fout << pixl.r << pixl.g << pixl.b;
        }
    }
    fout.close();
}
//RNUMGEN functions//
//taken from randomnumber_handout.pdf
rnumgen::rnumgen(int seedvalue, std::vector<int> &v){
  srand(seedvalue);

  F.resize(v.size());
  partial_sum(v.begin(), v.end(), F.begin());
  transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), F.back()));
}

//taken from randomnumber_handout.pdf
int rnumgen::rand(){
  const double randmax = RAND_MAX+1.0;
  const double p = (double)std::rand()/randmax;

  return upper_bound(F.begin(), F.end(), p) - F.begin();
}
