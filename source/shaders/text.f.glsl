varying vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;
uniform vec4 bgcolor;

void main(void) {
  //~ gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
  //~ gl_FragColor = vec4(0, 0, 0, texture2D(tex, texpos).a) * color;
  //~ gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * bgcolor;
  //~ gl_FragColor = mix(color,vec4(1,1,1,1),1-texture2D(tex, texpos).a) ;
  gl_FragColor = mix(color,bgcolor,1-texture2D(tex, texpos).a) ;
}
