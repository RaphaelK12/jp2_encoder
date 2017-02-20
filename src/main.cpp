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
    Wavelet2D *wavelet_r = new Wavelet2D();
    Wavelet2D *wavelet_g = new Wavelet2D();
    Wavelet2D *wavelet_b = new Wavelet2D();

    Quantizer *quantizer_r = new Quantizer();
    Quantizer *quantizer_g = new Quantizer();
    Quantizer *quantizer_b = new Quantizer();

    EbcotCoder *ebcotcoder_r = new EbcotCoder();
    EbcotCoder *ebcotcoder_g = new EbcotCoder();
    EbcotCoder *ebcotcoder_b = new EbcotCoder();

    MQcoder *mqcoder_r = new MQcoder();
    MQcoder *mqcoder_g = new MQcoder();
    MQcoder *mqcoder_b = new MQcoder();

    File_Format *format = new File_Format();

    int pixel_l = sbsize*2;
    int pixel_w = sbsize*2;
    int levels = 1;
    setPkt pkt;

    pkt.pixel_l = pixel_l;
    pkt.pixel_w = pixel_w;
    pkt.levels = levels;

    img_hdr_info *hdr_info_ptr;
    queue<pktParamfnl> param2fnl;

    queue<setPkt> queue1_r,queue1_g,queue1_b;


    queue1_r.push(pkt);
    queue1_g.push(pkt);
    queue1_b.push(pkt);


    queue<long> queue2_r,queue2_g,queue2_b;
    queue<coeffPkt> queue3_r,queue3_g,queue3_b;
    queue<sbandPkt> LL_r,LL_g,LL_b;
    queue<sbandPkt> HL_r,HL_g,HL_b;
    queue<sbandPkt> LH_r,LH_g,LH_b;
    queue<sbandPkt> HH_r,HH_g,HH_b;
    coeffPkt Cpkt[3];
    queue<context> CONTEXT_r,CONTEXT_g,CONTEXT_b;
    queue<int> queue4_r,queue4_g,queue4_b;
    queue<uint8_t> queue5_r,queue5_g,queue5_b;
    queue<pktParamBPC> param2BPC_r,param2BPC_g,param2BPC_b;




    ////////////////////////////////////////////////////////// open image //////////////////////////////////////////////////////////////
    FILE* f = fopen("bmwGray1024.bmp","r");
    if(f!=NULL){
        cout << "succussfully open file" << endl;
    }
    else {
        cout << "error while opening the file" << endl;

        return -1;
    }

    unsigned char info[54];
    fread(info,sizeof(unsigned char),54,f);            // read the 54 byte header

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    uint16_t bit_per_px = *(int*)&info[28];

    int t_x = ceil(width/128),t_y = ceil(height/128);
    printf("width = %d\n",width);
    printf("height = %d\n",height);
    printf("bit_per_px = %d\n",bit_per_px );

    int no_of_color_planes = bit_per_px/8;


    img_hdr_info hdr_info;

    hdr_info.width = width;
    hdr_info.height = height;
    hdr_info.no_of_cmp = no_of_color_planes;
    hdr_info.no_of_tiles = t_y *t_x;

    printf("no_of_tiles = %d\n", hdr_info.no_of_tiles);

    int size = no_of_color_planes * width * height;

    unsigned char *data = new unsigned char[size];   // allocate 3 byte per pixel
    int t =fread(data,sizeof(unsigned char),size,f);        //read the rest ot data at once
    fclose(f);

    printf("read pixels = %d\n",t);


    if (no_of_color_planes == 1)
    {
        for (int x = 0; x < t_x; ++x)
        {
            for (int y = t_y-1; y >=0; --y)
            {
                Wavelet2D *wavelet_r = new Wavelet2D();
                Wavelet2D *wavelet_g = new Wavelet2D();
                Wavelet2D *wavelet_b = new Wavelet2D();
            
                Quantizer *quantizer_r = new Quantizer();
                Quantizer *quantizer_g = new Quantizer();
                Quantizer *quantizer_b = new Quantizer();
            
                EbcotCoder *ebcotcoder_r = new EbcotCoder();
                EbcotCoder *ebcotcoder_g = new EbcotCoder();
                EbcotCoder *ebcotcoder_b = new EbcotCoder();
            
                MQcoder *mqcoder_r = new MQcoder();
                MQcoder *mqcoder_g = new MQcoder();
                MQcoder *mqcoder_b = new MQcoder();
                
                for (int i = 0; i < tile_width; ++i)
                {
                    for (int j = (tile_height-1); j >= 0; --j)
                    {
                        int index = (y*tile_height+j)*width + (x*tile_width+i);
                        queue2_r.push(long(data[index] -128)); 

                        //printf("%d \n", );  
                    }
                }

                wavelet_r->run(&queue2_r, &queue3_r, &queue1_r);
                quantizer_r->run(&queue3_r,&LL_r, &HL_r, &LH_r, &HH_r, &param2BPC_r, &param2fnl);
                ebcotcoder_r->run(&LL_r, &LH_r, &HL_r, &HH_r, &CONTEXT_r, &param2BPC_r); // LL LH HL HH
                mqcoder_r->run(&CONTEXT_r, &queue4_r, &queue5_r);
            } 
        }

        format->run(&queue4_r, &queue5_r,&queue4_g, &queue5_g,&queue4_b, &queue5_b, &param2fnl,&hdr_info);
        
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

        for (int y = 0; y < t_y; ++y)
        {
            for (int x = 0; x < t_x; ++x)
            {
                Wavelet2D *wavelet_r = new Wavelet2D();
                Wavelet2D *wavelet_g = new Wavelet2D();
                Wavelet2D *wavelet_b = new Wavelet2D();
            
                Quantizer *quantizer_r = new Quantizer();
                Quantizer *quantizer_g = new Quantizer();
                Quantizer *quantizer_b = new Quantizer();
            
                EbcotCoder *ebcotcoder_r = new EbcotCoder();
                EbcotCoder *ebcotcoder_g = new EbcotCoder();
                EbcotCoder *ebcotcoder_b = new EbcotCoder();
            
                MQcoder *mqcoder_r = new MQcoder();
                MQcoder *mqcoder_g = new MQcoder();
                MQcoder *mqcoder_b = new MQcoder();
                

                for (int i = 0; i < 3*tile_width; i+=3)
                {
                    for (int j = 3*(tile_height-1); j >= 0; j-=3)
                    {
                        queue2_r.push(long(data[(j)*width + i] -128));   
                    }
                }
                wavelet_r->run(&queue2_r, &queue3_r, &queue1_r);
                quantizer_r->run(&queue3_r,&LL_r, &HL_r, &LH_r, &HH_r, &param2BPC_r, &param2fnl);
                ebcotcoder_r->run(&LL_r, &LH_r, &HL_r, &HH_r, &CONTEXT_r, &param2BPC_r); // LL LH HL HH
                mqcoder_r->run(&CONTEXT_r, &queue4_r, &queue5_r);
        
                 for (int i = 0; i < 3*tile_width; i+=3)
                 {
                     for (int j = 3*(tile_height-1); j >= 0; j-=3)
                    {
                        queue2_g.push(long(data[j*width + i +1] -128));   
                    }
                }
                wavelet_g->run(&queue2_g, &queue3_g, &queue1_g);
                quantizer_g->run(&queue3_g,&LL_g, &HL_g, &LH_g, &HH_g, &param2BPC_g, &param2fnl);
                ebcotcoder_g->run(&LL_g, &LH_g, &HL_g, &HH_g, &CONTEXT_g, &param2BPC_g); // LL LH HL HH
                mqcoder_g->run(&CONTEXT_g, &queue4_g, &queue5_g);
        
                 for (int i = 0; i < 3*tile_width; i+=3)
                 {
                     for (int j = 3*(tile_height-1); j >= 0; j-=3)
                    {
                        queue2_b.push(long(data[j*width + i +2] -128));   
                    }
                }
                wavelet_b->run(&queue2_b, &queue3_b, &queue1_b);
                quantizer_b->run(&queue3_b,&LL_b, &HL_b, &LH_b, &HH_b, &param2BPC_b, &param2fnl);
                ebcotcoder_b->run(&LL_b, &LH_b, &HL_b, &HH_b, &CONTEXT_b, &param2BPC_b); // LL LH HL HH
                mqcoder_b->run(&CONTEXT_b, &queue4_b, &queue5_b);
            }
        }

        format->run(&queue4_r, &queue5_r,&queue4_g, &queue5_g,&queue4_b, &queue5_b, &param2fnl,&hdr_info);

    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


