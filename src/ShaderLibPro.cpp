#include "ShaderLibPro.h"

#include <iostream>
#include <QImage>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include "ShaderVariables.h"
#include "NGLScene.h"

ShaderLibPro::ShaderLibPro() :
  m_shaderSets(0),
  m_currentShaderindex(0)
{

}

ShaderLibPro::~ShaderLibPro()
{
  std::cout << "shutting down ShaderLibPro" << std::endl;
}

void ShaderLibPro::useShader(size_t _index)
{
  if(_index >= m_shaderSets.size())
  {
    std::cerr << "accessing invalid shader!" << std::endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    m_currentShaderindex = _index;
  }
}

void ShaderLibPro::addShader(const std::string &_sourceFile)
{
  m_shaderSets.push_back(std::unique_ptr<ShaderSet>(new ShaderSet(_sourceFile)));
}

void ShaderLibPro::draw(Entity *_entity, ngl::Camera *_cam)
{
  useShader(_entity->getShaderLibIndex());
  m_shaderSets[m_currentShaderindex]->draw(_entity, _cam);
}
