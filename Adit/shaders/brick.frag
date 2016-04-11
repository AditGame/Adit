#version 120

varying vec3 v_V;
varying vec3 v_P;

void main() {
	vec3 N = normalize(cross(dFdy(v_P), dFdx(v_P))); // N is the world normal
	vec3 V = normalize(v_V);
	vec3 R = reflect(V, N);
	vec3 L = normalize(vec3(gl_LightSource[0].position));

	vec4 ambient = gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * max(dot(L, N), 0.0);
	vec4 specular = gl_FrontMaterial.specular * pow(max(dot(R, L), 0.0), gl_FrontMaterial.shininess);

	gl_FragColor = ambient + diffuse + specular;
}