#include "model3d.h"

Texture2D* const Model3D::defaultTexture = new Texture2D("missingtexture2.tga");

Model3D::Model3D(const std::string &fileName) :
	Renderable(GL_TRIANGLES),
	specularity(0),
	glossiness(1),
	diffuseMap(defaultTexture),
	normalMap(nullptr),
	specularMap(nullptr)
{
	name = fileName.substr(0, fileName.rfind("."));

	std::vector<Vec3<float>> positions;
	std::vector<Vec3<float>> normals;
	std::vector<Vec2<float>> texCoords;
	std::vector<unsigned short> indices;

	if (!ImportModel(fileName, positions, normals, texCoords, indices))
	{
		Logger::Log(LogType::Error) << "Could not load model: " << fileName << "\n";

		return;
	}

	vao.AddBuffer(&positions[0], positions.size() * sizeof(positions[0]), 0, 3);
	vao.AddBuffer(&normals[0], normals.size() * sizeof(normals[0]), 1, 3);
	vao.AddBuffer(&texCoords[0], texCoords.size() * sizeof(texCoords[0]), 2, 2);
	vao.AddIndices(&indices[0], indices.size());
}

void Model3D::SetDiffuseTexture(const std::string &fileName)
{
	diffuseMap = new Texture2D(fileName);
}

void Model3D::Draw(Camera &camera, Mat4 &cameraMatrix)
{
	vao.Bind();
	shader->Bind();

	// Update shader matrices
	shader->SetUniformMat4("transformMatrix", GetTransformMatrix());
	shader->SetUniformMat4("viewMatrix", camera.viewMatrix);
	shader->SetUniformMat4("mvpMatrix", GetTransformMatrix() * cameraMatrix);

	// Update shader specular and gloss values
	shader->SetUniformFloat("specularity", specularity);
	shader->SetUniformFloat("glossiness", glossiness);

	diffuseMap->Bind(0);

	vao.DrawElements(renderMode);
}

bool Model3D::ImportModel(
	const std::string& filePath,
	std::vector<Vec3<float>> &in_positions,
	std::vector<Vec3<float>> &in_normals,
	std::vector<Vec2<float>> &in_texCoords,
	std::vector<unsigned short> &indices)
{
	std::ifstream file("res/models/" + filePath, std::ios::binary);
	std::string fileName = filePath.substr(filePath.rfind("/") + 1, filePath.length());

	if (!file.is_open())
	{
		Logger::Log(LogType::Error) << "Couldn't open " << fileName << "\n";
		return false;
	}

	char header[13];
	file.seekg(0, std::ios::beg);
	file.read(&header[0], 13);

	// TO-DO: Handle error
	if (file.fail())
	{
		Logger::Log(LogType::Error) << "Header could not be read in " << fileName << "\n";
		return false;
	}

	// BUG: memcpy sometimes copies 0x2400 into numIndices
	if (header[0] == 'M' && header[1] == 'M' && header[2] == 'F')
	{
		unsigned char errors;
		unsigned int  numVertices;
		unsigned int  numIndices;

		memcpy(&errors, &header[4], 1);
		memcpy(&numVertices, &header[5], 4);
		memcpy(&numIndices, &header[9], 4);

		file.seekg(13, std::ios::beg);

		in_positions.reserve(numVertices);
		in_normals.reserve(numVertices);
		in_texCoords.reserve(numVertices);
		indices.reserve(numIndices);

		GetVertex(file, in_positions, numVertices);
		GetVertex(file, in_normals, numVertices);
		GetUVs(file, in_texCoords, numVertices);
		GetIndices(file, indices, numIndices);

		file.close();

		return true;
	}
	else
	{
		Logger::Log(LogType::Error) << "Selected file is not an MMF file. Use obj-mmf.exe to convert a wavefront obj model file in to an appropriate MMF file.\n";
		file.close();

		return false;
	}
}

void Model3D::GetIndices(std::ifstream &file, std::vector<unsigned short> &index, unsigned int count)
{
	char test[2];
	unsigned short currentIndex;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 2);
		memcpy(&currentIndex, &test[0], sizeof(unsigned short));
		index.push_back(currentIndex);
	}
}

void Model3D::GetUVs(std::ifstream& file, std::vector<Vec2<float>> &uv, unsigned int count)
{
	char test[4];
	Vec2<float> currentUV;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 4);
		memcpy(&currentUV.x, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentUV.y, &test[0], sizeof(float));
		uv.push_back(currentUV);
	}
}

void Model3D::GetVertex(std::ifstream &file, std::vector<Vec3<float>> &vertex, unsigned int count)
{
	char test[4];
	Vec3<float> currentVertex;

	for (unsigned int i = 0; i < count; i++)
	{
		file.read(&test[0], 4);
		memcpy(&currentVertex.x, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentVertex.y, &test[0], sizeof(float));
		file.read(&test[0], 4);
		memcpy(&currentVertex.z, &test[0], sizeof(float));
		vertex.push_back(currentVertex);
	}
}