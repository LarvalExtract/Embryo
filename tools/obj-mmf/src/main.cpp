#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer() : start(std::chrono::high_resolution_clock::now()) {};

	void Reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	double Elapsed() const
	{
		return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(std::chrono::high_resolution_clock::now() - start).count();	
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

struct vec3
{
	float x, y, z;

	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct vec2
{
	float x, y;

	vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	vec2(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}
};

struct face
{
	unsigned short v, vt, vn;

	face(const unsigned short& v, const unsigned short& vt, const unsigned short& vn)
	{
		this->v  = v;
		this->vt = vn;
		this->vn = vt;
	}
};

struct PackedVertex
{
	vec3 positions;
	vec2 texCoords;
	vec3 normals;

	PackedVertex()
	{
		positions = vec3(0, 0, 0);
		texCoords = vec2(0, 0);
		normals = vec3(0, 0, 0);
	}

	PackedVertex(const vec3& positions, const vec2& texCoords, const vec3& normals)
	{
		this->positions = positions;
		this->texCoords = texCoords;
		this->normals	= normals;
	}
};

vec2 GetUVs(std::string line)
{
	line.erase(0, 2);

	while (line.at(0) == ' ')
		line.erase(0, 1);

	float x = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float y = stof(line.substr(0, line.find('\r')));

	return vec2(x, y);
}

vec3 GetVertices(std::string line)
{
	line.erase(0, 2);

	while (line.at(0) == ' ')
		line.erase(0, 1);

	float x = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float y = stof(line.substr(0, line.find(' ')));
	line.erase(0, line.find(' ') + 1);

	float z = stof(line.substr(0, line.find('\r')));

	return vec3(x, y, z);
}

void GetIndices(
	std::string line,
	std::vector<face>& in_faces,
	const unsigned int& vnCount,
	const unsigned int& vtCount)
{
	line.erase(0, 2);

	// Check if the model is triangulated
	char spaces = 0;

	for (int i = 0; i < line.length(); i++)
		if (line.at(i) == ' ')
			spaces++;

	if (spaces > 3)
		std::cout << "Error: Mesh is not triangulated! Re-export model with 'triangulate mesh' enabled." << std::endl;

	unsigned short v, vn, vt = 0;

	// Add indices from obj file to index lists
	for (char i = 0; i < 3; i++)
	{
		v = stoi(line.substr(0, line.find('/'))) -1;
		line.erase(0, line.find('/') + 1);

		if (vtCount > 0)
		{
			vt = stoi(line.substr(0, line.find('/'))) -1;
			line.erase(0, line.find('/') + 1);
		}

		if (vnCount > 0)
		{
			if (vtCount == 0)
				line.erase(0, 1);

			vn = stoi(line.substr(0, line.find(' '))) -1;
			line.erase(0, line.find(' ') + 1);
		}

		in_faces.push_back(face(v, vn, vt));
	}
}

bool Compare(float a, float b)
{
	if (a - b < 0.0001f && a - b > -0.0001f)
		return true;

	return false;
}

bool GetSimilarVertex(
	vec3& in_position,
	vec3& in_normal,
	vec2& in_texCoord,
	//std::vector<vec3>& out_positions,
	//std::vector<vec3>& out_normals,
	//std::vector<vec2>& out_texCoords,
	std::vector<PackedVertex>& out_vertices,
	unsigned short& result)
{
	for (unsigned int i = 0; i < out_vertices.size(); i++)
	{
		if (Compare(in_position.x, out_vertices[i].positions.x) &&
			Compare(in_position.y, out_vertices[i].positions.y) &&
			Compare(in_position.z, out_vertices[i].positions.z) &&
			Compare(in_normal.x,   out_vertices[i].normals.x) &&
			Compare(in_normal.y,   out_vertices[i].normals.y) &&
			Compare(in_normal.z,   out_vertices[i].normals.z) &&
			Compare(in_texCoord.x, out_vertices[i].texCoords.x) &&
			Compare(in_texCoord.y, out_vertices[i].texCoords.y))
		{
			result = i;
			return true;
		}
	}
	return false;
}

std::vector<PackedVertex> CreateVertexArray(
	std::vector<std::vector<PackedVertex>>& out_vertexArrays,
	const unsigned char& numObjects,
	const std::vector<vec3>& in_positions,
	const std::vector<vec2>& in_texCoords,
	const std::vector<vec3>& in_normals,
	const std::vector<std::vector<face>>& faces)
{
	std::cout << "Generating vertex arrays...";
	Timer timer;

	std::vector<PackedVertex> vertices;

	for (unsigned char j = 0; j < faces.size(); j++)
	{
		for (unsigned int i = 0; i < faces[j].size(); i++)
		{
			//unsigned short posIndex = faces[i].v;
			//unsigned short uvIndex = faces[i].vt;
			//unsigned short normIndex = faces[i].vn;
			//
			//vec3 position = temp_positions[posIndex];
			//vec2 texCoord = temp_texCoords[uvIndex];
			//vec3 normal = temp_normals[normIndex];
			//
			//in_positions.push_back(position);
			//in_texCoords.push_back(texCoord);
			//in_normals.push_back(normal);
			vertices.push_back(PackedVertex(
				in_positions[faces[j][i].v],
				in_texCoords[faces[j][i].vt],
				in_normals	[faces[j][i].vn]));
		}

		out_vertexArrays.push_back(vertices);
		vertices.clear();
	}

	std::cout << " Done! (" << timer.Elapsed() << " seconds)\n\n";
	return vertices;
}

//Painfully slow
void IndexVBO(
	//std::vector<vec3>& in_positions,
	//std::vector<vec3>& in_normals,
	//std::vector<vec2>& in_texCoords,
	//std::vector<vec3>& out_positions,
	//std::vector<vec3>& out_normals,
	//std::vector<vec2>& out_texCoords,
	std::vector<std::vector<PackedVertex>>& in_vertexArrays,
	std::vector<std::vector<PackedVertex>>& out_vertexArrays,
	//const std::vector<std::vector<face>>& in_faces,
	std::vector<std::vector<unsigned short>>& out_faces)
{
	std::cout << "Indexing vertex arrays...";
	Timer timer;

	unsigned short index;
	std::vector<unsigned short> out_indices;
	bool found;

	for (unsigned int j = 0; j < in_vertexArrays.size(); j++)
	{
		for (unsigned int i = 0; i < in_vertexArrays[j].size(); i++)
		{
			found = GetSimilarVertex(
				in_vertexArrays[j][i].positions, 
				in_vertexArrays[j][i].normals, 
				in_vertexArrays[j][i].texCoords, 
				out_vertexArrays[j],
				//out_vertexArrays[j],
				//out_vertexArrays[j],
				index);

			if (found)
			{
				out_indices.push_back(index);
			}

			else
			{
				//out_positions.push_back(in_vertexArrays[j][i].positions);
				//out_normals.push_back(in_vertexArrays[j][i].normals);
				//out_texCoords.push_back(in_vertexArrays[j][i].texCoords);
				out_vertexArrays.push_back(in_vertexArrays[i]);
				out_indices.push_back((unsigned short)out_vertexArrays[i].size() - 1);
			}
		}
		out_faces.push_back(out_indices);
		out_indices.clear();
	}

	std::cout << " Done! (" << timer.Elapsed() << " seconds)\n\n";
}

// TODO: Store vertex data is ints * 1000 and simply divide them when reimporting to Embryo for accuracy
int main()//int argc, char* argv[])
{
	std::cout << "Wavefront obj to \"My Model Format\" converter. Converts obj vertex data in to OpenGL friendly vertex data.\n"
			  << "Version 0.0. (NOTE: Only works with complete obj files; obj files that are missing normal OR texture co-ordinate data will crash!)\n\n";

	std::ifstream objFile("D:\\Documents\\Visual Studio 2017\\Projects\\Embryo\\resources\\models\\shapes_seperated.obj");
	//std::ifstream objFile(argv[1]);

	if (!objFile.is_open())
	{
		std::cout << "Error: File could not be found/opened. If the directory contains spaces, use quotes." << std::endl;
		std::cin.get();
		return -1;
	}

	std::string name = "D:\\Documents\\Visual Studio 2017\\Projects\\Embryo\\resources\\models\\shapes_seperated.obj";
	//std::string name = argv[1];
	std::string inFileName = name.substr(name.rfind("\\") + 1, name.length());

	std::string line;

	std::cout << "Extracting vertex data from " << inFileName << "...";
	Timer timer;

	std::vector<vec3> temp_positions;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_texCoords;
	std::vector<face> temp_faces;
	std::vector<unsigned int> objOffsets;

	objOffsets.push_back(0);

	while (getline(objFile, line))
	{
		if (line.length() != 0)
		{
			if (line.compare(0, 2, "v ") == 0)
				temp_positions.push_back(GetVertices(line));

			if (line.compare(0, 2, "vn") == 0)
				temp_normals.push_back(GetVertices(line));

			if (line.compare(0, 2, "vt") == 0)
				temp_texCoords.push_back(GetUVs(line));

			if (line.compare(0, 2, "o ") == 0)
				objOffsets.push_back(temp_positions.size());

			if (line.compare(0, 2, "f ") == 0)
				GetIndices(line, temp_faces, temp_normals.size(), temp_texCoords.size());
		}
	}

	std::cout << " Done! (" << timer.Elapsed() << " seconds)\n" << std::endl;
	objFile.close();

	std::vector<std::vector<face>> indexLists;
	indexLists.reserve(objOffsets.size());

	std::vector<face> cur_face;

	// Split the full list of indices in to seperate index lists per object "o" found in the obj file
	for (unsigned char i = 0; i < objOffsets.size() - 1; i++)
	{	
		for (unsigned int j = 0; j < temp_faces.size(); j++)
		{
			if (temp_faces[j].v >= objOffsets[i] && temp_faces[j].v < objOffsets[i + 1])
				cur_face.push_back(temp_faces[j]);

		}

		indexLists.push_back(cur_face);
		cur_face.clear();
	}

	//temp_faces.clear();

	//std::vector<vec3> in_positions;
	//std::vector<vec2> in_texCoords;
	//std::vector<vec3> in_normals;

	std::vector<std::vector<PackedVertex>> vertexArrays;
	CreateVertexArray(vertexArrays, objOffsets.size(), temp_positions, temp_texCoords, temp_normals, indexLists);

	/*std::vector<vec3> indexed_positions;
	std::vector<vec3> indexed_normals;
	std::vector<vec2> indexed_texCoords;*/

	std::vector<std::vector<unsigned short>> out_faces;
	std::vector<std::vector<PackedVertex>> out_vertexArrays;

	IndexVBO(vertexArrays, out_vertexArrays, out_faces);

	std::string outFileName = inFileName.erase(inFileName.length() - 4, 4) + ".mmf";
	std::ofstream output(outFileName, std::ios::out | std::ios::beg | std::ios::binary);

	//std::cout << "Writing to " << outFileName << "...";

	//char fileID[4] = { 'M', 'M', 'F', '.' };	// The first 4 bytes of the output file

	//char errors = 0;	// This byte represents an error code.
	//					// If normal data could not be found in the original .obj file, this number will be 1.
	//					// If texture co-ordinate data could not be found, this number will be 2.
	//					// If neither normal or texture co-ordinate data is present, this number will be 3.

	//if (indexed_normals.size() == 0)
	//	errors += 1;

	//if (indexed_texCoords.size() == 0)
	//	errors += 2;

	//unsigned int numVertices	= indexed_positions.size();		// Unsigned integer representing the amount of vertices the model has
	//unsigned int numIndices		= out_indices.size();				// Unsigned integer representing the amount of indices the model has

	//// Create file header
	//output.write(fileID, 4);
	//output.write((char*)&errors, sizeof(char));
	//output.write((char*)&numVertices, sizeof(unsigned int));
	//output.write((char*)&numIndices, sizeof(unsigned int));

	//// Write vertex data to file
	//output.seekp(13, std::ios::beg);
	//for (unsigned int i = 0; i < numVertices; i++)
	//{
	//	output.write((char*)&indexed_positions[i].x, sizeof(float));
	//	output.write((char*)&indexed_positions[i].y, sizeof(float));
	//	output.write((char*)&indexed_positions[i].z, sizeof(float));
	//}

	//for (unsigned int i = 0; i < numVertices; i++)
	//{
	//	output.write((char*)&indexed_normals[i].x, sizeof(float));
	//	output.write((char*)&indexed_normals[i].y, sizeof(float));
	//	output.write((char*)&indexed_normals[i].z, sizeof(float));
	//}

	//for (unsigned int i = 0; i < numVertices; i++)
	//{
	//	output.write((char*)&indexed_texCoords[i].x, sizeof(float));
	//	output.write((char*)&indexed_texCoords[i].y, sizeof(float));
	//}							

	//for (unsigned int i = 0; i < numIndices; i++)
	//{
	//	output.write((char*)&out_indices[i], sizeof(out_indices[0]));
	//}

	//output.close();
	//std::cout << " Done\n" << std::endl;

	//std::cout << outFileName << " finished with ";

	//if (errors != 0)
	//{
	//	if (errors == 1)
	//		std::cout << "1 error: no normal co-ordinate data present in " << inFileName << " (" << indexed_positions.size() << "vertices, " << out_indices.size() << "indices)\n\n";

	//	else if (errors == 2)
	//		std::cout << "1 error: no texture co-ordinate data present in " << inFileName << " (" << indexed_positions.size() << "vertices, " << out_indices.size() << "indices)\n\n";

	//	else if (errors == 3)
	//		std::cout << "2 errors: No normal or texture co-ordinate data present in " << inFileName << " (" << indexed_positions.size() << "vertices, " << out_indices.size() << "indices)\n\n";
	//}

	//else
	//	std::cout << "0 errors. (" << indexed_positions.size() << " vertices, " << out_indices.size() << " indices)\n\n";

	//std::cin.get();

	return 0;
}