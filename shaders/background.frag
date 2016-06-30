out vec4 outputColor;
in vec2 texCoords;
uniform sampler2DArray texSamplers;
void main(){
   outputColor = texture(texSamplers, vec3(texCoords, 0));
}
