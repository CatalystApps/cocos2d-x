
const char* ccTechniqueOutputFrag = STRINGIFY(

\n#ifdef GL_ES\n
varying mediump vec2 v_texCoord;
\n#else\n
varying vec2 v_texCoord;
\n#endif\n
void main()
{
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);;
}
);
