//---------------------------------------------------------------------------
// Fragment
//---------------------------------------------------------------------------
#version 420 core
//---------------------------------------------------------------------------
in vec2 pos;                    // screen position <-1,+1>
out vec4 gl_FragColor;          // fragment output color
uniform sampler2D txr_font;     // ASCII 32x8 characters font texture unit
uniform float fxs,fys;          // font/screen resolution ratio
uniform sampler2D texture;
uniform vec2 iResolution;
ivec2 intResolution=ivec2(iResolution);
//---------------------------------------------------------------------------
const int _txtsiz=32;           // text buffer size
int txt[_txtsiz],txtsiz;        // text buffer and its actual size
vec4 col;                       // color interface for txt_print()
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
    if ((x<0.0)||(x>float(txtsiz))||(y<0.0)||(y>1.0)) {
        return;
    }
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
vec2 trans=vec2(1.0,-1.0)/iResolution;

void main()
    {
    vec2 uv=vec2(0.0,1.0)+trans*(gl_FragCoord.xy);
    col.rg=uv;
    col.a=1.0;

    if ( int(uv.x*iResolution.x)==0 && int(uv.y*iResolution.y)==0 )
        col=vec4(float('Y')/255.0, float('E')/255.0, float('S')/255.0, 1.0 );
    else if ( int(uv.x*iResolution.x)==1 && int(uv.y*iResolution.y)==0 )
        col=vec4(float('Y')/255.0, float('O')/255.0, float('U')/255.0, 1.0 );

    vec4 col2=texelFetch(texture,ivec2(0,0),0);

    txtsiz=0;
    txt[txtsiz++]=int(255.0*col2.r);
    txt[txtsiz++]=int(255.0*col2.g);
    txt[txtsiz++]=int(255.0*col2.b);
    txt[txtsiz++]=int(255.0*col2.a);

    col2=texelFetch(texture,ivec2(1,0),0);
    txt[txtsiz++]=int(255.0*col2.r);
    txt[txtsiz++]=int(255.0*col2.g);
    txt[txtsiz++]=int(255.0*col2.b);
    txt[txtsiz++]=int(255.0*col2.a);

    txt_print(0.0,1.0);

/*
    txtsiz=0;
    txt[txtsiz]='R'; txtsiz++;
    txt[txtsiz]=':'; txtsiz++;
    txt_decimal(255.0*col2.r);
    txt_print(1.0,1.0);

    txtsiz=0;
    txt[txtsiz]='G'; txtsiz++;
    txt[txtsiz]=':'; txtsiz++;
    txt_decimal(255.0*col2.g);
    txt_print(1.0,2.0);

    txtsiz=0;
    txt[txtsiz]='B'; txtsiz++;
    txt[txtsiz]=':'; txtsiz++;
    txt_decimal(255.0*col2.b);
    txt_print(1.0,3.0);

    txtsiz=0;
    txt[txtsiz]='A'; txtsiz++;
    txt[txtsiz]=':'; txtsiz++;
    txt_decimal(255.0*col2.a);
    txt_print(1.0,4.0);
*/
    gl_FragColor=col;
    }
//---------------------------------------------------------------------------
