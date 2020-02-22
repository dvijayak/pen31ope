#include "Mesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

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
    std::vector<std::array<uint, 3>> faces;

    // Read each line
    const uint bufSize = 1024;
    char buf[1024];
    while (ifs.good())
    {
        ifs.getline(buf, bufSize);

        // Tokenize line by space
        std::vector<std::string> tokens = split(buf, ' ');
        if (!tokens.empty())
        {
            std::string const& first = tokens[0];

            // Handle vertex definition
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
            // Handle face definition
            else if (first == "f")
            {
                if (tokens.size() > 3)
                {
                    std::array<uint, 3> vertices;
                    for (int i = 0; i < 3; i++)
                    {
                        std::vector<std::string> faceTokens = split(tokens[i + 1], '/');
                        std::stringstream ss(faceTokens[0]); // only care about the vertex index for now
                        ss >> vertices[i];
                    }
                    faces.push_back(vertices);
                }
            }
            // Ignore everything else
        }
    }

    if (ifs.eof())
    {
        // On EOF, prepare the Mesh object and quit
        auto pMesh = std::make_unique<Mesh>();
        for (auto const& face : faces)
        {
            Triangle::vertices_type expandedFaces;
            for (int i = 0; i < 3; i++)
            {
                expandedFaces[i] = vertices[face[i] - 1]; // OBJ file indices start from 1, so compensate
            }

            // Create the face from the vertices, compute surface normal, etc.
            face_type triangle(expandedFaces);            
            triangle.m_normal = Normalized(Cross(triangle[1] - triangle[0], triangle[2] - triangle[0]));            
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