//@todo:


struct ParticleVertex
{
	vec3 position;
	vec2 tex;
	vec3 color;
	vec3 partPos;
	vec3 oldPos;
	bool additive;
	float rotation;
	float size;
};

struct OutVertex
{
    bool additive;
};


uniform float aspectRatio;
uniform sampler2D particleTexture;

varying vec2 pTexCoord;
varying float isAdditive;

in ParticleVertex myParticle;

void main( )
{
    vec4 finalPos;
    vec4 vertEyePos;
    vec3 vertEyePos3;

    //vec3 temp = myParticle.position + myParticle.partPos;

    //finalPos = gl_ModelViewProjectionMatrix * vec4( myParticle.partPos, 1.0 );

    vertEyePos = gl_ModelViewMatrix * vec4( myParticle.partPos, 1.0 );
    finalPos = gl_ModelViewProjectionMatrix * vec4( myParticle.position, 1.0 );
    //finalPos = gl_ModelViewProjectionMatrix * vec4( myParticle.partPos + myParticle.position, 1.0 );


    vertEyePos3 = vec3(vertEyePos) / vertEyePos.w;

    float dist = finalPos.z / finalPos.w;

    mat2 rotMat;
    rotMat[0][0] = cos(myParticle.rotation);
    rotMat[0][1] = sin(myParticle.rotation);
    rotMat[1][0] = -rotMat[0][1];
    rotMat[1][1] = rotMat[0][0];

    vec2 rotPos = rotMat * vec2(myParticle.partPos);
//
//    finalPos.x = finalPos.x + (myParticle.size *(0.5/dist) );
//    finalPos.y = finalPos.y + (1.0/aspectRatio * myParticle.size *(0.5/dist) );

    finalPos.x = finalPos.x + (rotPos.x * (myParticle.size * (1.0/dist)) * 0.5 );
    finalPos.y = finalPos.y + (rotPos.y * ((800.0/600.0) * myParticle.size * (1.0/dist) ) * 0.5);

    //myParticle.tex.x = myParticle.tex.x * ( 1.0 / aspectRatio);
    gl_Position = finalPos;

	pTexCoord = myParticle.tex;
	isAdditive = float(myParticle.additive);

	gl_FrontColor = vec4(myParticle.color, 1.0);
}




