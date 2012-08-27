//--------------------------------------------------------------------------------------
// File: DanShader.fx
//
// The effect file for the BasicHLSL sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 WorldViewProjMat;          // World * View * Projection matrix
float4x4 WorldMat;	
float3   LightDirection;
float3   LightDiffuse;
float3   PtLightPosition;

texture  s_MeshTexture;


//--------------------------------------------------------------------------------------
// A texture sampler for the mesh.  This sampler
//--------------------------------------------------------------------------------------


sampler MeshTextureSampler = 
sampler_state
{
    Texture = <s_MeshTexture>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
 

//--------------------------------------------------------------------------------------
// Vertex shader input structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float3 vNormal  : NORMAL;
	float4 color	: COLOR0;
	float2 texUV    : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 UVcoords   : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( VS_INPUT input, uniform bool doTexture, uniform bool doLighting, uniform bool doPoints )
{
    VS_OUTPUT Output;
    float3 vNormalWorld;
    float3 lightDir = PtLightPosition - (float3)input.position;
    
    //All I want is to ouput to keep the values and pass them onto the pixel shader. 
    Output.Position = mul( input.position, WorldViewProjMat );

	if( doPoints )
	{
		Output.Diffuse = input.color;//float3(0.0, 1.0, 0.0);
	}
	else
	{  
		vNormalWorld = mul( input.vNormal, (float3x3)WorldMat );	    
		Output.Diffuse.rgb = input.color.rgb;
		if( doLighting )
		{
			Output.Diffuse.rgb *= dot( lightDir, vNormalWorld );
		}
	    
		if ( doTexture )		
			Output.UVcoords = input.texUV;
		else
			Output.UVcoords = 0;
	}          
	
	Output.Diffuse.a = 0.8;	
        
    return Output;    
}



//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In, uniform bool use_texture) 
{ 
    PS_OUTPUT Output;

	if ( use_texture)
		Output.RGBColor = tex2D( MeshTextureSampler, In.UVcoords ) * In.Diffuse; 
	else
		Output.RGBColor = In.Diffuse;

    return Output;
}

technique RenderScene
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS( false, true, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)
        
        sampler[0] = <MeshTextureSampler>;
        
        ColorOp[0]   = MODULATE;            /* NEWER THAN NEW */
        ColorArg1[0] = TEXTURE;
        ColorArg2[0] = DIFFUSE;               /* MODIFIED */
        AlphaOp[0]   = SELECTARG1;            /* NEW */
        AlphaArg1[0] = DIFFUSE;               /* NEW */

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE; 
    }    
}

technique RenderSceneUnlit
{
	pass P0
	{	
        VertexShader = compile vs_2_0 RenderSceneVS( false, false, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)		
	}
}


