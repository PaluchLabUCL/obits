out vec4 outputColor;
in vec2 texCoords;
uniform sampler2D texSampler;
void main(){
   outputColor = texture(texSampler, texCoords)/4;
}
