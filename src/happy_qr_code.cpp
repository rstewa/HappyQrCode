/* happy_qr_code.cpp
   Ryan Stewart
   9/25/21

   This program reads in a qr code image in ppm format and provides
   a user ways to modify its appearance.
*/

#include <iostream>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include "support.h"
using namespace std;

int usage();
void process_qrcode(char *fileName, char *output_fn, string arg = "none", string val = "none");
int get_version(int cols);
bool is_color_too_light(int r, int g, int b);

int main(int argc, char **argv)
{
  vector<string> options{"-o", "-c", "-p", "-r", "-h", "--help"};
  srand(time(NULL)); // seed psuedo-random number generator

  // args checks
  if (argc < 3 || argc > 5)
    return usage();

  if (argc > 3)
  {
    string option(argv[3]);
    if (find(options.begin(), options.end(), option) == options.end() || option == "-h" || option == "--help")
    {
      return usage();
    }

    if (option == "-c" || option == "-p")
    {
      string val(argv[4]);
      process_qrcode(argv[1], argv[2], option, val);
    }
    else
      process_qrcode(argv[1], argv[2], option);
  }
  else
  {
    process_qrcode(argv[1], argv[2]);
  }

  return 0;
}

// function based on answer from here: https://stackoverflow.com/questions/5477702/how-to-see-if-a-rgb-color-is-too-light
bool is_color_too_light(int r, int g, int b) // {{{
{
  auto yiq = ((r * 299) + (g * 587) + (b * 114)) / 1000;
  return yiq >= 128;
}

void process_qrcode(char *fileName, char *output_fn, string arg, string val)
{
  char *ptr;
  string fn2;
  double rc, gc, bc;

  // read in the ppm image using my support.cpp helper class
  ppm img;
  img.read(fileName);

  auto rows = img.get_Nrows();
  auto cols = img.get_Ncols();

  string fn(output_fn);
  fn += ".jgr";

  FILE *fout;
  fout = fopen(fn.c_str(), "w");

  // creating the .jgr file for jgraph to use
  fprintf(fout, "newgraph\nxaxis min 0 max %d nodraw\nyaxis min -%d max 0 nodraw\n\n", cols, rows);

  // get rgb values if user specified '-c'
  if (arg == "-c")
  {
    // tokenize the value to get the rgb values
    char *str = &val[0];

    ptr = strtok(str, " ");
    rc = atof(ptr);
    if (rc > 1.0)
      rc /= 255;

    ptr = strtok(NULL, " ");
    gc = atof(ptr);
    if (gc > 1.0)
      gc /= 255;

    ptr = strtok(NULL, " ");
    bc = atof(ptr);
    if (bc > 1.0)
      bc /= 255;
  }
  else if (arg == "-p")
  {
    // convert the image to .eps and resize it
    fn2 = val + ".eps";
    string cmd = "convert " + val + " -resize 64x64 " + fn2;
    system(cmd.c_str());
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      RGB &pxl = img[j][i];
      if ((int)pxl.r == 0)
      {
        // random color option for squares in the qr code
        if (arg == "-r")
        {
          int r = rand() % 255, g = rand() % 255, b = rand() % 255;
          while (is_color_too_light(r, g, b))
          {
            r = rand() % 255;
            g = rand() % 255;
            b = rand() % 255;
          }
          float rr = r / 255.0, gg = g / 255.0, bb = b / 255.0;
          fprintf(fout, "newcurve marktype box marksize 0.93 0.93 cfill %f %f %f color %f %f %f\npts %d.0 -%d.0\n\n", rr, gg, bb, rr, gg, bb, j, i);
        }
        // specific color option
        else if (arg == "-c")
        {
          fprintf(fout, "newcurve marktype box marksize 0.93 0.93 cfill %f %f %f color %f %f %f\npts %d.0 -%d.0\n\n", rc, gc, bc, rc, gc, bc, j, i);
        }
        // option to use a picture for the squares of the QR code
        else if (arg == "-p")
        {
          fprintf(fout, "newcurve eps %s marksize 1 1\npts %d.0 -%d.0\n\n", fn2.c_str(), j, i);
        }
        // normal
        else
        {
          fprintf(fout, "newcurve marktype box marksize 0.93 0.93 color 0 0 0 cfill 0 0 0\npts %d.0 -%d.0\n\n", j, i);
        }
      }
    }
  }
  fclose(fout);

  string command = "sh jgraph2jpg.sh " + fn;
  system(command.c_str());
}

int get_version(int cols)
{
  return (cols - 17) / 4;
}

int usage()
{
  printf("usage ./happy_qr_code [qrcode.ppm] [output-file-name] [option] [arg] ...\noptions and arguments:\n-c               : input an rgb value to use to change the QR code color to (i.e. \"1 0 0\")\n-p [image-file]  : uses a jpeg or png picture for the squares of the qr code\n-r               : will use random colors for the qr code\n-h | --help      : list the usage and options\n");
  return 1;
}
