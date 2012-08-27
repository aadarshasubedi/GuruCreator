/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ModelDescriptor.cpp
Brief:      ModelDescriptor class implementation.
/************************************************************************/


#include "ModelDescriptor.h"
#include "ModelComponent.h"
#include "Engine.h"
#include "CRenderModule.h"


ModelDescriptor::ModelDescriptor() : ComponentDescriptor()
{
}


ModelDescriptor::~ModelDescriptor()
{
}


ComponentDisciple* ModelDescriptor::Create(Entity* const obj)
{
	CRenderModule* module = (CRenderModule*)Engine::GetSingletonPtr()->Module();
	
	ModelComponent* comp = new ModelComponent;

	//Set the values from one to the other.
	comp->SetFamily(mFamily);
	comp->SetName(mName);
	comp->SetType("Model");
	comp->SetParent(obj);

	//Create the model component.  The descriptor basically just copies over all of its data to the model, 
	// which is contained by the component.  The render module is included so that the model pointer
	// can be added to the render list.  @TODO: This will eventually get replaced by the scene graph.

	//Set all the data.
	comp->model()->allData() = this->mVboAllData;
	comp->model()->allIndices() = this->mVboIndices;
	comp->model()->numFaces() = this->mNumFaces;
	comp->model()->numVertsPerFace() = this->mNumVertsPerFace;
	comp->model()->drawType() = this->mDrawType;
	comp->model()->mParent = obj;
	module->mModels.push_back(comp->model());
 	return comp;
}

