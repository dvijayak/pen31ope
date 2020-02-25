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

// TODO: Should separate into a MeshLoader interface
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
    std::vector<Vector3> vertexNormals;
    struct FaceDef
    {
        std::array<uint, 3> vertexIds;
        std::array<uint, 3> vtIds;
        std::array<uint, 3> vnIds;
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
            // Vertex normals
            else if (first == "vn")
            {
                if (tokens.size() > 3)
                {
                    float normals[3];
                    for (int i = 0; i < 3; i++)
                    {
                        std::stringstream ss(tokens[i + 1]);
                        ss >> normals[i];
                    }
                    vertexNormals.push_back(Vector3(normals[0], normals[1], normals[2]));
                }
            }
            // Face definition
            else if (first == "f")
            {
                if (tokens.size() > 3)
                {
                    std::array<uint, 3> vertices;
                    std::array<uint, 3> textureCoords;
                    std::array<uint, 3> normals;
                    for (int i = 0; i < 3; i++)
                    {
                        std::vector<std::string> faceTokens = split(tokens[i + 1], '/');                        
                        {
                            std::stringstream ss(faceTokens[0]);
                            ss >> vertices[i];
                        }
                        if (faceTokens[1] != "")
                        {
                            std::stringstream ss(faceTokens[1]);
                            ss >> textureCoords[i];
                        }
                        if (faceTokens[2] != "")
                        {
                            std::stringstream ss(faceTokens[2]);
                            ss >> normals[i];
                        }
                    }
                    faces.push_back({vertices, textureCoords, normals});
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
            for (int i = 0; i < 3; i++)
            {
                // OBJ file indices start from 1, so we have to compensate
                expandedVertices[i].m_xyz = vertices[faceDef.vertexIds[i] - 1];
                expandedVertices[i].m_uv =  vertexTextureCoords[faceDef.vtIds[i] - 1];
                expandedVertices[i].m_normal = vertexNormals[faceDef.vnIds[i] - 1];
            }

            // Create the face from the vertices, compute surface normal, etc.
            face_type triangle(expandedVertices);
            triangle.m_normal = Normalized(Cross(triangle[1].xyz() - triangle[0].xyz(), triangle[2].xyz() - triangle[0].xyz()));
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