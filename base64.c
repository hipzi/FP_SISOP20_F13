#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

static void encodeblock( unsigned char in[], unsigned char out[], int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

void base64_encode( unsigned char infile[],unsigned long insize, unsigned char outfile[], int linesize )
{
	unsigned char in[3]={0};
	unsigned char out[4]={0};

        int i,len, cin = 0,cout = 0, blocksout = 0;

	*in = (unsigned char) 0;
	*out = (unsigned char) 0;
        
        while( cin < insize ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char)  infile[cin++];

            if( cin <= insize ) {
                len++;
            }
            else {
                in[i] = (unsigned char) 0;
            }
        }
        if( len > 0 ) {
            encodeblock( in, out, len );
	    memmove(&outfile[cout],out,4);
            cout+=4;
            blocksout++;
        }
	if( blocksout >= (linesize/4)) {
            if( blocksout > 0 ) {
                memmove( &outfile[cout], "\r\n", 2 );
                cout+=2;
            }
            blocksout = 0;
        }
	if( cin >= insize ) {
            if( blocksout > 0 ) {
                memmove( &outfile[cout], "\n", 1 );
                cout+=1;
            }
            blocksout = 0;
        }
    }
}

static void decodeblock( unsigned char in[], unsigned char out[] )
{   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

void base64_decode( unsigned char infile[],unsigned long insize, unsigned char outfile[] )
{
    unsigned char in[4]={0};
    unsigned char out[3]={0};
    
    int v;
    int i, len, cin =0, cout =0;

    while( cin < insize ) {
        for( len = 0, i = 0; i < 4 && cin < insize; i++ ) {
            v = 0;
            while( cin < insize && v == 0 ) {
                v = infile[cin++];
                if( v != 0 ) {
	                v = ((v < 43 || v > 122) ? 0 : (int) cd64[ v - 43 ]);
					if( v != 0 ) {
						v = ((v == (int)'$') ? 0 : v - 61);
					}
                }
            }
            if( cin < insize ) {
                len++;
                if( v != 0 ) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in[i] = (unsigned char) 0;
            }
        }
        if( len > 0 ) {
            decodeblock( in, out );
            memmove(&outfile[cout],out,3);
            cout+=3;
        }
    }
}

int main(int argc, char **argv) {
  unsigned char hasil[1024];
  int file;

  if(argc == 2){	
  if ((file = open(argv[1],O_RDONLY)) < 0){
        printf(1,"File %s tidak ada\n", argv[1]);
    	close(file);
	exit();
  }
  int n;
  unsigned char temp[1024];
  while ((n = read(file,temp,sizeof(temp))) > 0);
  base64_encode(temp, strlen((char*)temp), hasil, 76);
  printf(1,"%s",hasil);
  close(file);
  exit();
 }
  if(argv[1][0] == '-' && argv[1][1] == 'd'){
	if ((file = open(argv[2],O_RDONLY)) < 0){
        printf(1,"File %s tidak ada\n", argv[2]);
        close(file);
        exit();
  }
  int n;
  unsigned char temp[1024];
  while ((n = read(file,temp,sizeof(temp))) > 0);
  base64_decode(temp, strlen((char*)temp), hasil);
  printf(1,"%s",hasil);
  close(file);
  exit();
  }
}
