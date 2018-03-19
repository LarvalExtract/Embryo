#include "model3d.h"

Texture2D* const Model3D::defaultTexture = new Texture2D("missingtexture2.tga");

struct ObjFace
{
	int v[3];
	int vt[3];
	int vn[3];

	ObjFace() :v{ -1 }, vt{ -1 }, vn{ -1 } {}
};

struct Vertex
{
	Vec3<float> position;
	Vec2<float> texCoord;
	Vec3<float> normal;

	Vertex() {}

	Vertex(const Vec3<float> &position, const Vec2<float> &texCoord, const Vec3<float> &normal)
	{
		this->position = position;
		this->texCoord = texCoord;
		this->normal = normal;
	}
};

// Return a Vec3 of floats from a line in an OBJ file
static Vec3<float> GetVec3FromOBJLine(std::string &line);

// Return a Vec2 of floats from a line in an OBJ file
static Vec2<float> GetVec2FromOBJLine(std::string &line);

// Return an ObjFace from a line in an OBJ file
static ObjFace GetFaceFromOBJLine(std::string &line, unsigned int numUVs, unsigned int numNormals);

typedef std::vector< std::vector<Vec3<float>>* > vec3Lists;
typedef std::vector< std::vector<Vec2<float>>* > vec2Lists;
typedef std::vector< std::vector<ObjFace>* >	 objFaceLists;

// Generate un-indexed position, normal and texCoord arrays from the indices provided by OBJ file
// This is necessary because OBJ file indices cannot be used by OpenGL, thus new vertex arrays need to be created to later produce OpenGL friendly indices
void GenerateVertexArrays(
	vec3Lists &out_positions,
	vec3Lists &out_normals,
	vec2Lists &out_texCoords,
	const std::vector<Vec3<float>> &in_positions,
	const std::vector<Vec3<float>> &in_normals,
	const std::vector<Vec2<float>> &in_texCoords,
	const std::vector<ObjFace> &in_indexList);

// Compare two floats to determine if they are the same
bool Compare(float a, float b)
{
	bool result;
	(a - b < 0.0001f && a - b > -0.0001f) ? result = true : result = false;
	return result;
}

// Compare an input Vertex to map of vertices
// If similar, return the index. Otherwise, return false
bool GetSimilarVertexIndex(
	Vec3<float> &in_positions,
	Vec3<float> &in_normals,
	Vec2<float> &in_texCoords,
	std::vector<Vec3<float>> &out_positions,
	std::vector<Vec3<float>> &out_normals,
	std::vector<Vec2<float>> &out_texCoords,
	unsigned int &result);

// Iterate through an array of vertices to find similar vertices and generate an list of indices
void GenerateIndices(
	vec3Lists &in_positions,
	vec3Lists &in_normals,
	vec2Lists &in_texCoords,
	vec3Lists &out_positions,
	vec3Lists &out_normals,
	vec2Lists &out_texCoords,
	std::vector<std::vector<unsigned int>> &out_indices);

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
	std::vector<std::vector<unsigned int>> indices;

	if (!ImportOBJ(fileName, positions, normals, texCoords, indices))
	{
		Console::Log(LogType::Error) << "Could not load model: " << fileName << "\n";

		return;
	}

	vao.AddBuffer(&positions[0], positions.size() * sizeof(positions[0]), 0, 3);
	vao.AddBuffer(&normals[0], normals.size() * sizeof(normals[0]), 1, 3);
	vao.AddBuffer(&texCoords[0], texCoords.size() * sizeof(texCoords[0]), 2, 2);

	for (unsigned char i = 0; i < indices.size(); i++)
		vao.AddIndices(&indices[i][0], indices[i].size());
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
		Console::Log(LogType::Error) << "Couldn't open " << fileName << "\n";
		return false;
	}

	char header[13];
	file.seekg(0, std::ios::beg);
	file.read(&header[0], 13);

	// TO-DO: Handle error
	if (file.fail())
	{
		Console::Log(LogType::Error) << "Header could not be read in " << fileName << "\n";
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
		Console::Log(LogType::Error) << "Selected file is not an MMF file. Use obj-mmf.exe to convert a wavefront obj model file in to an appropriate MMF file.\n";
		file.close();

		return false;
	}
}

bool Model3D::ImportOBJ(
	const std::string &filePath,
	std::vector<Vec3<float>> &in_positions,
	std::vector<Vec3<float>> &in_normals,
	std::vector<Vec2<float>> &in_texCoords,
	std::vector<std::vector<unsigned int>> &out_indices
)
{
	std::ifstream objFile("res/models/two_boxes.obj");
	//name = filePath.substr(filePath.rfind("/") + 1, filePath.length());

	if (!objFile.is_open())
	{
		Console::Log(LogType::Error) << "Couldn't open file!";
		return false;
	}

	// Stored all vertex data in to unseperated vectors
	// This ignores the concept of individual objects in OBJ files and treats all the data in the OBJ file as one object
	std::vector<Vec3<float>>	temp_positions;
	std::vector<Vec2<float>>	temp_texCoords;
	std::vector<Vec3<float>>	temp_normals;

	// indexLists is a vector of vectors. This is to extract faces from the OBJ file as seperate objects dependant on lines beginning with o
	objFaceLists indexLists;

	// Buffer to store the current line in the obj file
	std::string curLine;

	// Iterate through OBJ file line-by-line to extract appropriate data
	while (getline(objFile, curLine))
	{
		// Only bother checking the line for data if the line is greater than n characters long
		if (curLine.length() > 5)
		{
			// If an O is found, push a new vector of ObjFaces on to the vector of vectors
			if (curLine.compare(0, 2, "o ") == 0)
				indexLists.push_back(new std::vector<ObjFace>);

			// Get the vertex positions from the current line in the OBJ file
			else if (curLine.compare(0, 2, "v ") == 0)
				temp_positions.push_back(GetVec3FromOBJLine(curLine));

			// Get the texture co-ordinates from the current line in the OBJ file
			else if (curLine.compare(0, 2, "vt") == 0)
				temp_texCoords.push_back(GetVec2FromOBJLine(curLine));

			// Get the vertex normals from the current line in the OBJ file
			else if (curLine.compare(0, 2, "vn") == 0)
				temp_normals.push_back(GetVec3FromOBJLine(curLine));

			// If an f is found, push the face data on to the last vector of ObjFaces added to the vector of indexLists when an o was last found
			else if (curLine.compare(0, 2, "f ") == 0)
				indexLists.back()->push_back(GetFaceFromOBJLine(curLine, temp_texCoords.size(), temp_normals.size()));
		}
	}

	// Vectors of vectors to store vertex data by seperate object
	vec3Lists new_positions;
	vec3Lists new_normals;
	vec2Lists new_texCoords;

	// The following algorithm requires the seperated lists of indices to generate multiple, un-indexed vertex arrays per object found in the OBJ file
	for (unsigned short i = 0; i < indexLists.size(); i++)
		GenerateVertexArrays(new_positions, new_normals, new_texCoords, temp_positions, temp_normals, temp_texCoords, *indexLists[i]);
	
	// Clear memory that is no longer needed
	temp_positions.clear();
	temp_normals.clear();
	temp_texCoords.clear();
	
	for (unsigned char i = 0; i < indexLists.size(); i++)
		delete indexLists[i];

	indexLists.clear();

	// It works up to here
	// TODO: Now that there are seperate vertex arrays per individual object, they need to be indexed...
	vec3Lists indexed_positions;
	vec3Lists indexed_normals;
	vec2Lists indexed_texCoords;

	// TODO: Stick all of the indexed_ vectors in to 1 final vertex buffer for OpenGL, but generate new index lists per object
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

Vec3<float> GetVec3FromOBJLine(std::string &line)
{	
	// Remove OBJ line identifier from line
	line.erase(0, 2);

	// Remove any left over leading whitespace
	while (line[0] == ' ')
		line.erase(0, 1);

	// Get float values from line
	float x = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float y = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float z = stof(line.substr(0, line.find('\r')));

	return Vec3<float>(x, y, z);
}

Vec2<float> GetVec2FromOBJLine(std::string &line)
{
	// Remove OBJ line identifier from line
	line.erase(0, 2);

	// Remove any left over leading whitespace
	while (line[0] == ' ')
		line.erase(0, 1);

	float x = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float y = stof(line.substr(0, line.find('\r')));

	return Vec2<float>(x, y);
}

ObjFace GetFaceFromOBJLine(std::string &line, unsigned int numUVs, unsigned int numNormals)
{
	// Remove OBJ line identifier from line
	line.erase(0, 2);

	// Check if the model is triangulated by getting the number of spaces from the current line
	char spaces = 0;
	for (char i = 0; i < line.length(); i++)
		if (line[i] == ' ')
			spaces++;

	if (spaces > 3)
		Console::Log(LogType::Error) << "OBJ model is not triangulated! Re-export the model with triangulate mesh enabled.";

	ObjFace face;

	// Extract face data from OBJ line
	for (char i = 0; i < 3; i++)
	{
		face.v[i] = stoi(line.substr(0, line.find('/'))) - 1;
		line.erase(0, line.find('/') + 1);

		if (numUVs > 0)
		{
			face.vt[i] = stoi(line.substr(0, line.find('/'))) - 1;
			line.erase(0, line.find('/') + 1);
		}

		if (numNormals > 0)
		{
			// If the model has normals but no texture co-ordinates, an extra / needs to be removed
			if (numUVs == 0)
				line.erase(0, 1);

			face.vn[i] = stoi(line.substr(0, line.find(' '))) - 1;
			line.erase(0, line.find(' ') + 1);
		}
	}

	return face;
}

void GenerateVertexArrays(
	vec3Lists &out_positions,
	vec3Lists &out_normals,
	vec2Lists &out_texCoords,
	const std::vector<Vec3<float>> &in_positions,
	const std::vector<Vec3<float>> &in_normals,
	const std::vector<Vec2<float>> &in_texCoords,
	const std::vector<ObjFace> &in_indexList)
{
	out_positions.push_back(new std::vector<Vec3<float>>);
	out_normals.push_back(new std::vector<Vec3<float>>);
	out_texCoords.push_back(new std::vector<Vec2<float>>);

	for (unsigned int i = 0; i < in_indexList.size(); i++)
	{
		for (char j = 0; j < 3; j++)
		{
			out_positions.back()->push_back(in_positions[in_indexList[i].v[j]]);
			out_normals.back()->push_back(in_normals[in_indexList[i].vn[j]]);
			out_texCoords.back()->push_back(in_texCoords[in_indexList[i].vt[j]]);
		}
	}	
}

bool GetSimilarVertexIndex(
	Vec3<float> &in_positions,
	Vec3<float> &in_normals,
	Vec2<float> &in_texCoords,
	std::vector<Vec3<float>> &out_positions,
	std::vector<Vec3<float>> &out_normals,
	std::vector<Vec2<float>> &out_texCoords,
	unsigned int &result
)
{
	// Iterate though out_vertices to find a similar vertex
	for (unsigned int i = 0; i < out_positions.size(); i++)
	{
		if (Compare(in_positions.x, out_positions[i].x) &&
			Compare(in_positions.y, out_positions[i].y) &&
			Compare(in_positions.z, out_positions[i].z) &&
			Compare(in_normals.x, out_normals[i].x) &&
			Compare(in_normals.y, out_normals[i].y) &&
			Compare(in_normals.z, out_normals[i].z) &&
			Compare(in_texCoords.x, out_texCoords[i].x) &&
			Compare(in_texCoords.y, out_texCoords[i].y))
		{
			// Similar vertex found, set result to i and return true
			result = i;
			return true;
		}
	}

	// Similar vertex not found, return false
	return false;
}

void GenerateIndices(
	vec3Lists &in_positions,
	vec3Lists &in_normals,
	vec2Lists &in_texCoords,
	vec3Lists &out_positions,
	vec3Lists &out_normals,
	vec2Lists &out_texCoords,
	std::vector<std::vector<unsigned int>> &out_indices
)
{
	unsigned int index;
	bool found;

	for (unsigned int j = 0; j < in_positions.size(); j++)
	{
		out_positions.push_back(new std::vector<Vec3<float>>);
		out_normals.push_back(new std::vector<Vec3<float>>);
		out_texCoords.push_back(new std::vector<Vec2<float>>);
		std::vector<unsigned int> test;
		out_indices.push_back(test);

		for (unsigned int i = 0; i < in_positions[j]->size(); i++)
		{
			found = GetSimilarVertexIndex(in_positions[j]->at(i), in_normals[j]->at(i), in_texCoords[j]->at(i), *out_positions.at(j), *out_normals.at(j), *out_texCoords.at(j), index);

			if (found == true)
			{
				out_indices.back().push_back(index);
			}

			else
			{
				out_positions.back()->push_back(in_positions[j]->at(i));
				out_normals.back()->push_back(in_normals[j]->at(i));
				out_texCoords.back()->push_back(in_texCoords[j]->at(i));

				if (j == 0)
					out_indices.back().push_back(out_positions[j]->size() - 1);
				else
					out_indices.back().push_back(out_positions[j - 1]->size() + out_positions[j]->size() - 1);
			}
		}
	}
}