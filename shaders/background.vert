in vec3 pos;
uniform mat2 scale;
uniform vec2 offset;

out vec2 texCoords;
void main(){
  texCoords=vec2(5*pos.x - 0.1*offset.x, 5*pos.y - 0.1*offset.y);
  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
