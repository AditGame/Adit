#version 120

varying vec3 v_V;
varying vec3 v_P;

void main() {

	// Again, for the purposes of these examples we cannot be sure that per-vertex normals are provided. A sensible fallback 
	// is to use this little trick to compute per-fragment flat-shaded normals from the world positions using derivative operations.
	vec3 normal = -normalize(cross(dFdy(v_V.xyz), dFdx(v_V.xyz)));

	// from http://stackoverflow.com/questions/1964308/glsl-shader-generation-of-normals

	vec3 lightDir;
    vec4 diffuse;
    float NdotL;
    vec4 finalColor;


	// hardcoded light direction 
    vec4 light = gl_ModelViewMatrix * vec4(0.1, 0.1, 1.0, 0.0);
    lightDir = normalize(light.xyz);

    NdotL = max(dot(normal, lightDir), 0.0);

    diffuse = gl_Color;

    finalColor = diffuse * NdotL; 
    finalColor.a = 1.0; // final color ignores everything, except lighting

    gl_FragColor = finalColor;
	
	// We are just using the normal as the output color, and making it lighter so it looks a bit nicer. 
	// Obviously a real shader would also do texuring, lighting, or whatever is required for the application.
	//gl_FragColor = gl_Color + vec4(normal * 0.1, 1.0);
}