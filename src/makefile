CC = g++
CFLAGS = -g -std=c++11
BIN =  qr_generator happy_qr_code

all: $(BIN) 

qr_generator: qr_generator.cpp 
	$(CC) $(CFLAGS) -o $@ qr_generator.cpp -L../include/qrcodegencpp -lqrcodegencpp
	./qr_generator large_qrcode "What is Lorem Ipsum? Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged."

happy_qr_code: happy_qr_code.cpp support.h support.o
	$(CC) $(CFLAGS) -o $@ happy_qr_code.cpp support.cpp
	rm -rf ./OUTPUT
	mkdir ./OUTPUT
	./happy_qr_code large_qrcode.ppm ./OUTPUT/normal &
	./happy_qr_code large_qrcode.ppm ./OUTPUT/red_color_qrcode -c "1 0 0" &
	./happy_qr_code large_qrcode.ppm ./OUTPUT/random_color_qrcode -r &
	./happy_qr_code large_qrcode.ppm ./OUTPUT/puppy_qrcode -p puppy.png
	rm -f ./OUTPUT/*.jgr
	echo "Happpy QR Code samples can be found in the 'OUTPUT' directory" & clear

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(BIN) *.o *.jgr *.pdf *.eps small_qrcode* large_qrcode* *.jpg
