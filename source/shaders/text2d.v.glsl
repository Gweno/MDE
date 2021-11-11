//~ attribute vec3 coord;
attribute vec2 tex_coord;
attribute vec4 fontcolor;
attribute vec4 coord;
varying vec4 fontcol;
varying vec2 texpos;
//~ uniform mat4 m_transform;
//~ uniform mat4 m_model2d;
//~ uniform mat4 m_view;
//~ uniform mat4 m_projection2d;

void main(void) {
  //~ gl_Position = m_projection2d * m_view * vec4(coord.xy, 0, 1);
  //~ gl_Position = m_projection2d * m_view * m_model * vec4(coord2d.xy, 0, 1);
  //~ gl_Position = m_projection2d * m_model2d * vec4(coord.xy, 0, 1);
  gl_Position = vec4(coord.xy, 0, 1);
  texpos = tex_coord;
  //~ texpos = coord.zw;
  fontcol = fontcolor;
}
