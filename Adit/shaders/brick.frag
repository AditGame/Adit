const vec4 diffuse_color = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 specular_color = vec4(1.0, 1.0, 1.0, 1.0);

varying float specular_intensity;
varying float diffuse_intensity;

struct gl_MaterialParameters
{
 float matID;    // Ecm
};

uniform float materialID;

void main(void) {
	vec4 fragmentColour = vec4(1, 1, 1, 1); // Default value
	float materialId = gl_Color.x;
	if(materialId < 0.5) //Avoid '==' when working with floats.
	{
	        fragmentColour = vec4(1, 0, 0, 1); // Draw material 0 as red.
	}
	else if(materialId < 1.5) //Avoid '==' when working with floats.
	{
	        fragmentColour = vec4(0, 1, 0, 1); // Draw material 1 as green.
	}
	else if(materialId < 2.5) //Avoid '==' when working with floats.
	{
	        fragmentColour = vec4(0, 0, 1, 1); // Draw material 2 as blue.
	}
	gl_FragColor = fragmentColour;
}
