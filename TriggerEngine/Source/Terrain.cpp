#include "terrain.h"


TerrainHeightMap::TerrainHeightMap()
{

}

TerrainHeightMap::~TerrainHeightMap()
{

}

void TerrainHeightMap::TerrainInit(const std::string& file)
{
	//load height map
	LoadHeightMap(file);
	//normalize height map
	NormalizeHeightMap();
	//calc normal
	CalculateNormals();
}

void TerrainHeightMap::LoadHeightMap(const std::string& file)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, file.c_str(), "rb");
	if (error != 0)
	{
		assert("FAILED TO OPEN HEIGHT MAP FILE");
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		assert("FAILED TO OPEN HEIGHT MAP FILE HEADER");
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		assert("FAILED TO OPEN HEIGHT MAP FILE HEADER INFO");
	}

	// Save the dimensions of the terrain.
	m_TerrainWidth = bitmapInfoHeader.biWidth;
	m_TerrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = m_TerrainWidth * m_TerrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		assert("FAILED TO OPEN HEIGHT MAP FILE DATA ALLOCATION");
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		assert("FAILED TO OPEN HEIGHT MAP FILE DATA INFO");
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		assert("FAILED TO CLSOE FILE");
	}

	// Create the structure to hold the height map data.
	m_HeightMap = new Vec3[m_TerrainWidth * m_TerrainHeight];
	if (!m_HeightMap)
	{
		assert("HEIGHT MAP FAILED TO ALLOCATED");
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<m_TerrainHeight; j++)
	{
		for (i = 0; i<m_TerrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (m_TerrainHeight * j) + i;

			m_HeightMap[index].x = (float)i;
			m_HeightMap[index].y = (float)height;
			m_HeightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;
}

void TerrainHeightMap::NormalizeHeightMap()
{
	int i, j;

	for (j = 0; j<m_TerrainHeight; j++)
	{
		for (i = 0; i<m_TerrainWidth; i++)
		{
			m_HeightMap[(m_TerrainHeight * j) + i].y /= 15.0f;
		}
	}

}

void TerrainHeightMap::CalculateNormals()
{
	//int i, j, index1, index2, index3, index, count;
	//float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	////VectorType* normals;


	//// Create a temporary array to hold the un-normalized normal vectors.
	//normals = new VectorType[(m_terrainHeight - 1) * (m_terrainWidth - 1)];
	//if (!normals)
	//{
	//	return false;
	//}

	//// Go through all the faces in the mesh and calculate their normals.
	//for (j = 0; j<(m_TerrainHeight - 1); j++)
	//{
	//	for (i = 0; i<(m_TerrainWidth - 1); i++)
	//	{
	//		index1 = (j * m_TerrainHeight) + i;
	//		index2 = (j * m_TerrainHeight) + (i + 1);
	//		index3 = ((j + 1) * m_TerrainHeight) + i;

	//		// Get three vertices from the face.
	//		vertex1[0] = m_HeightMap[index1].x;
	//		vertex1[1] = m_HeightMap[index1].y;
	//		vertex1[2] = m_HeightMap[index1].z;
	//					   
	//		vertex2[0] = m_HeightMap[index2].x;
	//		vertex2[1] = m_HeightMap[index2].y;
	//		vertex2[2] = m_HeightMap[index2].z;
	//					   
	//		vertex3[0] = m_HeightMap[index3].x;
	//		vertex3[1] = m_HeightMap[index3].y;
	//		vertex3[2] = m_HeightMap[index3].z;

	//		// Calculate the two vectors for this face.
	//		vector1[0] = vertex1[0] - vertex3[0];
	//		vector1[1] = vertex1[1] - vertex3[1];
	//		vector1[2] = vertex1[2] - vertex3[2];
	//		vector2[0] = vertex3[0] - vertex2[0];
	//		vector2[1] = vertex3[1] - vertex2[1];
	//		vector2[2] = vertex3[2] - vertex2[2];

	//		index = (j * (m_TerrainHeight - 1)) + i;

	//		// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
	//		normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
	//		normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
	//		normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
	//	}
	//}

	//// Now go through all the vertices and take an average of each face normal 	
	//// that the vertex touches to get the averaged normal for that vertex.
	//for (j = 0; j<m_TerrainHeight; j++)
	//{
	//	for (i = 0; i<m_TerrainWidth; i++)
	//	{
	//		// Initialize the sum.
	//		sum[0] = 0.0f;
	//		sum[1] = 0.0f;
	//		sum[2] = 0.0f;

	//		// Initialize the count.
	//		count = 0;

	//		// Bottom left face.
	//		if (((i - 1) >= 0) && ((j - 1) >= 0))
	//		{
	//			index = ((j - 1) * (m_TerrainHeight - 1)) + (i - 1);

	//			sum[0] += normals[index].x;
	//			sum[1] += normals[index].y;
	//			sum[2] += normals[index].z;
	//			count++;
	//		}

	//		// Bottom right face.
	//		if ((i < (m_TerrainWidth - 1)) && ((j - 1) >= 0))
	//		{
	//			index = ((j - 1) * (m_TerrainHeight - 1)) + i;

	//			sum[0] += normals[index].x;
	//			sum[1] += normals[index].y;
	//			sum[2] += normals[index].z;
	//			count++;
	//		}

	//		// Upper left face.
	//		if (((i - 1) >= 0) && (j < (m_TerrainHeight - 1)))
	//		{
	//			index = (j * (m_TerrainHeight - 1)) + (i - 1);

	//			sum[0] += normals[index].x;
	//			sum[1] += normals[index].y;
	//			sum[2] += normals[index].z;
	//			count++;
	//		}

	//		// Upper right face.
	//		if ((i < (m_TerrainWidth - 1)) && (j < (m_TerrainHeight - 1)))
	//		{
	//			index = (j * (m_TerrainHeight - 1)) + i;

	//			sum[0] += normals[index].x;
	//			sum[1] += normals[index].y;
	//			sum[2] += normals[index].z;
	//			count++;
	//		}

	//		// Take the average of the faces touching this vertex.
	//		sum[0] = (sum[0] / (float)count);
	//		sum[1] = (sum[1] / (float)count);
	//		sum[2] = (sum[2] / (float)count);

	//		// Calculate the length of this normal.
	//		length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

	//		// Get an index to the vertex location in the height map array.
	//		index = (j * m_TerrainHeight) + i;

	//		// Normalize the final shared normal for this vertex and store it in the height map array.
	//		m_HeightMap[index].nx = (sum[0] / length);
	//		m_HeightMap[index].ny = (sum[1] / length);
	//		m_HeightMap[index].nz = (sum[2] / length);
	//	}
	//}

	//// Release the temporary normals.
	//delete[] normals;
	//normals = 0;

}

void TerrainHeightMap::CleanUp()
{
	if (m_HeightMap)
	{
		delete[] m_HeightMap;
		m_HeightMap = 0;
	}
}


