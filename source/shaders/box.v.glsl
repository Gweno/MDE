attribute vec3 coord3d_b;
attribute vec3 v_color_b;
varying vec3 f_color_b;
uniform mat4 m_transform;

void main(void) {
  gl_Position = m_transform * vec4(coord3d_b, 1.0);
  f_color_b = v_color_b;
}
