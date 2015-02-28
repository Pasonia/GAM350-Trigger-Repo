#include "Grass.h"
#include "texturemanager.h"

GrassClass::GrassClass()
{

}

GrassClass::~GrassClass()
{

}

void GrassClass::InitGrass(const std::string& path, const Vec3 &pos, const float scale, int TEST_SIZE)
{
	//update
	if (!mGrassUpdate.Init())
	{
		assert("FAIL TO INIT PARTICLE UPDATE");
	}
	//render
	if (!mGrassRender.Init())
	{
		assert("FAIL TO INIT PARTICLE RENDER");
	}

	glGenTransformFeedbacks(1, &mTransformFeedbackBuffer);
	glGenQueries(1, &mQuery);

	glGenBuffers(2, mGrassBuffer);
	glGenVertexArrays(2, mVAO);

	GrassType tGrass;
	tGrass.mType = 0;
	unsigned int SizeOfGrass = sizeof(GrassType);

	for (int i = 0; i < 2; ++i)
	{
		glBindVertexArray(mVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mGrassBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, SizeOfGrass * TEST_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GrassType), &tGrass);

		for (int j = 0; j < 3; ++j)
			glEnableVertexAttribArray(j);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SizeOfGrass, (const GLvoid*)0);  // Position
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, SizeOfGrass, (const GLvoid*)12); // Size
		glVertexAttribPointer(2, 1, GL_INT,   GL_FALSE, SizeOfGrass, (const GLvoid*)16); // Type
	}
	
	TextureLoader_Inst->LoadTexture(path.c_str(), mGrassTexture);

	NumberOfGrass = TEST_SIZE;
	CurrentReadBuffer = 0;

	mGrassPosition = pos;
	mGrassSize = scale;
}

void GrassClass::RenderGrass(const Mat4& proj, const Mat4&view, const Vec3& q1, const Vec3& q2)
{
	glDisable(GL_RASTERIZER_DISCARD);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glDepthMask(GL_FALSE);
	//start render shader billboard
	mGrassRender.StartShaderProgram();
	mGrassRender.SetUniform("proj", proj);
	mGrassRender.SetUniform("view", view);
	mGrassRender.SetUniform("quad1", q1);
	mGrassRender.SetUniform("quad2", q2);
	static float intensity_val = 0;
	static float intensity = 1.0f;
	intensity_val += g_dt * intensity;
	intensity = (intensity_val > 1 * (intensity + 1)) ? -1.f : 1.f;

	mGrassRender.SetUniform("offset", intensity_val);
	//set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mGrassTexture.mTextureID);

	glBindVertexArray(mVAO[CurrentReadBuffer]);

	glDrawArrays(GL_POINTS, 0, NumberOfGrass);
	//glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void GrassClass::UpdateGrass()
{
	mGrassUpdate.StartShaderProgram();
	//position
	mGrassUpdate.SetUniform("Position", mGrassPosition);
	//scale
	mGrassUpdate.SetUniform("Scale", mGrassSize);
	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedbackBuffer);
	glBindVertexArray(mVAO[CurrentReadBuffer]);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mGrassBuffer[1 - CurrentReadBuffer]);

	//glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mQuery);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, NumberOfGrass);

	glEndTransformFeedback();

	//glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	//glGetQueryObjectiv(mQuery, GL_QUERY_RESULT, &NumberOfGrass);

	CurrentReadBuffer = 1 - CurrentReadBuffer;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	glDisable(GL_RASTERIZER_DISCARD);
}


