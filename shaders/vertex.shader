in vec2 xyPosition;
uniform mat2 scale;
uniform vec2 offset;
uniform vec3 plotColor;
smooth out vec4 color;

uniform vec2 center;
uniform mat2 orientation;

uniform int type;

void main(){
    //first transform into world coordinates
    vec2 world_pos = orientation*xyPosition + center;
    //then tranform into
    vec2 pos = scale*world_pos + offset;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    color=vec4(plotColor,1);
}
