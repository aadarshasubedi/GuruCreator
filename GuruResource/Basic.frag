
uniform bool applyTexture;
uniform sampler2D theTexture;

varying vec2 texCoord;

varying float lightIntensity;

void main()
{
	vec4 final = vec4( texture2D(theTexture, texCoord.st ) * gl_Color );
    final.xyz = final.xyz * lightIntensity;
    final.w = 1.0;
	gl_FragColor = final;
}
