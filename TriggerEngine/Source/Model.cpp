/******************************************************************************/
/*!
\file Model.cpp
\author Long Jiquan Joel, TEAM TRIGGER
\par email: jiquanjoel.long\@digipen.edu
\par email: z.pung\@digipen.edu
\par email: kwanyee.leung\@digipen.edu
\par email: yunanjoel.lew\@digipen.edu
\par email: weiwei.hong\@digipen.edu
\par email: shu.yang\@digipen.edu
\par Course: GAM300
\par Game: Wayward
\brief
Copyright (C) 2014 DigiPen Institute of Technology.
*/
/******************************************************************************/

#include "Model.h"
#include "System.h"
#include "Timer.h"

static const Mat4 scale_bias_matrix = Mat4(Vec4(0.5f, 0.0f, 0.0f, 0.0f),
	                                       Vec4(0.0f, 0.5f, 0.0f, 0.0f),
	                                       Vec4(0.0f, 0.0f, 0.5f, 0.0f),
	                                       Vec4(0.5f, 0.5f, 0.5f, 1.0f));
VertexType::VertexType()
{
#pragma omp parallel for 
	for (int i = 0; i < NUM_BONES_PER_VERTEX; ++i)
	{
		boneIds[i] = 0;
		boneWeights[i] = 0;
	}
}

void VertexType::AddBoneData(int BoneID, float Weight, VertexType &Bonedata)
{
	for (int i = 0; i < NUM_BONES_PER_VERTEX; i++) 
	{
		if (Bonedata.boneWeights[i] == 0.0)
		{
			Bonedata.boneIds[i] = BoneID;
			Bonedata.boneWeights[i] = Weight;
			return;
		}
	}
}

GLuint Model::mFirstPassProg = 0;
GLuint Model::mShadowMapLoc = 0;
GLuint Model::mForwardRenderProg = 0;

void Model::FirstPassProg()
{
	mFirstPassProg = mFirstPassProg ? mFirstPassProg : System::GetInstance()->GetGraphic()->Get_FirstPassProgram();
}

void Model::ForwardRenderProg()
{
	mForwardRenderProg = mForwardRenderProg ? mForwardRenderProg : System::GetInstance()->GetGraphic()->Get_ForwardRednerProgram();
}

Model::Model() : m_animationtrack(this)
{
	FirstPassProg();
	GetShadowMapLocation();
	ForwardRenderProg();
	
	mDeferred_ModelLocation = glGetUniformLocation(mFirstPassProg, "modelMat");

	mBoneMatrix_Location = glGetUniformLocation(mFirstPassProg, "gBones");

	mAnimation_Location = glGetUniformLocation(mFirstPassProg, "isANIMATION");

	mHightLight_Location = glGetUniformLocation(mFirstPassProg, "isHighLight");

	mDebug_Location = glGetUniformLocation(mFirstPassProg, "isDebug");

	mDebugColor_Location = glGetUniformLocation(mFirstPassProg, "isCollided");
}

const int VERTEX_STRIDE_OFFSET_ = sizeof(VertexType);
void Model::BindAttributes()
{
	const VertexLayoutElement *layout = vertexLayout;
	int count = 0;
	//for the last element in layout, layout->size is 0 so the loop stops
	while (layout->size)
	{
		if (count == 3)
		{
			glEnableVertexAttribArray(layout->index);
			glVertexAttribIPointer(layout->index, layout->size, layout->type, VERTEX_STRIDE_OFFSET_, (void*)layout->offset);
			layout++;
		}
		else
		{

			glEnableVertexAttribArray(layout->index);
			glVertexAttribPointer(layout->index, layout->size, layout->type, layout->normalized, VERTEX_STRIDE_OFFSET_, (void*)layout->offset);
			layout++;
		}
		++count;
	}
}


void Model::BindBuffer()
{
	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(mVertexData[0]), &mVertexData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(mIndexData[0]), &mIndexData[0], GL_STATIC_DRAW);

	BindAttributes();
}

void Model::UnBindVAO()
{
	glBindVertexArray(0);
}

void Model::ReBindVAO()
{
	glBindVertexArray(mVertexArrayID);
}

void Model::AddAnimationTrackTimer(float start, float end)
{
	m_animationtrack.AddAnimationData(start, end);
}

void Model::Render(Mat4 &ModelMat, Texture* mTexture, int mFlag, int anim, float timer, const Vec2& scaling)
{
	//set normal draw
	glUniform1i(mDebug_Location, false);
	glUniformMatrix4fv(mDeferred_ModelLocation, 1, GL_FALSE, glm::value_ptr(ModelMat));
	//transformation for model
	std::vector<Mat4> Transforms;
	mAnimationData.BoneTransform(m_animationtrack.GetTime(anim, timer), Transforms);
	if (Transforms.size() == 0)
		glUniform1i(mAnimation_Location, false);
	else
	{
		glUniform1i(mAnimation_Location, true);
		glUniformMatrix4fv(mBoneMatrix_Location, Transforms.size(), GL_FALSE, glm::value_ptr(Transforms[0]));
	}
	if (mFlag & RENDER_HIGHLIGHT)
	{
		glUniform1i(mHightLight_Location, true);
	}
	else
	{
		glUniform1i(mHightLight_Location, false);
	}
	/* Passing texture maps into shaders */
	glActiveTexture(GL_TEXTURE0 + mTexture->mID);
	glBindTexture(GL_TEXTURE_2D, mTexture->mTextureID);
	//texture sacle
	int loc = glGetUniformLocation(mFirstPassProg, "Tex_scale");
	glUniform2fv(loc, 1, glm::value_ptr(scaling));

	ReBindVAO();
	//draw wire or tri
	if (mFlag & RENDER_WIREFRAME)
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	
	UnBindVAO();
}

void Model::RenderGAOMAO(Mat4 &mFinalMtx, bool isCollided)
{
	//set debug draw
	glUniform1i(mDebug_Location, true);
	//animation off
	glUniform1i(mAnimation_Location, false);
	//highlight off
	glUniform1i(mHightLight_Location, false);
	//passing mtx
	glUniformMatrix4fv(mDeferred_ModelLocation, 1, GL_FALSE, glm::value_ptr(mFinalMtx));
	//draw green or draw red
	if (isCollided)
	{
		glUniform1i(mDebugColor_Location, true);
	}
	else
	{
		glUniform1i(mDebugColor_Location, false);
	}
	//draw obj
	ReBindVAO();
	glDrawElements(GL_LINE_STRIP, indexCount, GL_UNSIGNED_INT, nullptr);
	UnBindVAO();
}

void Model::GetShadowMapLocation()
{
	mShadowMapLoc = System::GetInstance()->GetGraphic()->Get_ShadowMapLocation();
}

void Model::RenderShadowMap(Mat4 &ModelMat)
{
	glUniformMatrix4fv(mShadowMapLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
	ReBindVAO();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	UnBindVAO();
}

Model::~Model()
{

}

void Model::Destroy()
{
	glDeleteVertexArrays(1, &mVertexArrayID);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);

	mVertexData.clear();
	mIndexData.clear();
}

void Model::ForwardRender(const Mat4 &ModelMat, const Texture* mTexture, const Vec2& scaling, const int Number, const Texture *tex, bool isTerrain)
{
	static const glm::mat4 scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
														 glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
														 glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
														 glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	Mat4 viewMTX = System::GetInstance()->GetGraphic()->GetCamera()->GetViewMatrix();
	Mat4 ProjMTX = System::GetInstance()->GetOpenGL()->GetPerspective();
	Mat4 shadowMat = System::GetInstance()->GetGraphic()->GetLightViewMat();
	glUseProgram(mForwardRenderProg);
	//passing mtx
	int loc = glGetUniformLocation(mForwardRenderProg, "modelMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	loc = glGetUniformLocation(mForwardRenderProg, "viewMat");
	/* Pass the viewing matrix into the shaders */
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMTX));

	loc = glGetUniformLocation(mForwardRenderProg, "projMat");
	/* Pass the projection matrix into the shaders */
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(ProjMTX));

	loc = glGetUniformLocation(mForwardRenderProg, "shadowMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(shadowMat));

	loc = glGetUniformLocation(mForwardRenderProg, "bias_scale");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(scale_bias_matrix));

	loc = glGetUniformLocation(mForwardRenderProg, "Tex_scale");
	glUniform2fv(loc, 1, glm::value_ptr(scaling));

	loc = glGetUniformLocation(mForwardRenderProg, "draw_type");
	glUniform1i(loc,isTerrain);

	
	LIGHTSHADER_INST->SetLightProg(mForwardRenderProg);

	if (Number != 0)
	{
		/* Passing texture maps into shaders */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex[0].mTextureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex[1].mTextureID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex[2].mTextureID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex[3].mTextureID);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, System::GetInstance()->GetGraphic()->GetShadowDepth());
	}
	else
	{
		/* Passing texture maps into shaders */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture->mTextureID);
	}
	ReBindVAO();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	UnBindVAO();
}

void Model::RednerSkybox(Mat4 &ModelMat, float id, float STATE)
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	Mat4 viewMTX = System::GetInstance()->GetGraphic()->GetCamera()->GetViewMatrix();
	Mat4 ProjMTX = System::GetInstance()->GetOpenGL()->GetPerspective();
	SkyBoxInst->Render(ModelMat, viewMTX, ProjMTX, id, STATE);

	ReBindVAO();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	UnBindVAO();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}

