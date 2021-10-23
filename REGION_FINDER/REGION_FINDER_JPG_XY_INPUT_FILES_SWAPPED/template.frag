//---------------------------------------------------------------------------
// Fragment
//---------------------------------------------------------------------------
//#version 420 core
#version 130
//---------------------------------------------------------------------------
uniform sampler2D texture;
in vec2 pos;                    // screen position <-1,+1>
out vec4 gl_FragColor;          // fragment output color
vec4 col;                       // color interface for txt_print()

vec2 iRes_texture=vec2(textureSize( texture,0 ));
uniform vec2 iResolution;

/*
uniform sampler2D txr_font;     // ASCII 32x8 characters font texture unit
uniform float fxs,fys;          // font/screen resolution ratio
//---------------------------------------------------------------------------
const int _txtsiz=32;           // text buffer size
int txt[_txtsiz],txtsiz;        // text buffer and its actual size
//---------------------------------------------------------------------------
void txt_decimal(float x)       // print float x into txt
    {
    int i,j,c;          // l is size of string
    float y,a;
    const float base=10;
    // handle sign
    if (x<0.0) { txt[txtsiz]='-'; txtsiz++; x=-x; }
     else      { txt[txtsiz]='+'; txtsiz++; }
    // divide to int(x).fract(y) parts of number
    y=x; x=floor(x); y-=x;
    // handle integer part
    i=txtsiz;                   // start of integer part
    for (;txtsiz<_txtsiz;)
        {
        a=x;
        x=floor(x/base);
        a-=base*x;
        txt[txtsiz]=int(a)+'0'; txtsiz++;
        if (x<=0.0) break;
        }
    j=txtsiz-1;                 // end of integer part
    for (;i<j;i++,j--)      // reverse integer digits
        {
        c=txt[i]; txt[i]=txt[j]; txt[j]=c;
        }
    // handle fractional part
    for (txt[txtsiz]='.',txtsiz++;txtsiz<_txtsiz;)
        {
        y*=base;
        a=floor(y);
        y-=a;
        txt[txtsiz]=int(a)+'0'; txtsiz++;
        if (y<=0.0) break;
        }
    txt[txtsiz]=0;  // string terminator
    }
//---------------------------------------------------------------------------
void txt_print(float x0,float y0)   // print txt at x0,y0 [chars]
    {
    int i;
    float x,y;
    // fragment position [chars] relative to x0,y0
    x=0.5*(1.0+pos.x)/fxs; x-=x0;
    y=0.5*(1.0-pos.y)/fys; y-=y0;
    // inside bbox?
    if ((x<0.0)||(x>float(txtsiz))||(y<0.0)||(y>1.0)) return;
    // get font texture position for target ASCII
    i=int(x);               // char index in txt
    x-=float(i);
    i=txt[i];
    x+=float(int(i&31));
    y+=float(int(i>>5));
    x/=32.0; y/=8.0;    // offset in char texture
    col=texture2D(txr_font,vec2(x,y));
    }
//---------------------------------------------------------------------------
void nep_main()
    {
    col=vec4(0.0,1.0,0.0,1.0);  // background color
    txtsiz=0;
    txt[txtsiz]='F'; txtsiz++;
    txt[txtsiz]='l'; txtsiz++;
    txt[txtsiz]='o'; txtsiz++;
    txt[txtsiz]='a'; txtsiz++;
    txt[txtsiz]='t'; txtsiz++;
    txt[txtsiz]=':'; txtsiz++;
    txt[txtsiz]=' '; txtsiz++;
    txt_decimal(12.345);
    txt_print(1.0,1.0);
    gl_FragColor=col;
    }
//---------------------------------------------------------------------------
*/
//vec2 iResolution=vec2(1920.0,1080.0);

vec2 trans=1.0/iResolution;

void main()
{
//    vec2 uv=vec2(0.0,1.0)+trans*(gl_FragCoord.xy-1.0);
//    vec2 uv=vec2(0.0,1.0)+trans*(gl_FragCoord.xy-1.0);
    vec2 uv=(gl_FragCoord.xy)/iResolution;
    int r=0;
    vec4 col;
    vec4 c;
	col=texture2D(texture,  uv);
	if (col.g>0.5 && col.r>0.5)
        for (int x=-1; x<2; x++)
            for (int y=-1; y<2; y++) {
//                c=texture2D(texture,  uv+vec2(x,y)/iResolution);
                c=texture2D(texture,  uv+vec2(x,y)/iResolution);
                if ( c.g>0.5 && c.r<0.5 ) r++;
            }

    if (r==0) gl_FragColor=col;
	else gl_FragColor=vec4(1.0,0,0,1.0);
}
