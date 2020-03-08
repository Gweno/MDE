attribute vec3 coord;
attribute vec2 tex_coord;
//~ attribute vec4 coord;
varying vec2 texpos;
uniform mat4 m_transform;

void main(void) {
  gl_Position = m_transform * vec4(coord.xy, coord.z + 0.0001, 1);
  texpos = tex_coord;
}
