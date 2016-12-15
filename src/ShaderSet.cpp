#include "ShaderSet.h"

#include <iostream>
#include <QImage>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include "ShaderVariables.h"
#include "NGLScene.h"

ShaderSet::ShaderSet(const std::string &_sourceFile) :
  m_shaders(0)
{
  setShaderInfo(_sourceFile);
}


void ShaderSet::setShaderInfo(const std::string &_sourceFile)
{
  // reading in a shader, prints some information to std::cout
  std::cout << "loading shaders" << std::endl;
  std::ifstream file(_sourceFile);

  //check file is open
  if(!file.is_open())
  {
    std::cerr << "file not found" << _sourceFile << std::endl;
    exit(EXIT_FAILURE);
  }

  bool mainIsSet = false;
  std::string line;

  // go through first to set up each shader
  // this is premlinary so that frame buffers can be set up before
  while(std::getline(file, line)){
    std::istringstream iss(line);
    // split up string by whitespace
    std::vector<std::string> splitString((std::istream_iterator<std::string>(iss)), (std::istream_iterator<std::string>()));

    if(splitString.size() == 0)
    {
      // empty line
      continue;
    }

    if(splitString[0] == "SHADER")
    {
      std::cout << "new Shader " << std::endl;

      ShaderPro *shader = new ShaderPro;

      shader->m_progID = glCreateProgram();
      shader->m_name = splitString[1];

      // generate framebuffers if needed
      if(splitString[1] != "MAIN")
      {
        std::cout << "generating framebuffer" << splitString[1] << std::endl;

        GLuint bufferID;
        GLuint bufferTexID;
        GLuint bufferDepthStencilID;

        glGenFramebuffers(1, &bufferID);
        glGenTextures(1, &bufferTexID);
        glGenRenderbuffers(1, &bufferDepthStencilID);

        shader->m_outBufferID = bufferID;
        shader->m_outTextureID = bufferTexID;
        shader->m_outDepthStencilID = bufferDepthStencilID;
      }
      else
      {
        mainIsSet = true;
        std::cout << "MAIN shader found" << std::endl;
      }

      m_shaders.push_back(std::unique_ptr<ShaderPro>(shader));
    }
  }

  // give warning if no shader is "MAIN"
  if(!mainIsSet)
  {
    std::cerr << "WARNING: no main shader set!" << std::endl;
  }

  int lineNum = 1;
  size_t shaderNum = 0;
  ShaderPro *currentShader = nullptr;

  // reset file
  file.clear();
  file.seekg(0, file.beg);
  // go through file again to use what's been set up
  while(std::getline(file, line))
  {
    std::istringstream iss(line);
    std::vector<std::string> splitString((std::istream_iterator<std::string>(iss)), (std::istream_iterator<std::string>()));

    if(splitString.size() == 0)
    {
      // file line is empty
      continue;
    }

    if(splitString[0][0] == '#')
    {
      // line is a comment
      continue;
    }

    if(splitString[0] == "SHADER")
    {
      if(shaderNum < m_shaders.size())
      {
        currentShader = m_shaders[shaderNum].get();
      }
      else
      {
        std::cerr << "ERROR: trying to work on shader that doesn't fit, line " << lineNum << std::endl;
      }
    }
    // if we have a valid currentShader:
    else if(currentShader)
    {
      if(splitString[0] == "VERTEX")
      {
          std::cout << "Loading vertex shader: " << splitString[1] << std::endl;

          currentShader->m_vertID = glCreateShader(GL_VERTEX_SHADER);
          currentShader->m_vertFile = splitString[1];
      }
      else if(splitString[0] == "FRAGMENT")
      {
          std::cout << "Loading fragment shader: " << splitString[1] << std::endl;

          currentShader->m_fragID = glCreateShader(GL_FRAGMENT_SHADER);
          currentShader->m_fragFile = splitString[1];

      }
      else if(splitString[0] == "TEXTURE")
      {
          std::cout << "adding texture: ";

          TextureData texture;
          GLuint texID;

          if(splitString[1] == "2D")
          {
            std::cout << "2D" << std::endl;

            glGenTextures(1, &texID);

            texture.id = texID;

            texture.type = TEXTURE2D;
            texture.textureSource = splitString[2];
          }
          else if(splitString[1] == "CUBE")
          {
            std::cout << "Cube" << std::endl;

            glGenTextures(1, &texID);

            texture.id = texID;

            texture.type = TEXTURECUBE;
            texture.textureSource = splitString[2];
          }
          else if(splitString[1] == "BUFFER")
          {
            std::cout << "creating buffer" << std::endl;
            ShaderPro *shader = getShader(splitString[2]);

            if(shader)
            {
              texture.id = shader->m_outTextureID;
            }
            else
            {
              std::cerr << "loading shader " << splitString[2] << " failed!" <<std::endl;
              exit(EXIT_FAILURE);
            }

            texture.type = BUFFER;
            texture.textureSource = shader->m_name;
          }
          else
          {
            std::cerr << "ERROR: unrecognised texture type, line " << lineNum << std::endl;
          }

          currentShader->m_textures.push_back(texture);
      }

      else if(splitString[0] == "END")
      {
          std::cout << "end of Shader" << std::endl;

          currentShader = nullptr;
          shaderNum++;
      }
      else
      {
        std::cerr << "ERROR: glll line " << lineNum << std::endl;
      }
    }
    else
    {
      std::cerr << "ERROR: no current shader, line " << lineNum << std::endl;
    }
    lineNum++;
  }

  loadShaders();
}

void ShaderSet::draw(Entity *_entity, ngl::Camera *_cam)
{
  ngl::Mat4 M;
  ngl::Mat4 MVP;
  ngl::Mat4 MV;
  ngl::Mat3 normalMatrix;

  for(auto &shader : m_shaders)
  {
    // debug print
    //ShaderVariables::instance()->printVariables();

    glBindFramebuffer(GL_FRAMEBUFFER, shader->m_outBufferID);

    M = _entity->getTransformMatrix();
    if(_cam)
    {
      MVP = M * _cam->getVPMatrix();
      MV = M * _cam->getViewMatrix();
    }
    else
    {
      MVP = M;
      MV = M;
    }

    normalMatrix = MV;
    normalMatrix.inverse();

    ShaderVariables::instance()->M = M;
    ShaderVariables::instance()->MVP = MVP;
    ShaderVariables::instance()->MV = MV;
    ShaderVariables::instance()->normalMatrix = normalMatrix;

    ShaderVariables::instance()->matID = _entity->getMatID();

    glUseProgram(shader->m_progID);
    ShaderVariables::instance()->loadToShader(shader->m_progID);

    // multiple textures can be slow
    shader->texturesToShader();

    // debug print
    //shader->printShaderData();

    _entity->draw();
  }
}

void ShaderSet::loadShaders()
{
  for(auto &shader : m_shaders)
  {
    shader->compile();
  }
}

ShaderPro *ShaderSet::getShader(const std::string &_shaderName)
{
  for(auto &shader : m_shaders)
  {
    if(shader->m_name == _shaderName)
    {
      std::cout << "shader " << _shaderName << " found!" << std::endl;
      return shader.get();
    }
  }
  std::cerr << "ERROR: Shader " << _shaderName << " not found!" << std::endl;
  return nullptr;
}
