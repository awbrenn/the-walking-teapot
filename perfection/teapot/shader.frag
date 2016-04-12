varying vec3 ec_vnormal, ec_vposition;
uniform sampler2D myDiffuse;
uniform sampler2D myNormal;
uniform sampler2D mySpecular;

void main() {
   vec3 diffuse_color = vec3(texture2D(myDiffuse, gl_TexCoord[0].st/gl_TexCoord[0].q));
   
   gl_FragColor = vec4(diffuse_color, 1.0);
}
