attribute vec3 coord;
attribute vec2 tex_coord;
//~ attribute vec4 coord;
varying vec2 texpos;
uniform mat4 m_transform;

void main(void) {
  //~ gl_Position = m_transform * vec4(coord.xy, 0.001, 1);
  gl_Position = m_transform * vec4(coord.xy, coord.z + 0.001, 1);
  //~ gl_Position = m_transform * vec4(coord.xy, 2, 1);
  //~ texpos = coord.zw;
  texpos = tex_coord;
}
