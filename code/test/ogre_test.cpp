#include <gtest/gtest.h>

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <Overlay/OgreOverlaySystem.h>
#include <Bites/OgreCameraMan.h>

class OgreApplication_test {
public:
  OgreApplication_test() {}
  ~OgreApplication_test() {
    delete m_overlaySystem;
    delete m_window;
    delete m_sceneMgr;
    delete m_camera;
    delete m_root;
  }

  void setup();
  
private:
  void initResources();
  void createSceneManager();

  Ogre::Root* m_root = nullptr;
  Ogre::Camera* m_camera = nullptr;
  Ogre::SceneManager* m_sceneMgr = nullptr;
  Ogre::RenderWindow* m_window = nullptr;
  Ogre::String m_resourcesCfg = Ogre::StringUtil::BLANK;
  Ogre::String m_pluginsCfg = Ogre::StringUtil::BLANK;
	Ogre::OverlaySystem* m_overlaySystem = nullptr;
  
  OgreBites::CameraMan* m_cameraMan;       // basic camera controller
};

void OgreApplication_test::setup() {
  m_resourcesCfg = "code\\test\\test_data\\resources.cfg";
  m_pluginsCfg = "code\\test\\test_data\\plugins.cfg";

  m_root = new Ogre::Root(m_pluginsCfg);
  initResources();

  if (!m_root->showConfigDialog(nullptr)) {
    return;
  }

  m_window = m_root->initialise(true, "Test Render Window");
  createSceneManager();
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

TEST(OgreTest, ogre_scene_test) {
  OgreApplication_test testObj;
  testObj.setup();  // Create a basic application to make sure that everything is loaded and running

  EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}