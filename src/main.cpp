#include <iostream>
#include "commonDefs.h"
#include "Quantizer.h"
#include "Wavelet2D.h"
#include "EbcotCoder.h"
#include "MQcoder.h" 
#include "File_Format.h"
using namespace std;
//const char *inputtxt = "image pool/image0.txt";
image imcreate();
int main()
{
    Wavelet2D *wavelet = new Wavelet2D();
    Quantizer *quantizer = new Quantizer();
    EbcotCoder *ebcotcoder = new EbcotCoder();
    MQcoder *mqcoder = new MQcoder();
    File_Format *format = new File_Format();

    int pixel_l = sbsize*2;
    int pixel_w = sbsize*2;
    int levels = 1;
    setPkt pkt;

    pkt.pixel_l = pixel_l;
    pkt.pixel_w = pixel_w;
    pkt.levels = levels;
    queue<setPkt> queue1;
    queue1.push(pkt);
    queue<long> queue2;
    queue<coeffPkt> queue3;
    queue<sbandPkt> LL;
    queue<sbandPkt> HL;
    queue<sbandPkt> LH;
    queue<sbandPkt> HH;
    coeffPkt Cpkt;
    queue<context> CONTEXT;
    queue<int> queue4;
    queue<uint8_t> queue5;
    queue<pktParamBPC> param2BPC;
    queue<pktParamfnl> param2fnl;
    img_hdr_info *hdr_info_ptr;

    ////////////////////////////////////////////////////////// open image //////////////////////////////////////////////////////////////
    FILE* f = fopen("image2.bmp","r");
    if(f!=NULL){
        cout << "succussfully open file" << endl;
    }
    else {
        cout << "error while opening the file" << endl;
    }

    unsigned char info[54];
    fread(info,sizeof(unsigned char),54,f);            // read the 54 byte header

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    uint16_t bit_per_px = *(int*)&info[28];


    printf("width = %d\n",width);
    printf("height = %d\n",height);
    printf("bit_per_px = %d\n",bit_per_px );

    int no_of_color_planes = bit_per_px/8;


    img_hdr_info hdr_info;

    hdr_info.width = width;
    hdr_info.height = height;
    hdr_info.no_of_cmp = no_of_color_planes;



    int size = no_of_color_planes * width * height;

    unsigned char *data = new unsigned char[size];   // allocate 3 byte per pixel


    int t =fread(data,sizeof(unsigned char),size,f);        //read the rest ot data at once
    fclose(f);

    printf("read pixels = %d\n",t);

    if (no_of_color_planes == 1)
    {
        for (int i=0; i<size; i++){

            queue2.push(long(data[i] -128));

        }
        
        wavelet->run(&queue2, &queue3, &queue1);
        quantizer->run(&queue3,&LL, &HL, &LH, &HH, &param2BPC, &param2fnl);
        ebcotcoder->run(&LL, &LH, &HL, &HH, &CONTEXT, &param2BPC); // LL LH HL HH
        mqcoder->run(&CONTEXT, &queue4, &queue5);
        format->run(&queue4, &queue5, &param2fnl,&hdr_info);
        
    }
    else if(no_of_color_planes == 3)
    {

    	printf("no 0f com = 3\n");

        for (int i = 0; i < size; i += 3)
        {
            unsigned char tmp = data[i];
            data[i]           = data[i+2];
            data[i+2]         = tmp;
        }

        for (int i=0; i<size; i++){                                                       ////////////////   R

            queue2.push(long(data[i] -128));
        }
        wavelet->run(&queue2, &queue3, &queue1);
        quantizer->run(&queue3,&LL, &HL, &LH, &HH, &param2BPC, &param2fnl);
        ebcotcoder->run(&LL, &LH, &HL, &HH, &CONTEXT, &param2BPC); // LL LH HL HH
        mqcoder->run(&CONTEXT, &queue4, &queue5);

        for (int i=0; i<size; i++){                                                     //////////////////// G

            queue2.push(long(data[i+1] -128));
        }
        wavelet->run(&queue2, &queue3, &queue1);
        quantizer->run(&queue3,&LL, &HL, &LH, &HH, &param2BPC, &param2fnl);
        ebcotcoder->run(&LL, &LH, &HL, &HH, &CONTEXT, &param2BPC); // LL LH HL HH
        mqcoder->run(&CONTEXT, &queue4, &queue5);

        for (int i=0; i<size; i++){												      ////////////////////// B

            queue2.push(long(data[i+2] -128));
        }
        wavelet->run(&queue2, &queue3, &queue1);
        quantizer->run(&queue3,&LL, &HL, &LH, &HH, &param2BPC, &param2fnl);
        ebcotcoder->run(&LL, &LH, &HL, &HH, &CONTEXT, &param2BPC); // LL LH HL HH
        mqcoder->run(&CONTEXT, &queue4, &queue5);

        format->run(&queue4, &queue5, &param2fnl,&hdr_info);
   
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


