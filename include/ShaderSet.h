#ifndef SHADERSET_H__
#define SHADERSET_H__

#include <string>
#include <memory>
#include "ShaderPro.h"
#include "Entity.h"
#include "ngl/Camera.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ShaderSet.h
/// @brief contains a set of shaders
/// @author Felix Marrington-Reeve
/// @version 0.1
/// @class ShaderSet
/// @brief ShaderSets contain a vector of shaders and are responsible for managing them and making draw calls to Entities
//----------------------------------------------------------------------------------------------------------------------
class ShaderSet
{
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor from source file, taken in text form (see report)
  /// @param[in] _sourceFile the text file to read in a shader
  //----------------------------------------------------------------------------------------------------------------------
  ShaderSet(const std::string &_sourceFile);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~ShaderSet() = default;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used in constructor to read source file and generate shader vector
  /// @param[in] _sourceFile the text file to read in a shader
  //----------------------------------------------------------------------------------------------------------------------
  void setShaderInfo(const std::string &_sourceFile);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw the inputted entity
  /// @param[in] _entity the entity to draw
  /// @param[in] _cam the camera to use for MVP etc
  //----------------------------------------------------------------------------------------------------------------------
  void draw(Entity *_entity, ngl::Camera *_cam);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief compiles all shaders
  //----------------------------------------------------------------------------------------------------------------------
  void loadShaders();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return pointer to shader referred to
  /// @param[in] _shaderName the name given to the shader from the text file, eg "MAIN"
  /// @returns pointer to the shader needed
  //----------------------------------------------------------------------------------------------------------------------
  ShaderPro *getShader(const std::string &_shaderName);

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of unique pointers to shaders in the set
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<ShaderPro>> m_shaders;
};

#endif//SHADERLIBPRO_H__
