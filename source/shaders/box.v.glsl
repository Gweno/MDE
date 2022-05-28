attribute vec3 coord3d_b;
//~ attribute vec3 v_color_b;
attribute vec4 v_color_b;
//~ varying vec3 f_color_b;
varying vec4 f_color_b;
//~ uniform mat4 m_transform;
//~ uniform mat4 m_model;
//~ uniform mat4 m_view;
//~ uniform mat4 m_projection;
uniform mat4 m_model_b;
uniform mat4 m_view_b;
uniform mat4 m_projection_b;
//~ uniform mat4 mvp;

void main(void) {
//~ gl_Position = m_transform * vec4(coord3d_b, 1.0);
//~ gl_Position = m_transform * m_model * vec4(coord3d_b, 1.0);
  //~ gl_Position = m_projection * m_view * m_model * vec4(coord3d_b, 1.0);
  gl_Position = m_projection_b * m_view_b * m_model_b * vec4(coord3d_b, 1.0);
  //~ gl_Position = mvp * vec4(coord3d_b, 1.0);
  //~ gl_Position = m_projection * m_transform * vec4(coord3d_b, 1.0);
  f_color_b = v_color_b;
}
