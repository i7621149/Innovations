#ifndef TEXTUREDATA_H__
#define TEXTUREDATA_H__

#include "ngl/Util.h"
#include "string"

//----------------------------------------------------------------------------------------------------------------------
/// @file TextureData.h
/// @brief This struct contains data for textures used in shaders
/// @author Felix Marrington-Reeve
/// @version 0.1
/// @enum textureType
/// @brief refers to 3 texture types. Only TEXTURE2D is currently fully supported
//----------------------------------------------------------------------------------------------------------------------
enum textureType {TEXTURE2D, TEXTURECUBE, BUFFER};
//----------------------------------------------------------------------------------------------------------------------
/// @struct TextureData
/// @brief This struct is used by ShaderPro and contains texture data like id, type, and the sourcefile
//----------------------------------------------------------------------------------------------------------------------
struct TextureData
{
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the id of the texture
  //----------------------------------------------------------------------------------------------------------------------
  GLuint id;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the type of the texture, either 2d, cube or buffer
  //----------------------------------------------------------------------------------------------------------------------
  textureType type;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the location of the shader's source file/image
  //----------------------------------------------------------------------------------------------------------------------
  std::string textureSource;
};

#endif//TEXTUREDATA_H__
