in vec2 xyPosition;
uniform mat2 scale;
uniform vec2 offset;
uniform vec4 plotColor;
smooth out vec4 color;
void main(){

    vec2 pos = scale*xyPosition + offset;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    color=plotColor;
}
