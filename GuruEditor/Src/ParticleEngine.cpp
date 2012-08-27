/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

//
#include "Engine.h"
#include "Camera.h"
#include "Game_Renderer.h"
#include "ParticleEngine.h"
#include "Helper.h"
#ifdef USE_LUA
#include "LunarHelper.h"
#endif


using namespace std;


//This function will create the vbos and set the screen aligned quad.  No buffering for
// the particle buffer since it will be filled during render.
void ParticleEngine::Init()
{
    //Create buffer for the screen aligned quad.
    glGenBuffersARB( 1, &mVboQuadVerts );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, mVboQuadVerts );

    //Now create the four points and their tex coords.
    float vertAttrib[ 4 * 5 ];
    GLint indices[4];

    vertAttrib[0] = -1;
    vertAttrib[1] = 1;
    vertAttrib[2] = 0;

    vertAttrib[3] = 0.0f;
    vertAttrib[4] = 0.0f;

    vertAttrib[5] = -1;
    vertAttrib[6] = -1;
    vertAttrib[7] = 0;

    vertAttrib[8] = 0.0f;
    vertAttrib[9] = 1.0f;

    vertAttrib[10] = 1;
    vertAttrib[11] = -1;
    vertAttrib[12] = 0;

    vertAttrib[13] = 1.0f * (600.0f/800.0f);
    vertAttrib[14] = 1.0f;

    vertAttrib[15] = 1;
    vertAttrib[16] = 1;
    vertAttrib[17] = 0;

    vertAttrib[18] = 1.0f * (600.0f/800.0f);
    vertAttrib[19] = 0.0f;

    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof( float ) * 4 * 5, vertAttrib, GL_STREAM_DRAW_ARB );

    //Create index buffer for saq.
    glGenBuffers( 1, &mVboQuadIndices );

    //Bind the saq buffer and buffer the data to it.  The quad is always
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mVboQuadIndices );

    // the whole screen, and will be scaled depending on the dist away from the eye.
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;

    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof( GLint ) * 4, indices, GL_STREAM_DRAW_ARB );

    //Create buffer for the master particles.
    glGenBuffersARB( 1, &mVboParticles );

	//Create the glsl program the shaders will attach to.
	mShaderProgram = glCreateProgram();

    //Now that the VBO's have been taken care of, load in the shaders for the particles.
    LoadFullShaderProgram( "resource/BasicParticle.vert", "resource/BasicParticle.frag" );
	glLinkProgram( mShaderProgram );

    glBindProgramARB( GL_VERTEX_PROGRAM_ARB, mVertShader );
	glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, mFragShader);


	//Now that we have the program linked, grab the attributes to set.
	mPosAttrib = glGetAttribLocation( mShaderProgram, "myParticle.position" );
	mTexAttrib = glGetAttribLocation( mShaderProgram, "myParticle.tex" );
	mColAttrib = glGetAttribLocation( mShaderProgram, "myParticle.color" );
	mPartPosAttrib = glGetAttribLocationARB( mShaderProgram, "myParticle.partPos" );
	mPartOldAttrib = glGetAttribLocationARB( mShaderProgram, "myParticle.oldParticlePos" );
	mAddAttrib = glGetAttribLocationARB( mShaderProgram, "myParticle.additive" );
	mRotAttrib = glGetAttribLocationARB( mShaderProgram, "myParticle.rotation" );
    mSizeAttrib = glGetAttribLocationARB( mShaderProgram, "myParticle.size" );

	mAspectAttrib = glGetUniformLocation( mShaderProgram, "aspectRatio" );
	mTextureUniform = glGetUniformLocation( mShaderProgram, "particleTexture" );
}



//This function will push all particles into the master list, along with setting any necessary states.
void ParticleEngine::Begin(bool clearMasterList, bool sortParticles, CVector3 camPos)
{
    //Set all of the graphics states necessary to render the particles.
    //Set all textures active here.

    //glDisable( GL_DEPTH );


    //glDisable( GL_DEPTH_TEST );
	glEnable( GL_VERTEX_PROGRAM_ARB );
	glEnable( GL_FRAGMENT_PROGRAM_ARB );
	glDisable( GL_DEPTH_TEST );

	glUseProgram( mShaderProgram );

	//Clear list if necessary.
	if( clearMasterList )
	{
	    mMasterParticles.clear();
	}

    //Go through the particles and push them into the master list.

    //Iterate over all the systems and "render" them.  What this does is go through all groups and add their particles to the
    // guru's master list.  This list will then be sorted before the saq is drawn for each.

    //Start with the systems, iterate over their layers, and push the particles with their depth to the list.
    particle_systems::iterator sys_begin = mParticleSystems.begin();
    particle_systems::const_iterator sys_end = mParticleSystems.end();

    while( sys_begin != sys_end )
    {
        (*sys_begin)->Render( camPos );
        ++sys_begin;
    }

    mSortParticles = sortParticles;
}


void ParticleEngine::Render()
{
    //The particle sorting will be performed here before any data sent to graphics card.
    if( mSortParticles )
    {
        std::sort( mMasterParticles.begin(), mMasterParticles.end(), SortParticleDepth );
    }

    //unsigned size = mMasterParticles.size();

    //Here vertex attributes and any data sent to the shader will be done here.glUniform1i( ma, GL_TEXTURE0 );
    //Push data to graphics card, and for each particle render the saq passing those particle attributes to the shader.
    CRenderModule* mod = Engine::GetSingletonPtr()->Module();

	glActiveTexture(mTextures[0]);
	glUniform1i( mTextureUniform, GL_TEXTURE0 );	
    glUniform1f( mAspectAttrib, mod->GetCamera()->AspectRatio() );

	glBindBuffer( GL_ARRAY_BUFFER, this->mVboQuadVerts );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->mVboQuadIndices );

	glVertexAttribPointer( mPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct ParticleVertexData), (void*)(sizeof(GLfloat)*0) );
	glVertexAttribPointer( mTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(struct ParticleVertexData), (void*)(sizeof(GLfloat)*3) );
	//glVertexAttribPointer( mColAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexData), (void*)(sizeof(GLfloat)*5) );

	glEnableVertexAttribArray( mPosAttrib );
	glEnableVertexAttribArray( mTexAttrib );

	for_each(begin(mMasterParticles), end(mMasterParticles), [](ParticleSortNode psorter)
    {
        Particle* particle = (Particle*)psorter.data;
		ParticleEngine* pGuru = (ParticleEngine *)ParticleGuru::GetSingletonPtr();

        CVector3 pos = particle->mPos;
        glVertexAttrib3f( pGuru->mPosAttrib, pos.x, pos.y, pos.z);

        pos = particle->mColor;
        glVertexAttrib3f( pGuru->mColAttrib, pos.x, pos.y, pos.z );

        pos = particle->mOldPosition;
        glVertexAttrib3f( pGuru->mPartOldAttrib, pos.x, pos.y, pos.z);

        glVertexAttrib1f( pGuru->mAddAttrib, particle->mIsAdditive );

        glVertexAttrib1f( pGuru->mRotAttrib, particle->mRotation );

        glVertexAttrib1f( pGuru->mSizeAttrib, particle->mSize );

        ////////////////////Now the actual draw call.//////////////////////////

        glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, 0 );

        ///////////////////////////////////////////////////////////////////////
    });

    //If necessary, deactivate attribPointers to set new ones if needed.
	glDisableVertexAttribArray( mTexAttrib );
	glDisableVertexAttribArray( mPosAttrib );

    //@todo:  Depending on how i can send the data to the shader, i may have to loop through the particles to draw each quad so
    // so i can make sure to pass the right particle data per quad, along with the saq's vert data.

    //The element array and array buffers will be cleared in the End function.
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}



void ParticleEngine::End()
{
    //This will end the rendering and de-set any states set, and clear the master particles list.

    //Clear master particle list.
    mMasterParticles.clear();
    glEnable( GL_DEPTH );
}


void ParticleEngine::LoadVertexShader( const string& file )
{
    char program_string[16834], shader_info[16834];

    mVertShader = load_shader_source( file.c_str(), GL_VERTEX_SHADER, mShaderProgram, program_string, shader_info );
}

void ParticleEngine::LoadFragmentShader( const string& file )
{
    char program_string[16834], shader_info[16834];

    mFragShader = load_shader_source( file.c_str(), GL_FRAGMENT_SHADER, mShaderProgram, program_string, shader_info );
}

void ParticleEngine::LoadFullShaderProgram( const string& vertFile, const string& fragFile )
{
    LoadVertexShader( vertFile );
    LoadFragmentShader( fragFile );
}

GLuint ParticleEngine::GetParticlesVBO() const
{
    return mVboParticles;
}

GLuint ParticleEngine::GetQuadVBO_verts() const
{
    return mVboQuadVerts;
}

GLuint ParticleEngine::GetQuadVBO_indices() const
{
    return mVboQuadIndices;
}

GLuint ParticleEngine::GetShaderProgram() const
{
    return mShaderProgram;
}


GLuint* ParticleEngine::GetTexturesListPtr()
{
    return mTextures;
}

void    ParticleEngine::AddTextureRef( GLuint texture )
{
    mTextures[mNumTexturesActive] = texture;
    ++mNumTexturesActive;
}

GLuint  ParticleEngine::GetTextureRef( unsigned index )
{
	if( mTextures && mNumTexturesActive )
	{
		if( index > MAX_PARTICLE_TEXTURES )
		{
			return mTextures[0];
		}
		else if( 0 <= index && index < mNumTexturesActive )
		{
			//This is a bad texture reference since this doesn't exist yet.  return mNumTexturesActive - 1 to guarantee a set textures.
			return mTextures[index];
		}
	}
	
	//Getting here means that there were no active textures.  Return and go AHHHHHHH
	return 0;

}



ParticleSystem* ParticleEngine::CreateDemoSystem()
{
    mParticleSystems.push_back( new ParticleSystem() );
    ParticleSystem* sys = mParticleSystems.back();

    sys->SetLife( 10.0f );
    sys->SetSorted( true );

    //sys->SetPosition( CVector3(0, 20, 0 ) );

    //Set this system up at the origin, with velocity straight up, and identity orientation.
    //Set the life of the system to be 5 seconds (5000 in milliseconds.)


    //Set layers as continuous.  Do no sorting, so default is false.
    //Add a particle group with the texture being used by the basic objects for now.
    //Set texutre id and program.
    //Add a reference of the texture to the particle guru, that way it'll know which textures
    // to load in quickly before rendering.
    AddTextureRef( 1 );

    ParticleGroup* group = sys->AddParticleGroup( GetTextureRef(0), 1, 1.0f );
    group->SetContinuous( true );
    group->SetAdditive( false );

    //Now set all of the spawn and update parameters for each attribute.
    //Params for color are initial values of r, g, b... so 0, 1, 2 indices and whatever values.
    sys->SetGroupSpawnParam_Col(0, 0, 0.2f );
    sys->SetGroupSpawnParam_Col(0, 1, 0.5f );
    sys->SetGroupSpawnParam_Col(0, 2, 0.1f );
    sys->SetGroupSpawnParam_Col(0, 3, 0.8f );
    sys->SetGroupSpawnParam_Col(0, 4, 0.5f );
    sys->SetGroupSpawnParam_Col(0, 5, 0.8f );

    sys->SetGroupSpawnParam_Life(0, 0, 8.0f);
    sys->SetGroupSpawnParam_Life(0, 1, 8.0f);
    sys->SetGroupSpawnParam_Life(0, 2, 0.0f);
    sys->SetGroupSpawnParam_Life(0, 3, 0.0f);
    sys->SetGroupSpawnParam_Life(0, 4, 0.0f);
    sys->SetGroupSpawnParam_Life(0, 5, 0.0f);

    sys->SetGroupSpawnParam_Pos(0, 0, 0.0f);
    sys->SetGroupSpawnParam_Pos(0, 1, 0.0f);
    sys->SetGroupSpawnParam_Pos(0, 2, 0.0f);
    sys->SetGroupSpawnParam_Pos(0, 3, 0.0f);
    sys->SetGroupSpawnParam_Pos(0, 4, 0.0f);
    sys->SetGroupSpawnParam_Pos(0, 5, 0.0f);

    sys->SetGroupSpawnParam_Vel(0, 0, -2.1f);
    sys->SetGroupSpawnParam_Vel(0, 1, 2.1f);
    sys->SetGroupSpawnParam_Vel(0, 2, 8.0f);
    sys->SetGroupSpawnParam_Vel(0, 3, 10.0f);
    sys->SetGroupSpawnParam_Vel(0, 4, -2.1f);
    sys->SetGroupSpawnParam_Vel(0, 5, 2.1f);

    sys->SetGroupSpawnParam_Rot(0, 0, 0.0f);
    sys->SetGroupSpawnParam_Rot(0, 1, 0.0f);
    sys->SetGroupSpawnParam_Rot(0, 2, 0.0f);
    sys->SetGroupSpawnParam_Rot(0, 3, 0.0f);
    sys->SetGroupSpawnParam_Rot(0, 4, 0.0f);
    sys->SetGroupSpawnParam_Rot(0, 5, 0.0f);

    sys->SetGroupSpawnParam_Size(0, 0, 0.5f);
    sys->SetGroupSpawnParam_Size(0, 1, 12.0f);
    sys->SetGroupSpawnParam_Size(0, 2, 0.0f);
    sys->SetGroupSpawnParam_Size(0, 3, 0.0f);
    sys->SetGroupSpawnParam_Size(0, 4, 0.0f);
    sys->SetGroupSpawnParam_Size(0, 5, 0.0f);

    //The group must set it's own update values for now.  Helpers are on the way.
    float val_params[4][NUM_PARAMS];

    //0 index is size - min and max
    val_params[0][0] = 0.0f;
    val_params[0][1] = 80.0f;
    val_params[0][2] = 0.0f;
    val_params[0][3] = 0.0f;
    val_params[0][4] = 0.0f;
    val_params[0][5] = 0.0f;

    //1 index is velocity - 3 * min/max
    val_params[1][0] = 0.0f;
    val_params[1][1] = 10.2f;
    val_params[1][2] = 0.0f;
    val_params[1][3] = 0.0f;
    val_params[1][4] = 0.0f;
    val_params[1][5] = 0.0f;

    //2 index is rotation
    val_params[2][0] = 1.0f;
    val_params[2][1] = 2.0f;
    val_params[2][2] = 0.0f;
    val_params[2][3] = 0.0f;
    val_params[2][4] = 0.0f;
    val_params[2][5] = 0.0f;

    //3 index is color
    val_params[3][0] = 0.2f;
    val_params[3][1] = 0.8f;
    val_params[3][2] = 0.2f;
    val_params[3][3] = 0.8f;
    val_params[3][4] = 0.2f;
    val_params[3][5] = 0.8f;

    group->SetSizeUpdateParams( val_params[0] );
    group->SetVelUpdateParams( val_params[1] );
    group->SetRotUpdateParams( val_params[2] );
    group->SetColUpdateParams( val_params[3] );

    //After that, set the spawn and update functions pointers.  Once that's done, the system should be ready to go.
    sys->SetGroupSpawnFunc_Color( 0, GetColorSpawnFunc("SpawnRandomColor") );
    sys->SetGroupSpawnFunc_Size( 0, GetSizeSpawnFunc("SpawnRandomSize") );
    sys->SetGroupSpawnFunc_Life( 0, GetLifeSpawnFunc("SpawnRandomLife") );
    sys->SetGroupSpawnFunc_Position( 0, GetPositionSpawnFunc("SpawnRandomPosition") );
    sys->SetGroupSpawnFunc_Rotation( 0, GetRotationSpawnFunc("SpawnRandomRotation") );
    sys->SetGroupSpawnFunc_Velocity( 0, GetVelocitySpawnFunc("SpawnRandomVelocity") );

    sys->SetGroupUpdateFunc_Color( 0, GetColorUpdateFunc("LinearGrowth") );
    sys->SetGroupUpdateFunc_Size( 0, GetSizeUpdateFunc("ExponentialDecay") );
    sys->SetGroupUpdateFunc_Rotation( 0, GetRotationUpdateFunc("LinearDecay")  );
    sys->SetGroupUpdateFunc_Velocity( 0, GetVelocityUpdateFunc("ExponentialDecay") );

    return sys;
}
