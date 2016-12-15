#ifndef ENTITY_H__
#define ENTITY_H__

#include "ngl/Vec3.h"
#include "ngl/Transformation.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file Entity.h
/// @brief abstract class designed to be inherited from and use shader library with
/// @author Felix Marrington-Reeve
/// @version 0.1
/// @class Entity
/// @brief this class contains everything the shader library needs to use it
/// it also has information about position, rotation and scale, and functions to change the shader an instance uses
//----------------------------------------------------------------------------------------------------------------------
class Entity
{
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief construct Entity with id and position, as well as set m_alive to true
  /// @param[in] _id entity type, can be set by user and then used in shader
  /// @param[in] _pos initial position
  //----------------------------------------------------------------------------------------------------------------------
  Entity(int _id, ngl::Vec3 _pos);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief default destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~Entity() = default;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return transform matrix
  /// returns transform matrix of entity, based on pos, rot and scale
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 getTransformMatrix();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief get entity postion
  /// @returns m_pos
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getPos() {return m_pos;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief get entity velocity
  /// @returns m_vel
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getVel() {return m_vel;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief get entity rotation
  /// @returns m_rot
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getRot() {return m_rot;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief get entity scale
  /// @returns m_scale
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getScale() {return m_scale;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set enitity position
  /// @param[in] _pos postion
  //----------------------------------------------------------------------------------------------------------------------
  void setPos(ngl::Vec3 _pos) {m_pos = _pos;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set entity velocity
  /// @param[in] _vel velocity
  //----------------------------------------------------------------------------------------------------------------------
  void setVel(ngl::Vec3 _vel) {m_vel = _vel;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set entity rotation
  /// @param[in] _rot rotation
  //----------------------------------------------------------------------------------------------------------------------
  void setRot(ngl::Vec3 _rot) {m_rot = _rot;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set entity scale
  /// @param[in] _scale scale
  //----------------------------------------------------------------------------------------------------------------------
  void setScale(ngl::Vec3 _scale) {m_scale = _scale;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return the current shader index the entity is using
  /// @returns shader index for ShaderLibPro
  //----------------------------------------------------------------------------------------------------------------------
  int getShaderLibIndex() {return m_currentIndex + m_minIndex;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief return entity's current index
  /// @returns current index (relative to its minimum index)
  //----------------------------------------------------------------------------------------------------------------------
  int getCurrentIndex(){ return m_currentIndex;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set entity index
  /// @param[in] index (relative to its minimum index, eg input 0 to get its minimum)
  //----------------------------------------------------------------------------------------------------------------------
  void setCurrentIndex(int _index);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set the maximum and minimum shaders the shader can use
  /// @param[in] _min the minimum index for ShaderLibPro
  /// @param[in] _max the maximum index for ShaderLibPro
  //----------------------------------------------------------------------------------------------------------------------
  void resetIndexRange(int _min, int _max);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief find the minimum ShaderLibPro index the entity can have
  /// @returns the minimum index for ShaderLibPro
  //----------------------------------------------------------------------------------------------------------------------
  int getMinIndex(){return m_minIndex;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief find the maximum ShaderLibPro index the entity can have
  /// @returns the maximum index for ShaderLibPro
  //----------------------------------------------------------------------------------------------------------------------
  int getMaxIndex(){return m_maxIndex;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief check the entity type
  /// @returns material id
  //----------------------------------------------------------------------------------------------------------------------
  int getMatID() {return m_matID;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief check if the entity is alive
  /// @returns whether the entity is alive
  //----------------------------------------------------------------------------------------------------------------------
  bool isAlive() {return m_alive;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief virtual update function to be overwritten
  //----------------------------------------------------------------------------------------------------------------------
  virtual void update() = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief virtual draw function to be overwritten
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw() = 0;

protected :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief entity's material ID
  //----------------------------------------------------------------------------------------------------------------------
  int m_matID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief postion of entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_pos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief velocity of entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_vel = ngl::Vec3(0,0,0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief rotation of entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_rot = ngl::Vec3(0,0,0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief scale of entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_scale = ngl::Vec3(1,1,1);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief current shader index
  //----------------------------------------------------------------------------------------------------------------------
  int m_currentIndex = 0;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief minumum shader index allowed for entity
  //----------------------------------------------------------------------------------------------------------------------
  int m_minIndex = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief maximum shader index allowed for entity
  //----------------------------------------------------------------------------------------------------------------------
  int m_maxIndex = 99;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief whether the entity is alive/active
  //----------------------------------------------------------------------------------------------------------------------
  bool m_alive;
};

#endif//ENTITY_H__
