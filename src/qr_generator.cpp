/* qr_generator.cpp
   Ryan Stewart
   9/25/21

   This program allows a user to create a QR code using a the QRCodeGen library which can be found
   here: https://github.com/nayuki/QR-Code-generator.
*/

#include <iostream>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/qrcodegencpp/qrcodegen.hpp"
using namespace std;
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

void new_qr_code(string fileName, string text);
string toSvgString(const QrCode &qr, int border);
int help();

int main(int argc, char **argv)
{
  if(argc != 3) return help();

  string fn(argv[1]);
  string fc(argv[2]);

  new_qr_code(fn, fc);

  return 0;
}

// create a new qr code and returns the path of the newly created qr code
void new_qr_code(string fileName, string text)
{
  const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

  // Make and print the QR Code symbol
  const QrCode qr = QrCode::encodeText(text.c_str(), errCorLvl);
  auto svgStr = toSvgString(qr, 4);

  string filename(fileName);
  string fn = filename + ".svg";

  // write qr code to .svg file
  ofstream fout;
  fout.open(fn);
  fout << svgStr;
  fout.close();
  
  // todo: check if files exist
  string command = "convert ./" + fn + " ./" + filename + ".ppm"; 
  system(command.c_str());
  command = "rm ./" + fn;
  system(command.c_str());
}

int help()
{
  cout << "Usage: $ ./qr_generator [filename] [filecontent]\n\nExample: $ ./qr_generator helloworld_QrCode \"Hello, World!\"\n\n";
  return 1;
}

// ** NOTE: function taken from qrcodegen demo: https://github.com/nayuki/QR-Code-generator/blob/master/cpp/QrCodeGeneratorDemo.cpp **

// Returns a string of SVG code for an image depicting the given QR Code, with the given number
// of border modules. The string always uses Unix newlines (\n), regardless of the platform.
string toSvgString(const QrCode &qr, int border) {
  if (border < 0)
	throw domain_error("Border must be non-negative");
  if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
	throw overflow_error("Border too large");

  ostringstream sb;
  sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
  sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
  sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
  sb << "\t<path d=\"";
  for (int y = 0; y < qr.getSize(); y++) {
	for (int x = 0; x < qr.getSize(); x++) {
	  if (qr.getModule(x, y)) {
		if (x != 0 || y != 0)
		  sb << " ";
		sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
	  }
	}
  }
  sb << "\" fill=\"#000000\"/>\n";
  sb << "</svg>\n";
  return sb.str();
}
