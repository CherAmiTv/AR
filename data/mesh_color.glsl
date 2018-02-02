
#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;

uniform mat4 mvpMatrix;

void main( )
{
    gl_Position = mvpMatrix * ivec4(0,0,0,1);
    if(gl_VertexID == 0){
        gl_Position = vec4(-1,3,-1,1);
    }
    if(gl_VertexID == 1){
        gl_Position = vec4(-1,-1,-1,1);
    }
    if(gl_VertexID == 2){
        gl_Position = vec4(3,-1,-1,1);
    }
}
#endif

#ifdef FRAGMENT_SHADER

#ifdef USE_COLOR
in vec4 vertex_color;
#endif

uniform sampler2D diffuse_color;

out vec4 fragment_color;

void main( )
{
    vec4 baseColor = texelFetch(diffuse_color, ivec2(gl_FragCoord.xy), 0);
    fragment_color = baseColor;
}
#endif
