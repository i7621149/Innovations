#ifndef SHADERLIBPRO_H__
#define SHADERLIBPRO_H__

#include "ngl/Singleton.h"
#include "ngl/Mat4.h"
#include "ngl/Camera.h"
#include <string>
#include <memory>
#include "Entity.h"

#include "ShaderSet.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ShaderLibPro.h
/// @brief management class for shaders
/// @author Felix Marrington-Reeve
/// @version 0.1
/// @struct ShaderPro
/// @brief mostly used to add shaders and use shaders. simple interaction means it is easy to use
//----------------------------------------------------------------------------------------------------------------------
class ShaderLibPro : public ngl::Singleton<ShaderLibPro>
{
  friend class ngl::Singleton<ShaderLibPro>;

public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add a ShaderSet to the library, loaded from text file
  /// @param[in] _sourceFile text file to source shader from
  //----------------------------------------------------------------------------------------------------------------------
  void addShader(const std::string &_sourceFile);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw an entity
  /// @param[in] _entity the entity to  draw
  /// @param[in] _cam camera to draw from, can be nullptr to use identity matrices
  //----------------------------------------------------------------------------------------------------------------------
  void draw(Entity *_entity, ngl::Camera *_cam);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief use a specific shader, specified when drawing, by the entity
  /// @param[in] _index the index of the shader to use
  //----------------------------------------------------------------------------------------------------------------------
  void useShader(size_t _index);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief find out how many ShaderSets there currently are
  /// @returns number of shaders
  //----------------------------------------------------------------------------------------------------------------------
  int getShaderSetAmount() {return m_shaderSets.size();}

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor initialises m_shaderSets and m_currentShaderIndex to 0
  //----------------------------------------------------------------------------------------------------------------------
  ShaderLibPro();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief destructor prints message to std::cout
  //----------------------------------------------------------------------------------------------------------------------
  ~ShaderLibPro();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of ShaderSets currently active
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<ShaderSet>> m_shaderSets;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief current shader being used
  //----------------------------------------------------------------------------------------------------------------------
  int m_currentShaderindex;
};

#endif//SHADERLIBPRO_H__
