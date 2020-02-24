#include "Mesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Util.hpp"

static std::vector<std::string> split (std::string const& s, char delim)
{
    std::vector<std::string> tokens;

    const uint bufSize = 1024;
    char buf[bufSize];
    std::stringstream ss(s);
    while (ss.good())
    {
        ss.getline(buf, bufSize, delim);
        tokens.push_back(buf);
    }

    return std::move(tokens);
}

std::unique_ptr<Mesh> Mesh::MakeFromOBJ (std::string const& fileName)
{
    // Attempt to open file
    std::ifstream ifs(fileName, std::ifstream::in);    

    // If failed, return
    if (ifs.fail())
    {
        std::cerr << "Could not open file " << fileName << std::endl;
        return 0;
    }

    std::vector<Vector3> vertices;
    std::vector<Vector2> vertexTextureCoords;
    struct FaceDef
    {
        std::array<uint, 3> vertexIds;
        std::array<uint, 3> vtIds;
    };
    std::vector<FaceDef> faces;

    // Read each line
    const uint bufSize = 1024;
    char buf[1024];
    while (ifs.good())
    {
        ifs.getline(buf, bufSize);

        // Tokenize line by space
        std::vector<std::string> tokens = split(buf, ' ');
        tokens.erase(std::remove(tokens.begin(), tokens.end(), ""), tokens.end());
        if (!tokens.empty())
        {
            auto const& first = tokens[0];

            // Vertex definition
            if (first == "v")
            {
                if (tokens.size() > 3)
                {
                    float components[3];
                    for (int i = 0; i < 3; i++)
                    {
                        std::stringstream ss(tokens[i + 1]);
                        ss >> components[i];
                    }                    
                    vertices.push_back(Vector3(components[0], components[1], components[2]));
                }

            }
            // Texture coordinates
            else if (first == "vt")
            {
                if (tokens.size() > 3)
                {
                    float uv[2];
                    for (int i = 0; i < 2; i++)
                    {
                        std::stringstream ss(tokens[i + 1]);
                        ss >> uv[i];
                    }
                    vertexTextureCoords.push_back(Vector2(uv[0], uv[1]));
                }
            }
            // Face definition
            else if (first == "f")
            {
                if (tokens.size() > 3)
                {
                    std::array<uint, 3> vertices;
                    std::array<uint, 3> textureCoords;
                    for (int i = 0; i < 3; i++)
                    {
                        std::vector<std::string> faceTokens = split(tokens[i + 1], '/');
                        // only care about the vertex index and vt for now
                        {
                            std::stringstream ss(faceTokens[0]);
                            ss >> vertices[i];
                        }
                        if (faceTokens[1] != "")
                        {
                            std::stringstream ss(faceTokens[1]);
                            ss >> textureCoords[i];
                        }
                    }
                    faces.push_back({vertices, textureCoords});
                }
            }
            // Ignore everything else
        }
    }

    if (ifs.eof())
    {
        // On EOF, prepare the Mesh object and quit
        auto pMesh = std::make_unique<Mesh>();
        for (auto const& faceDef : faces)
        {
            Triangle::vertices_type expandedVertices;
            Triangle::uvs_type expandedVTs;
            for (int i = 0; i < 3; i++)
            {
                expandedVertices[i] = vertices[faceDef.vertexIds[i] - 1]; // OBJ file indices start from 1, so compensate
                expandedVTs[i] = vertexTextureCoords[faceDef.vtIds[i] - 1];
            }

            // Create the face from the vertices, compute surface normal, etc.
            face_type triangle(expandedVertices);
            triangle.m_normal = Normalized(Cross(triangle[1] - triangle[0], triangle[2] - triangle[0]));            
            triangle.m_vertexUVs = expandedVTs;
            pMesh->m_faces.push_back(triangle);
        }
        return pMesh;
    }
    else if (ifs.bad())
    {
        // On error, just quit
        std::cerr << "Encountered error when reading file " << fileName << std::endl;
        return nullptr;
    }

    return nullptr;
}