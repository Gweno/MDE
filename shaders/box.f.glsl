varying vec3 f_color_b;

void main(void) {
  gl_FragColor = vec4(f_color_b.r, f_color_b.g, f_color_b.b, 1.0);
}
