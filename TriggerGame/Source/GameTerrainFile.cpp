#include "GameTerrainFile.h"
#include "ObjectMap.h"

//////////////////////////////////////////

GameTerrainData::GameTerrainData() :
index_x(0), index_z(0), scale(Vec3(1, 1, 1)), rotation(Vec3(0)), y_offset(0), modelIndex(-1), textureIndex(-1), objecttype(""), boundtype("")
{

}

GameTerrainData::GameTerrainData(int x, int y, const Vec3& scale, const Vec3& rotation,
	float y_offset, const std::string& modelname, const std::string& texturename,
	const std::string& objecttype, const std::string& boundtype) :
index_x(x), index_z(y), scale(scale), rotation(rotation), y_offset(y_offset),
modelIndex(OBJMAP_INST->GetIndexToModel(modelname)), textureIndex(OBJMAP_INST->GetIndexToTexture(texturename)),
objecttype(objecttype), boundtype(boundtype)
{

}

GameTerrainData::GameTerrainData(const GameTerrainData& rhs) :
index_x(rhs.index_x), index_z(rhs.index_z), scale(rhs.scale),
rotation(rhs.rotation), y_offset(rhs.y_offset), modelIndex(rhs.modelIndex), textureIndex(rhs.textureIndex),
objecttype(rhs.objecttype), boundtype(rhs.boundtype)
{

}

///////////////////////////////////////////


void TerrainSegment::Add(const GameTerrainData& gamedata)
{
	data.push_back(gamedata);
}
void TerrainSegment::Add(int x, int y, const Vec3& scale, const Vec3& rotation, float y_offset, const std::string& modelname, const std::string& texturename,
	const std::string& objecttype, const std::string& boundtype)
{
	GameTerrainData gamedata(x, y, scale, rotation, y_offset, modelname, texturename, objecttype, boundtype);
	Add(gamedata);
}

//load modelname and texturename as their indices - use mapping
TerrainSegment LoadGameTerrainDataFromFile(const std::string& filename)
{
	TerrainSegment segment;

	rapidjson::Document doc;
	GetJSONObject(&doc, filename);

	Scripter::GetString(&doc, "objectMap", &segment.objectmap_filename);
	Scripter::GetInt(&doc, "difficulty", &segment.difficulty);

	const rapidjson::Value& _array = doc["LevelObjects"];
	for (rapidjson::SizeType i = 0; i < _array.Size(); ++i)
	{
		GameTerrainData data;

		data.index_x = _array[i]["x"].GetInt();
		data.index_z = _array[i]["y"].GetInt();
		data.scale.x = static_cast<float>(_array[i]["scaleX"].GetDouble());
		data.scale.y = static_cast<float>(_array[i]["scaleY"].GetDouble());
		data.scale.z = static_cast<float>(_array[i]["scaleZ"].GetDouble());
		data.rotation.x = static_cast<float>(_array[i]["rotationX"].GetDouble());
		data.rotation.y = static_cast<float>(_array[i]["rotationY"].GetDouble());
		data.rotation.z = static_cast<float>(_array[i]["rotationZ"].GetDouble());

		std::string modelname, texturename;
		modelname = _array[i]["modelname"].GetString();
		texturename = _array[i]["texturename"].GetString();
		data.objecttype = _array[i]["objecttype"].GetString();
		data.boundtype = _array[i]["boundtype"].GetString();

		data.modelIndex = OBJMAP_INST->GetIndexToModel(modelname);
		data.textureIndex = OBJMAP_INST->GetIndexToTexture(texturename);

		segment.Add(data);
	}

	return segment;
}
//save modelname AND texturename as their strings - use mapping.
void SaveGameTerrainDataToFile(const std::string& filename, const TerrainSegment& segment)
{
	rapidjson::Document doc;
	Scripter::InitializeDocument(&doc);

	Scripter::Value _array;
	Scripter::InitializeArray(_array);

	Scripter::AppendStringToJSONObject(doc, "ObjectMap", segment.objectmap_filename.c_str(), &doc);
	rapidjson::Value jsondiff(segment.difficulty);
	doc.AddMember("difficulty", jsondiff, doc.GetAllocator());

	for (size_t i = 0; i < segment.data.size(); ++i)
	{
		rapidjson::Value currentobj;
		Scripter::InitializeObject(currentobj);

		const Vec3& scale = segment.data[i].scale;
		const Vec3& rotation = segment.data[i].rotation;

		Scripter::AppendToJSONObject<int>(currentobj, segment.data[i].index_x, "x", &doc);
		Scripter::AppendToJSONObject<int>(currentobj, segment.data[i].index_z, "y", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, scale.x, "scaleX", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, scale.y, "scaleY", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, scale.z, "scaleZ", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, rotation.x, "rotationX", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, rotation.y, "rotationY", &doc);
		Scripter::AppendToJSONObject<float>(currentobj, rotation.z, "rotationZ", &doc);

		Scripter::AppendStringToJSONObject(currentobj, "modelname", OBJMAP_INST->GetMappedObjectName(segment.data[i].modelIndex).c_str(), &doc);
		Scripter::AppendStringToJSONObject(currentobj, "texturename", OBJMAP_INST->GetMappedTextureName(segment.data[i].textureIndex).c_str(), &doc);
		Scripter::AppendStringToJSONObject(currentobj, "objecttype", segment.data[i].objecttype.c_str(), &doc);
		Scripter::AppendStringToJSONObject(currentobj, "boundtype", segment.data[i].boundtype.c_str(), &doc);

		_array.PushBack(currentobj, doc.GetAllocator());
	}
	doc.AddMember("LevelObjects", _array, doc.GetAllocator());
	Scripter::ExportDocument(&doc, filename.c_str());
}

void DataInitializeObject(ObjectInstance * instance, const GameTerrainData& data, TerrainHeightMap* heightmap)
{
	const std::string& obstaclename = data.objecttype;

	//set model name and stuff to the new objectinst.
	instance->SetModel(OBJMAP_INST->GetMappedObjectName(data.modelIndex));
	instance->SetTexture(OBJMAP_INST->GetMappedTextureName(data.textureIndex));
	//set scale
	Vec3 scale = data.scale;
	instance->SetScale(scale.x, scale.y, scale.z);
	//set rotations
	Vec3 rot = data.rotation;
	instance->SetPitch(rot.x);
	instance->SetYaw(rot.y);
	instance->SetRoll(rot.z);
	//set position.
	int index = (data.index_z * heightmap->GetDimension().x) + data.index_x;
	instance->SetPosition(Vec3(
		data.index_x * -1.f,
		data.y_offset + heightmap->GetHeightData()[index].pos.y,
		data.index_z * -1.f));
}