#include "TerrainHeight.h"


TerrainHeightMap::TerrainHeightMap()
{

}

TerrainHeightMap::~TerrainHeightMap()
{
	//delete heightmap data
	CleanUp();
}

void TerrainHeightMap::TerrainInit(const std::string& HeightMap, float normalizeHEIGHT_)
{
	//load height map
	LoadHeightMap(HeightMap);
	//normalize height map
	NormalizeHeightMap(normalizeHEIGHT_);
	//calc normal
	CalculateNormals();
	//calc UVs
	CalculateUV();
}

void TerrainHeightMap::CalculateUV()
{
	double tuCoord = 1.f / m_TerrainWidth;
	double tvCoord = 1.f / m_TerrainHeight;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (int j = 0; j<m_TerrainHeight; j++)
	{
		
		for (int i = 0; i < m_TerrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_HeightMap[(m_TerrainWidth * j) + i].UV.x = static_cast<float>(i * tuCoord);
			m_HeightMap[(m_TerrainWidth * j) + i].UV.y = static_cast<float>(j * tvCoord);
		}
	}
}

void TerrainHeightMap::LoadHeightMap(const std::string& file)
{
	int i, j, k, index, numComponents;
	unsigned char *imageData;
	unsigned char height;

	if (ReadImageFile(file.c_str(), &imageData, &m_TerrainWidth, &m_TerrainHeight, &numComponents) == 0)
	{
		std::cout << "Failed to read png!\n";   // color texture not available ...
	}

	// Create the structure to hold the height map dat.a
	m_HeightMap = new HeightMapData[m_TerrainWidth * m_TerrainHeight];

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j < m_TerrainHeight; ++j)
	{
		for (i = 0; i < m_TerrainWidth; ++i)
		{
			height = imageData[k];

			index = (m_TerrainWidth * j) + i;

			m_HeightMap[index].pos.x = -(float)i;
			m_HeightMap[index].pos.y = (float)height;
			m_HeightMap[index].pos.z = -(float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	free(imageData);
}

void TerrainHeightMap::NormalizeHeightMap(float normalizeHEIGHT_)
{
	int i, j;

	for (j = 0; j<m_TerrainHeight; j++)
	{
		for (i = 0; i<m_TerrainWidth; i++)
		{
			m_HeightMap[(m_TerrainWidth * j) + i].pos.y *= normalizeHEIGHT_;
		}
	}

}

void TerrainHeightMap::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	Vec3* normals;

	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new Vec3[(m_TerrainHeight - 1) * (m_TerrainWidth - 1)];
	if (!normals)
	{
		assert("FAIL TO ALLOCATE normal for height map");
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(m_TerrainHeight - 1); j++)
	{
		for (i = 0; i<(m_TerrainWidth - 1); i++)
		{
			index1 = (j * m_TerrainWidth) + i;
			index2 = (j * m_TerrainWidth) + (i + 1);
			index3 = ((j + 1) * m_TerrainWidth) + i;

			// Get three vertices from the face.
			vertex1[0] = m_HeightMap[index1].pos.x;
			vertex1[1] = m_HeightMap[index1].pos.y;
			vertex1[2] = m_HeightMap[index1].pos.z;
						   				
			vertex2[0] = m_HeightMap[index2].pos.x;
			vertex2[1] = m_HeightMap[index2].pos.y;
			vertex2[2] = m_HeightMap[index2].pos.z;
						   					 
			vertex3[0] = m_HeightMap[index3].pos.x;
			vertex3[1] = m_HeightMap[index3].pos.y;
			vertex3[2] = m_HeightMap[index3].pos.z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_TerrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<m_TerrainHeight; j++)
	{
		for (i = 0; i<m_TerrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TerrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (m_TerrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (m_TerrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (m_TerrainHeight - 1)))
			{
				index = (j * (m_TerrainWidth - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (m_TerrainWidth - 1)) && (j < (m_TerrainHeight - 1)))
			{
				index = (j * (m_TerrainWidth - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * m_TerrainWidth) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_HeightMap[index].nrm.x = (sum[0] / length);
			m_HeightMap[index].nrm.y = (sum[1] / length);
			m_HeightMap[index].nrm.z = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;
}

Vec2& TerrainHeightMap::GetDimension()
{
	return mVec2Dimension = Vec2(m_TerrainWidth, m_TerrainHeight);
}

HeightMapData* TerrainHeightMap::GetHeightData()const
{
	return m_HeightMap;
}

void TerrainHeightMap::CleanUp()
{
	if (m_HeightMap)
	{
		delete[] m_HeightMap;
		m_HeightMap = 0;
	}
}


