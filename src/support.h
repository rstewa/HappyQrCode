/* 
 * Support.h
 * Author: Ryan Stewart
 * Date: 10/3/19
 * Description: This header file provides the matrix, ppm, and rnumgen classes. It also has the
 * pixel and rgb structs.
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

typedef unsigned char uchar;
extern const char ETX;

//Matrix class taken from matrix_handout.pdf
template <typename T>
class matrix { 
  //use matrix handout version 3
  public:
    matrix(int n_Ncols, int n_Nrows){
      Nrows = n_Nrows;//
      Ncols = n_Ncols;//

      buf = new T * [Ncols]; //***might be wrong***//
      for(int i=0; i<Ncols; i++){
        buf[i] = new T [Nrows];
      }
    }

    ~matrix(){ 
      for(int i=0; i<Ncols; i++){
        delete [] buf[i];
      }
      delete [] buf;
    }

    int get_Nrows() const { return Nrows; }
    int get_Ncols() const { return Ncols; }

    T * operator[] (int i) { return buf[i]; }

  private:
    int Nrows, Ncols;
    T **buf;
};

//stores x and y cord for a pixel
struct pixel { 
  public:
    pixel(int xx=0, int yy=0){
      x = xx;
      y = yy;
    }
    int x,y;
};

//these hold the color values for a pixel
struct RGB { 
  RGB(uchar rr=0, uchar gg=0, uchar bb=0, bool _marker=0){
    r=rr;
    g=gg;
    b=bb;
    marker=_marker;
  }
  uchar r, g, b;
  bool marker;
};

//this is the class that processes and stores the image 
//it reads in the image and writes the image to a new p6 ppm file
class ppm { 
  public:
    ppm(){ image = NULL; }
    void read(const char*);
    void write(const char*);

    int get_Nrows() const{ 
      if(image != NULL){ return image->get_Nrows(); }
      return 0;
    }
    int get_Ncols() const{ 
      if(image != NULL){ return image->get_Ncols(); }
      return 0;
    }

    RGB * operator[] (int index) {
      return (*image)[index];                       
    }

  private:
    matrix<RGB> *image;
    void image_creation(int w, int h){
      if(image != NULL){ delete image; }
      image = new matrix<RGB>(w, h); //changed it here//
    }
};

//this class was taken from randomnumber_handout.pdf
class rnumgen{
  public:
    rnumgen(int, std::vector<int> &);
    int rand();

  private:
    std::vector<float> F;
};

#endif
