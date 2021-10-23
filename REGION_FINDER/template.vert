//---------------------------------------------------------------------------
// Vertex
//---------------------------------------------------------------------------
#version 420 core
//#version 130
//---------------------------------------------------------------------------

/*
layout(location=0) in vec4 vertex;
out vec2 pos;   // screen position <-1,+1>
void main()
    {
    pos=vertex.xy;
    gl_Position=vertex;
    }

//---------------------------------------------------------------------------

*/
layout(location=0) in vec4 vertex;


void main()
{
    gl_Position=vertex*2.0-1.0;

//    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
//    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
//    gl_FrontColor = gl_Color;
}

