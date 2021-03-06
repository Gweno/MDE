varying vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;
uniform vec4 bgcolor;
uniform vec4 test_color;

void main(void) {
  gl_FragColor = mix(color,bgcolor,1-texture2D(tex, texpos).a) ;
}
