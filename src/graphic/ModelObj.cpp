#include "ModelObj.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <clocale>
#include <cstdio>
#include <string>
#include "Shader.hpp"
#include "utils/glError.hpp"

using namespace std;


struct GLfloat3
{
	GLfloat x,y,z;
	GLfloat3(GLfloat a, GLfloat b, GLfloat c):x(a),y(b),z(c){};
};

struct GLfloat2
{
	GLfloat x,y;
	GLfloat2(GLfloat a, GLfloat b):x(a),y(b){};
};

struct face
{
	GLuint n[9];
};

ModelObj::ModelObj(const char * filename):
    ModelObj(filename,ShaderProgram::loadFromFile(
        "shader/geometryPass.vert",
        "shader/geometryPass.frag"
    ))
{
    hasTexture = true;
}

ModelObj::ModelObj(const char * filename, ShaderProgram& _shader):
    hasTexture(false),
    shader(_shader),
    texture(Texture::loadFromFile(
        "texture/texture.png"
    ))
{
    glCheckError(__FILE__,__LINE__);
    vbo=0;

	ifstream file;
    
    // file open
	file.open(filename);
    if (not file.is_open())
    {
        cerr<<"[Error] the file "<<filename<<"doesn't exist"<<endl;
        exit(EXIT_FAILURE);
    }

	vector<GLfloat3> vertex;
	vector<GLfloat3> normals;
	vector<GLfloat2> texture;
    vector<GLfloat> vertexArray;
	vector<face> faces;

    // file parsing
	string identifier;
	while(file>>identifier)
	{
		if (identifier=="v")
		{
			double x,y,z;
			file>>x>>y>>z;
			vertex.push_back(GLfloat3(x,y,z));
		}
		else if (identifier=="vn")
		{
			double x,y,z;
			file>>x>>y>>z;
			normals.push_back(GLfloat3(x,y,z));
		}
        else if (identifier=="vt")
        {
            double x,y;
            file>>x>>y;
            texture.push_back(GLfloat2(x,y));
        }
		else if (identifier=="f")
		{
			getline(file,identifier);
			setlocale(LC_ALL, "en_US.utf8");
			face f;
			sscanf(identifier.c_str(),"%d/%d/%d %d/%d/%d %d/%d/%d",
					&f.n[0],
					&f.n[1],
					&f.n[2],
					&f.n[3],
					&f.n[4],
					&f.n[5],
					&f.n[6],
					&f.n[7],
					&f.n[8]
			);
			for(int i=0;i<9;++i)
				f.n[i]--;
			faces.push_back(f);
		}
	}
	cout<<vertex.size()<<endl;
	cout<<normals.size()<<endl;
	cout<<faces.size()<<endl;
    faceNb = faces.size();

    // vertexArray construction
	for(int i=0;i<faceNb;++i)
	{
        int f=0;
		for(int a=0;a<3;++a)
		{
            auto fv = faces[i].n[f++];
            auto ft = faces[i].n[f++];
            auto fn = faces[i].n[f++];

			vertexArray.push_back(vertex[fv].x);
			vertexArray.push_back(vertex[fv].y);
			vertexArray.push_back(vertex[fv].z);

			vertexArray.push_back(normals[fn].x);
			vertexArray.push_back(normals[fn].y);
			vertexArray.push_back(normals[fn].z);

			vertexArray.push_back(texture[ft].x);
			vertexArray.push_back(-texture[ft].y);
		}
	}

	file.close();

    // vbo allocation
    glGenBuffers( 1, &vbo );

    // vbo filling
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexArray.size(), &vertexArray[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // vao allocation
    glGenVertexArrays(1,&vao);

    cout<<"vao="<<vao<<"vbo="<<vbo<<endl;

    // vao filling
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    shader.setAttribute("position", 3, GL_FALSE, 8, 0);
    shader.setAttribute("normal"  , 3, GL_FALSE, 8, 3);
    shader.setAttribute("texCoord"   , 2, GL_FALSE, 8, 6);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glCheckError(__FILE__,__LINE__);
}

ModelObj::~ModelObj()
{
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}


void ModelObj::draw()
{
    shader.use();

    if (hasTexture)
    {
        texture.bind(GL_TEXTURE0);
        shader.setUniform("texture0",0);
    }

    glBindVertexArray(vao);

     
	glDrawArrays(
		GL_TRIANGLES,      // mode
        0,                 // first
		faceNb*3           // count
	);

    glBindVertexArray(0);
    shader.unuse();
}

ShaderProgram& ModelObj::getShader()
{
    return shader;
}
