#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "ngl/NGLInit.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/NGLStream.h"
#include "ngl/Random.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "ShaderLibPro.h"
#include "ShaderVariables.h"

NGLScene::NGLScene() :
  m_fullScreen(false),
  m_mouseDown(false),
  m_time(QTime::currentTime()),
  m_lastFrameTime(0),
  m_mouseData(0,0,0,0),
  m_cam(ngl::Vec3::zero(), ngl::Vec3(0, 0, 1), ngl::Vec3::up())
{
  setTitle("Template Project");
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  // set resolution in shader
  // uses a Vec3 to be compatible with Shadertoy
  ShaderVariables::instance()->resolution = ngl::Vec3(m_width, m_height, 1.0);

  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
  // set resolution in shader
  // uses a Vec3 to be compatible with Shadertoy
  ShaderVariables::instance()->resolution = ngl::Vec3(m_width, m_height, 1.0);

  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

void NGLScene::initializeGL()
{
  // init ngl
  ngl::NGLInit::instance();

  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  ShaderLibPro *shader = ShaderLibPro::instance();

  // create background quad
  m_background.createQuad();

  // this is the system currently used for setting min/max shaders, works relatively well
  // and means that shader is set with 0, 1, 2 etc rather than arbritary numbers
  // could be more elegant
  int minIndex = 0;
  int maxIndex = 0;

  // background shaders
  // can add shaders with addShader function
  shader->addShader("shaders/backgroundDefault.txt");
  maxIndex = shader->getShaderSetAmount() - 1;

  m_background.resetIndexRange(minIndex, maxIndex);
  m_background.setCurrentIndex(0);

  // set up timer loop
  startTimer(16);
}

void NGLScene::paintGL()
{
  // increase frame number variable for shader
  ShaderVariables::instance()->frame++;
  // clear the screen and depth buffer
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_width, m_height);

  ShaderVariables::instance()->printVariables();

  // takes a null pointer for camera, which means it will be drawn without transforms
  ShaderLibPro::instance()->draw(&m_background, nullptr);

  // calculate time taken to render the frame (time since last frame was rendered)
  float renderTime = (m_time.elapsed() - m_lastFrameTime) / 1000.0;
  ShaderVariables::instance()->timeDelta = renderTime;

  m_lastFrameTime = m_time.elapsed();
}

void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
  // update mouse position, again used from shadertoy
  if(m_mouseDown)
  {
    m_mouseData[0] = _event->x()*devicePixelRatio();
    m_mouseData[1] = m_height-_event->y()*devicePixelRatio();
  }
}

void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  // mouse reading reverse engineered from shadertoy
  m_mouseData[2] = _event->x()*devicePixelRatio();
  m_mouseData[3] = m_height-_event->y()*devicePixelRatio();
  m_mouseDown = true;
}

void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // mouse reading reverse engineered from shadertoy
  m_mouseData[2] *= -1;
  m_mouseData[3] *= -1;
  m_mouseDown = false;
}

void NGLScene::wheelEvent(QWheelEvent *_event)
{

}

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value
  switch(_event->key())
  {
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;

      // render wireframe or shaded, mostly for bugfixing
      // wireframe makes things fast and look broken
    case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
    case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;

      // toggle fullscreen because who has time for two buttons for this
      // can make things very slow
    case Qt::Key_F : toggleFullScreen(); break;
  }

}

void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{

}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  // set shader variables:
  // time, date and mouse

  // getting seconds by dividing milliseconds by 1000
  float globalSeconds = m_time.elapsed()/1000.0;
  ShaderVariables::instance()->globalTime = globalSeconds;

  QDate date = QDate::currentDate();
  float dateYear = date.year();
  float dateMonth = date.month();
  float dateDay = date.day();
  float dateSeconds = (m_time.msecsSinceStartOfDay() + m_time.elapsed()) / 1000.0;
  ShaderVariables::instance()->date = ngl::Vec4(dateYear, dateMonth, dateDay, dateSeconds);

  ShaderVariables::instance()->mouse = m_mouseData;

  // draw
  update();
}

void NGLScene::toggleFullScreen()
{
  // complex fullscreen shaders can be expensive and slow, so be careful making window fullscreen (or just large)
  m_fullScreen ^= true;
  m_fullScreen ? showFullScreen() : showNormal();
}
