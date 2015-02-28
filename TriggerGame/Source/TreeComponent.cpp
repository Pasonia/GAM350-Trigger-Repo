#include "TreeComponent.h"
#include "ObjectInstance.h" //Object Instance functions
#include "mainGame.h"
#include <iostream>
#include "JSONWrapper.h"
#include "FolderManager.h"

Tree::Tree(ObjectInstance * parent) :
Component(parent)
{
	SetComponentID(CT_TREE);

	std::string filename;
	
	int randTree = rand() % 6;

	switch (randTree)
	{
	case 0:
		filename = ("../Resource/Trees/Tree2.json");
		break;
	case 1:
		filename = ("../Resource/Trees/Tree2.json");
		break;
	case 2:
		filename = ("../Resource/Trees/Tree3.json");
		break;
	case 3:
		filename = ("../Resource/Trees/Tree2.json");
		break;
	case 4:
		filename = ("../Resource/Trees/Tree3.json");
		break;
	case 5:
		filename = ("../Resource/Trees/Tree6.json");
		break;
	}
	

	/*rapidjson::Document doc;
	GetJSONObject(&doc, filename);

	ASSERT(doc["TrunkInstances"].IsArray(), "Error obtaining array of level objects");
	ASSERT(doc["LeaveInstances"].IsArray(), "Error obtaining array of level objects");
	const rapidjson::Value& _array = doc["TrunkInstances"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		Vec3 rotation, scale, position;
		ASSERT(_array[i].HasMember("rotation"), "Error reading rotation of current level object");
		ASSERT(_array[i].HasMember("scale"), "Error reading scale");
		ASSERT(_array[i].HasMember("position"), "Error reading position");

		rotation = Vec3((float)_array[i]["rotation"]["r"].GetDouble(),
			(float)_array[i]["rotation"]["p"].GetDouble(),
			(float)_array[i]["rotation"]["y"].GetDouble());

		scale = Vec3((float)_array[i]["scale"]["x"].GetDouble(),
			(float)_array[i]["scale"]["y"].GetDouble(),
			(float)_array[i]["scale"]["z"].GetDouble());

		position = Vec3((float)_array[i]["position"]["x"].GetDouble(),
			(float)_array[i]["position"]["y"].GetDouble(),
			(float)_array[i]["position"]["z"].GetDouble());

		Mat4 scaleM;
		Mat4 rotationM;
		Mat4 translationM;

		scaleM = Mat4(1.f);
		scaleM[0].x = scale.x;
		scaleM[1].y = scale.y;
		scaleM[2].z = scale.z;

		rotationM = glm::rotate(Mat4(1.0f),rotation.x, Vec3(0.f, 0.f, 1.f))*
			glm::rotate(Mat4(1.0f), rotation.y, Vec3(1.f, 0.f, 0.f)) *
			glm::rotate(Mat4(1.0f), -rotation.z, Vec3(0.f, 1.f, 0.f));


		translationM = glm::translate(position);

		Mat4 m_transform = translationM * rotationM * scaleM;

		trunks.push_back(TrunkInst(position, rotation, scale, m_transform));
	}*/

	/*const rapidjson::Value& _array_2 = doc["LeaveInstances"];
	for (rapidjson::SizeType i = 0; i < _array_2.Size(); ++i)
	{
		Vec3 rotation, scale, position;
		ASSERT(_array_2[i].HasMember("rotation"), "Error reading rotation of current level object");
		ASSERT(_array_2[i].HasMember("scale"), "Error reading scale");
		ASSERT(_array_2[i].HasMember("position"), "Error reading position");

		rotation = Vec3((float)_array_2[i]["rotation"]["r"].GetDouble(),
			(float)_array_2[i]["rotation"]["p"].GetDouble(),
			(float)_array_2[i]["rotation"]["y"].GetDouble());

		scale = Vec3((float)_array_2[i]["scale"]["x"].GetDouble(),
			(float)_array_2[i]["scale"]["y"].GetDouble(),
			(float)_array_2[i]["scale"]["z"].GetDouble());

		position = Vec3((float)_array_2[i]["position"]["x"].GetDouble(),
			(float)_array_2[i]["position"]["y"].GetDouble(),
			(float)_array_2[i]["position"]["z"].GetDouble());

		Mat4 scaleM;
		Mat4 rotationM;
		Mat4 translationM;

		scaleM = Mat4(1.f);
		scaleM[0].x = scale.x;
		scaleM[1].y = scale.y;
		scaleM[2].z = scale.z;

		rotationM = glm::rotate(Mat4(1.0f), rotation.x, Vec3(0.f, 0.f, 1.f))*
			glm::rotate(Mat4(1.0f), rotation.y, Vec3(1.f, 0.f, 0.f)) *
			glm::rotate(Mat4(1.0f), -rotation.z, Vec3(0.f, 1.f, 0.f));


		translationM = glm::translate(position);

		Mat4 m_transform = translationM * rotationM * scaleM;

		leaves.push_back(TrunkInst(position, rotation, scale, m_transform));
	}*/

	{
		Vec3 rotation, scale, position;

		Mat4 scaleM;
		Mat4 rotationM;
		Mat4 translationM;

		scaleM = Mat4(1.f);
		scaleM[0].x = scale.x = .2f;
		scaleM[1].y = scale.y = 5.f;
		scaleM[2].z = scale.z = .2f;

		rotationM = glm::rotate(Mat4(1.0f), rotation.x, Vec3(0.f, 0.f, 1.f))*
			glm::rotate(Mat4(1.0f), rotation.y, Vec3(1.f, 0.f, 0.f)) *
			glm::rotate(Mat4(1.0f), -rotation.z, Vec3(0.f, 1.f, 0.f));

		translationM = glm::translate(Vec3(0));
		Mat4 m_transform = translationM * rotationM * scaleM;
		trunks.push_back(TrunkInst(position, rotation, scale, m_transform));
	}

	float angleStep = 360 / 4;
	float radius = 0.02f;
	Vec3 rotation, scale, position;
	rotation.x = PI / 2 * 0.6f;
	rotation.y = -PI / 2;
	//rotation.y = degreesToRadians(-i * angleStep);
	Mat4 scaleM;
	Mat4 rotationM;
	Mat4 translationM;

	for (int j = 0; j < 7; ++j)
	{
		scaleM = Mat4(1.f);
		scaleM[0].x = .06f;
		scaleM[1].y = .06f;
		scaleM[2].z = .2f;

		Mat4 m_transform; Mat4 rot;

		angleStep = 360 / 6;
		

		for (int i = 0; i < 6; ++i)
		{
			int randRadius = rand() % 2;
			int randPitch = rand() % 10;
			randPitch -= 5;

			rotation.x = PI / 2 * 0.5f + degreesToRadians(randPitch);
			rotation.y = -PI / 2;

			glm::quat rotQ(rotation);
			rotationM = glm::toMat4(rotQ);

			position = Vec3(1.3f - j * 0.2f + randRadius/10.f, 2.5f + j * 0.5f, 0.f);
			translationM = glm::translate(position);
			m_transform = translationM * rotationM * scaleM;
			int randRot = rand() % 20;
			randRot -= 10;
			rot = glm::rotate(Mat4(1), degreesToRadians(angleStep * i + randRot) + PI/2, Vec3(0, 1, 0));
			leaves.push_back(TrunkInst(position, rotation, scale, rot * m_transform));
		}

		angleStep = 360 / 6;
		
		for (int i = 0; i < 6; ++i)
		{
			int randPitch = rand() % 10;
			randPitch -= 5;
			rotation.x = PI / 2 * 0.6f + degreesToRadians(randPitch);
			rotation.y = -PI / 2;

			glm::quat rotQ(rotation);
			rotationM = glm::toMat4(rotQ);

			int randRadius = rand() % 2;
			position = Vec3(1.5f - j * 0.2f + randRadius / 10.f, 2.6f + j * 0.5f, 0.f);
			translationM = glm::translate(position);
			m_transform = translationM * rotationM * scaleM;
			int randRot = rand() % 20;
			randRot -= 10;
			rot = glm::rotate(Mat4(1), degreesToRadians(angleStep * i + randRot), Vec3(0, 1, 0));
			leaves.push_back(TrunkInst(position, rotation, scale, rot * m_transform));
		}
	}
	
}
Tree::~Tree()
{

}
void Tree::Init() //must be defined
{
	Mat4 worldTransl = glm::translate(m_parent->GetPosition());
	Mat4 worldScale = Mat4(1.f);
	worldScale[0].x = m_parent->GetScale().x;
	worldScale[1].y = m_parent->GetScale().y;
	worldScale[2].z = m_parent->GetScale().z;


	Mat4 normScale = Mat4(1.f);
	normScale[0].x = 1 / trunks[0].scale.x;
	normScale[1].y = 1 / trunks[0].scale.y;
	normScale[2].z = 1 / trunks[0].scale.z;

	Mat4 worldRot = glm::rotate(Mat4(1.0f), m_parent->GetRoll(), Vec3(0.f, 0.f, 1.f))*
	glm::rotate(Mat4(1.0f), m_parent->GetPitch(), Vec3(1.f, 0.f, 0.f)) *
	glm::rotate(Mat4(1.0f), -m_parent->GetYaw(), Vec3(0.f, 1.f, 0.f));

	worldMat = worldTransl * worldRot * worldScale * normScale;
}
void Tree::Update() //must be defined
{
	//m_parent->Update();
}
void Tree::Free() //must be defined
{
}

void Tree::ForwardRender()
{

	Texture* m_leaves_texture = TextureManager_Inst->GetTexture("Leaves");
	Model * m_leaf = ModelManager_Inst->GetModelData("Plane");
	Mat4 rot;
	Mat4 transl;
	Mat4 scale;
	float angleStep = 360 / 6;
		for (unsigned i = 0; i < leaves.size(); ++i)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_leaf->ForwardRender(worldMat * leaves[i].transf, m_leaves_texture, Vec2(1, 1), 0, 0, false);
			glDisable(GL_BLEND);
		}
	
}

void Tree::Render()
{
	Model *  m_model = ModelManager_Inst->GetModelData("Trunk");
	Texture * m_texture = TextureManager_Inst->GetTexture("Trunk");
	Model*   m_leave_model = ModelManager_Inst->GetModelData("Sphere");
	for (unsigned i = 0; i < 1; ++i)
	{
		m_model->Render(worldMat * trunks[i].transf, m_texture, 0, 0, 0, Vec2(.5, .5));
	//	m_leave_model->Render(worldMat * trunks[i].transf, m_leaves_texture, 0, 0, 0, Vec2(.5, .5));
	}


}
