#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <windows.h>

#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>
#include <Overlay/OgreOverlaySystem.h>
#include <RTShaderSystem/OgreShaderGenerator.h>
#include <Bites/OgreCameraMan.h>

class OgreApplication_test {
public:
  OgreApplication_test() {}
  ~OgreApplication_test() {
    delete m_overlaySystem;
    delete m_root;
  }

  void setup();
  
private:
  void initResources();
  void createSceneManager();
  void createCamera();
  void createViewports();
  void initializeShaderGenerator();
  void loadResources();

  Ogre::Root* m_root = nullptr;
  Ogre::Camera* m_camera = nullptr;
  Ogre::SceneNode* m_cameraNode = nullptr;
  Ogre::SceneManager* m_sceneMgr = nullptr;
  Ogre::RenderWindow* m_window = nullptr;
  Ogre::String m_resourcesCfg = Ogre::StringUtil::BLANK;
  Ogre::String m_pluginsCfg = Ogre::StringUtil::BLANK;
	Ogre::OverlaySystem* m_overlaySystem = nullptr;
};

void OgreApplication_test::setup() {
  m_resourcesCfg = "code\\test\\test_data\\resources.cfg";
  m_pluginsCfg = "code\\test\\test_data\\plugins.cfg";

  m_root = new Ogre::Root(m_pluginsCfg);
  initResources();
  
  Ogre::RenderSystem* rs = m_root->getRenderSystemByName("Direct3D11 Rendering Subsystem");
  m_root->setRenderSystem(rs);
  rs->setConfigOption("Full Screen", "No");
  rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

  m_window = m_root->initialise(true, "Test Render Window");

  createSceneManager();
  createCamera();
  createViewports();
  initializeShaderGenerator(); //< MUST come before loading resources
  loadResources();
}

void OgreApplication_test::initResources() {
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(m_resourcesCfg);

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements()) {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i) {
          typeName = i->first;
          archName = i->second;
          Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
              archName, typeName, secName);
      }
  }
}

void OgreApplication_test::createSceneManager() {
  m_sceneMgr = m_root->createSceneManager();
	m_overlaySystem = new Ogre::OverlaySystem();
	m_sceneMgr->addRenderQueueListener(m_overlaySystem);
}

void OgreApplication_test::createCamera() {
  // Create the camera
  m_camera = m_sceneMgr->createCamera("PlayerCam");

  // Create a SceneNode and attach the camera to it
  m_cameraNode = m_sceneMgr->getRootSceneNode()->createChildSceneNode();
  m_cameraNode->attachObject(m_camera);
  // Position it at 500 in Z direction
  m_cameraNode->setPosition(Ogre::Vector3(0,0,500));
  // Look back along -Z
  m_cameraNode->lookAt(Ogre::Vector3(0,0,-300), Ogre::Node::TS_WORLD);
}

void OgreApplication_test::createViewports() {
    // Create one viewport, entire window
    Ogre::Viewport* vp = m_window->addViewport(m_camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    m_camera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


void OgreApplication_test::initializeShaderGenerator() {
  Ogre::RTShader::ShaderGenerator::initialize();
}

void OgreApplication_test::loadResources() {
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

TEST(OgreTest, ogre_scene_test) {
  OgreApplication_test testObj;
  testObj.setup();  // Create a basic application to make sure that everything is loaded and running

  EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}