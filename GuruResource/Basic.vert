
//@todo:  Set up a few lights for lighting.

//@todo:

struct DVertex
{
	vec3 position;//		: POSITION;
	vec3 color;//	 		: COLOR0;
	vec2 tex;//			: TEXTURE0;
	vec3 normal;
};

//bindable uniform vec4 centers[1000];

uniform sampler2D theTexture;
varying vec2 texCoord;

uniform vec3 cameraPos;
const int NumLights = 3;
const float specContrib = 0.1;
const float diffContrib = 1.0 - specContrib;

varying float lightIntensity;

void DirectionalLight( in int i, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
    float nDotVP;
    float nDotHV;
    float pf;

    nDotVP = max( 0.0, dot(normal, normalize( vec3(gl_LightSource[i].position) ) ) );

    nDotHV = max( 0.0, dot(normal, vec3(gl_LightSource[i].halfVector) ) );

    if( nDotVP == 0.0 )
    {
        pf = 0.0;
    }
    else
    {
        pf = pow( nDotHV, gl_FrontMaterial.shininess );
    }

    ambient += gl_LightSource[i].ambient;
    diffuse += gl_LightSource[i].diffuse * nDotVP;
    specular += gl_LightSource[i].specular * pf;
}


void PointLight( in int i, in vec3 eye, in vec3 ecPosition3, in vec3 normal,
                 inout vec4 ambient, inout vec4 diffuse, inout vec4 specular )
{
    float nDotVP;   // normal * light dir
    float nDotHV;   // normal * light half vector
    float pf;       // power factor
    float attenuation;  //computed attenuation factor
    float d;
    vec3 VP;
    vec3 halfVector;

    //Compute vector from surface to light position
    VP = vec3( gl_LightSource[i].position ) - ecPosition3;

    //Compute distance between surface and light position
    d = length(VP);

    //Normalize vector from surface to light pos
    VP = normalize(VP);

    //Compute attenuation.
    attenuation = 1.0 / ( gl_LightSource[i].constantAttenuation + gl_LightSource[i].linearAttenuation * d + gl_LightSource[i].quadraticAttenuation * d * d );

    halfVector = normalize( VP + eye );

    nDotVP = max( 0.0, dot(normal, VP) );
    nDotHV = max( 0.0, dot(normal, halfVector) );

    if( nDotVP == 0.0)
    {
        pf = 0.0;
    }
    else
    {
        pf = pow( nDotHV, 16.0 );
    }

    ambient += gl_LightSource[i].ambient * attenuation;
    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
    specular += gl_LightSource[i].specular * pf * attenuation;

	lightIntensity = diffContrib * max( 0.0, dot( VP, normal ) ) + specContrib * pow(specContrib, 16.0 );
}

in DVertex myVert;
void main( )
{
    vec4 amb, diff, spec;
    amb = vec4(0,0,0,0);
    diff = vec4(0,0,0,0);
    spec = vec4(0,0,0,0);

    //lightIntensity = 1.0;

    vec4 vertEyePos;
    vec3 vertEyePos3;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(myVert.position, 1.0);

    vertEyePos = gl_ModelViewMatrix * vec4(myVert.position, 1.0);
    vertEyePos3 = vec3(vertEyePos) / vertEyePos.w;

	myVert.normal = normalize(myVert.normal);

	for( int iter = 0; iter < NumLights; ++iter )
	{
	    if( gl_LightSource[iter].position.w == 0.0 )
	    {
	        DirectionalLight( iter, myVert.normal, amb, diff, spec );
	    }
	    else if( gl_LightSource[iter].spotCutoff == 180.0 )
	    {
            PointLight( iter, cameraPos, vertEyePos3, myVert.normal, amb, diff, spec );
	    }
	    else
	    {
	    }
	}

	vec4 color;
	color = vec4(myVert.color, 1.0) * gl_FrontMaterial.emission + amb * gl_FrontMaterial.ambient + diff * gl_FrontMaterial.diffuse + spec * gl_FrontMaterial.specular;

	texCoord = myVert.tex;

	gl_FrontColor = color;
}


