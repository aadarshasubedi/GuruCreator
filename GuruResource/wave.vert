

struct DVertex
{
	vec3<float> position	: POSITION;
	vec3<float> color 		: COLOR0;
	vec2<float> texs		: TEXTURE0;
};

struct DPixel
{
	vec3<int> pos		: POSITION;
	vec3<float> color	: COLOR0;
};


void main( in DVertex IN )
{
	DPixel OUT;

	//IN.position.y += 3000;
	gl_Position = gl_ModelViewProjectionMatrix * vec4<float>(IN.position, 1.0f);
	//gl_FragColor = IN.color
	//gl_Position = ftransform();
	//OUT.col0.xyz asdf= IN.color.xyz;

	//gl_Position = OUT.hPos;
	//gl_FragColor = Out.col0;

	//gl_Position = IN.position;
	//gl_Color  = IN.color.xyz;
}


