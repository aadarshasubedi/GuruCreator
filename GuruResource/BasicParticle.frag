
uniform sampler2D particleTexture;

varying vec2 pTexCoord;

varying float isAdditive;

void main()
{
	vec4 final = texture2D( particleTexture, pTexCoord.st ) * gl_Color;
	
//	if( isAdditive > 0.0 )
//	{
//	    final.w = 0.0;
//	}

	gl_FragColor = final + vec4(0.75, 0.2, 0.55, 0.5);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}




