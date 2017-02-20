/*
 *  File_Format.h
 *
 *  created on  :   25/08/2016
 *      author  :   sajith
 *
 */
#include "commonDefs.h"

class File_Format
{
    public:
        /*constructor*/
        File_Format(void);

        FILE * fp;
        int remain_bits;
        uint8_t tmp_b[1];
        queue<uint8_t> pkt_q;
        int DWT_r;

        /*functions*/
        int JP2_Signature_box(void);
        int Profile_box(void);
        int JP2_Header_box(img_hdr_info *hdr_info,int bcp);
        int Img_header_box(img_hdr_info *hdr_info,int bpc);
        int Colour_Spec_box(img_hdr_info *hdr_info);
        int Code_Stream_box(queue<int> *hdr_q_r,queue<uint8_t> *code_stream_q_r,queue<int> *hdr_q_g,queue<uint8_t> *code_stream_q_g,queue<int> *hdr_q_b,queue<uint8_t> *code_stream_q_b,queue<pktParamfnl> *qnt_q,img_hdr_info *hdr_info);

        int SOC(void);
        int SIZ(img_hdr_info *hdr_info, int bcp,int s1,int s2);
        int COD(int op, int delta_t, int D_t, int E1_cb, int E2_cb);
        int QCD(queue<pktParamfnl> *qnt_q);
        int SOT(int tile_index,int tile_part_index,int tile_part_length,int no_of_tile_parts);
        int SOD(void);

        int EOC(void);

        int packet(int pkt_index,queue<int> * hdr_q, queue<uint8_t> * code_stream_q,img_hdr_info *hdr_info);
        int make_packet(int pkt_index,queue<int> * hdr_q, queue<uint8_t> * code_stream_q,img_hdr_info *hdr_info);
        int write_packet(int length);
        int code_block(int length, queue<uint8_t> * code_stream_q);
        int send_no_layers(int no_of_layers);
        int send_no_of_coding_passes(int no_of_coding_passes);
        int send_no_of_zero_bitplanes(int zero_bit_planes);
        int send_length( int length,int bits);
        int insert_value(int bit_cntr, int value);
        int insert_zeros(int number_of_zeros);
        int push_q_value(int bit_cntr, int value);
        int push_q_zeros(int number_of_zeros);
        int push_q_bytes(int byte_cnt, uint32_t value);
        int send_Lblock(int length , int no_of_coding_passes);
        int push_bytes(int byte_cnt, uint32_t value);
        int run(queue<int> *hdr_q_r,queue<uint8_t> *code_stream_q_r,queue<int> *hdr_q_g,queue<uint8_t> *code_stream_q_g,queue<int> *hdr_q_b,queue<uint8_t> *code_stream_q_b,queue<pktParamfnl> *qnt_q,img_hdr_info *hdr_info);
};
