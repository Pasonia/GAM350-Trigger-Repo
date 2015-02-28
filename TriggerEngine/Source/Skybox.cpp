/******************************************************************************/
/*!
\file Skybox.cpp
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

#include "Skybox.h"

SkyBox* SkyBox::mInstance = 0;


SkyBox* SkyBox::Inst()
{
	mInstance = mInstance ? mInstance : new SkyBox();
	return mInstance;
}

void SkyBox::Destroy()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = 0;
	}
}

void SkyBox::CreateSkyBox()
{
	//create Skybox ID
	TriggerShader Vertexskybox, Fragskybox;
	Vertexskybox.LoadShader("../Resource/Shader/VertShader/Skybox_vs.shader", GL_VERTEX_SHADER);
	  Fragskybox.LoadShader("../Resource/Shader/FragShader/Skybox_fs.shader", GL_FRAGMENT_SHADER);

	skyboxProg.CreateProgram();
	skyboxProg.AddShaderToProgram(&Vertexskybox);
	skyboxProg.AddShaderToProgram(&Fragskybox);
	skyboxProg.LinkProgram();

	//location for view
	ModelMatLoc = glGetUniformLocation(skyboxProg.GetProgramID(), "modelMTX");
	viewMatLoc = glGetUniformLocation(skyboxProg.GetProgramID(), "viewMTX");
	ProjMatLoc = glGetUniformLocation(skyboxProg.GetProgramID(), "projMTX");
	time_step = glGetUniformLocation(skyboxProg.GetProgramID(), "time");
	mState = glGetUniformLocation(skyboxProg.GetProgramID(), "state");
	//read image file from the resource
	LoadCubeMap();
}

unsigned char* ComputerFaceBlock(int Row, int Col, int imageW, int FaceW, unsigned char *data, int component = 4)
{
	//total length per 1 face
	int LengthBlock = FaceW * FaceW * component * sizeof(unsigned char);
	//get the Row image
	int offsetRow = Row * FaceW * component * imageW;
	//get the Col image
	int offsetCol = Col * FaceW * component;

	unsigned char *FaceBlock = new unsigned char[LengthBlock];

	for (int i = 0; i < FaceW; ++i)
	{
		for (int j = 0; j < FaceW; ++j)
		{
			//R
			FaceBlock[(i * FaceW + j) * component + 2] = data[offsetRow + offsetCol + (i * imageW + j) * component + 0];
			//G
			FaceBlock[(i * FaceW + j) * component + 1] = data[offsetRow + offsetCol + (i * imageW + j) * component + 1];
			//B
			FaceBlock[(i * FaceW + j) * component + 0] = data[offsetRow + offsetCol + (i * imageW + j) * component + 2];
		}
	}
	std::reverse(FaceBlock, FaceBlock + LengthBlock);
	return FaceBlock;
}

void SkyBox::LoadCubeMap()
{
	unsigned char *imageData[3];

	int imageWidth, imageHeight, numComponents;

	/* Read cube texture */
	if (ReadImageFile("../Resource/image/DAY.png", &imageData[0], &imageWidth, &imageHeight, &numComponents) == 0)
	{
		std::cout << "Failed to read skybox\n";   // cube texture not available ...
		exit(2);
	}
	if (ReadImageFile("../Resource/image/DAWN_DUSK.png", &imageData[1], &imageWidth, &imageHeight, &numComponents) == 0)
	{
		std::cout << "Failed to read skybox\n";   // cube texture not available ...
		exit(2);
	}
	if (ReadImageFile("../Resource/image/NIGHT.png", &imageData[2], &imageWidth, &imageHeight, &numComponents) == 0)
	{
		std::cout << "Failed to read skybox\n";   // cube texture not available ...
		exit(2);
	}

	int FaceWidth = 256, FaceHeight = 256;

	unsigned char *PosX[3];
	unsigned char *NegX[3];
	unsigned char *PosY[3];
	unsigned char *NegY[3];
	unsigned char *PosZ[3];
	unsigned char *NegZ[3];

	for (int i = 0; i < 3; ++i)
	{
		PosX[i] = ComputerFaceBlock(1, 0, imageWidth, FaceWidth, imageData[i], numComponents);
		NegX[i] = ComputerFaceBlock(1, 2, imageWidth, FaceWidth, imageData[i], numComponents);
		PosY[i] = ComputerFaceBlock(2, 1, imageWidth, FaceWidth, imageData[i], numComponents);
		NegY[i] = ComputerFaceBlock(0, 1, imageWidth, FaceWidth, imageData[i], numComponents);
		PosZ[i] = ComputerFaceBlock(1, 1, imageWidth, FaceWidth, imageData[i], numComponents);
		NegZ[i] = ComputerFaceBlock(1, 3, imageWidth, FaceWidth, imageData[i], numComponents);

		glGenTextures(1, &cubeTexID[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexID[i]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosX[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegX[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosY[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegY[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, PosZ[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, FaceWidth, FaceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NegZ[i]);

		//enable the clamp for wrap and trilinear interpolation
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//under the standard filtering rules for cubemaps, filtering does not work across
		//faces of the cubemap. This results in a seam across the faces of a cubemap. 
		//This was a hardware limitation in the past, but modern hardware is capable
		//of interpolating across a cube face boundary.
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		free(imageData[i]);
		free(PosX[i]);
		free(NegX[i]);
		free(PosY[i]);
		free(NegY[i]);
		free(PosZ[i]);
		free(NegZ[i]);
	}
}

void SkyBox::Render(const Mat4 &modelMat, const Mat4 &viewMat, const Mat4 &projMat, float id, float State)
{
	//skybox	
	skyboxProg.UseProgram();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexID[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexID[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexID[2]);

	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));
	glUniform1fv(time_step, 1, &id);
	glUniform1fv(mState, 1, &State);
	
	//auto err = glGetError();
	//std::cout << err << std::endl;
	//i = glGetError();
	
}

SkyBox::SkyBox()
{
	CreateSkyBox();
}

SkyBox::~SkyBox()
{

}

