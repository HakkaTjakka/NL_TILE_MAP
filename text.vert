//---------------------------------------------------------------------------
// Vertex
//---------------------------------------------------------------------------
#version 420 core
//---------------------------------------------------------------------------
layout(location=0) in vec4 vertex;
out vec2 pos;   // screen position <-1,+1>
void main()
    {
    gl_Position=vertex*2.0-1.0;
    pos=vertex.xy*2.0-1.0;

//    pos=vertex.xy;
//    gl_Position=vertex;
    }
//---------------------------------------------------------------------------
