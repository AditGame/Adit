varying vec3 v_V;
varying vec3 v_P;
 
void main()
{
	gl_Position = ftransform();
	v_P = gl_Position.xyz; // v_P is the world position
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	gl_FrontColor = gl_Color;
}