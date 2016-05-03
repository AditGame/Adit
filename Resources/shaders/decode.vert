#version 140 
         
uniform mat4 osg_ModelViewProjectionMatrix; 
uniform mat3 osg_NormalMatrix; 
uniform vec3 ecLightDir; 
 
in vec4 osg_Vertex; 
in vec3 osg_Normal; 
out vec4 color; 
 
void main() 
{ 
    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); 
    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); 
    color = vec4( vec3( diffuse ), 1. ); 
 
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; 
}