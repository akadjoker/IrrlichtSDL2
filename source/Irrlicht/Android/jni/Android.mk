LOCAL_PATH := $(call my-dir)/../..
IRRLICHT_LIB_PATH := $(LOCAL_PATH)/../../lib/Android

include $(CLEAR_VARS)

LOCAL_MODULE := Irrlicht
IRRLICHT_LIB_NAME := lib$(LOCAL_MODULE).a

LOCAL_CFLAGS := -D_IRR_ANDROID_PLATFORM_ -Wall -pipe -fno-exceptions -fno-rtti -fstrict-aliasing

ifndef NDEBUG
LOCAL_CFLAGS += -g -D_DEBUG
else
LOCAL_CFLAGS += -fexpensive-optimizations -O3
endif

LOCAL_C_INCLUDES := ../../../include

LOCAL_SRC_FILES := \
					aesGladman/aescrypt.cpp \
					aesGladman/aeskey.cpp \
					aesGladman/aestab.cpp \
					aesGladman/fileenc.cpp \
					aesGladman/hmac.cpp \
					aesGladman/prng.cpp \
					aesGladman/pwd2key.cpp \
					aesGladman/sha1.cpp \
					aesGladman/sha2.cpp \
					C3DSMeshFileLoader.cpp \
					CAnimatedMeshHalfLife.cpp \
					CAnimatedMeshMD2.cpp \
					CAnimatedMeshMD3.cpp \
					CAnimatedMeshSceneNode.cpp \
					CAttributes.cpp \
					CB3DMeshFileLoader.cpp \
					CBillboardSceneNode.cpp \
					CBoneSceneNode.cpp \
					CBSPMeshFileLoader.cpp \
					CBurningShader_Raster_Reference.cpp \
					CCameraSceneNode.cpp \
					CColladaFileLoader.cpp \
					CColladaMeshWriter.cpp \
					CColorConverter.cpp \
					CCSMLoader.cpp \
					CCubeSceneNode.cpp \
					CDefaultGUIElementFactory.cpp \
					CDefaultSceneNodeAnimatorFactory.cpp \
					CDefaultSceneNodeFactory.cpp \
					CDepthBuffer.cpp \
					CDMFLoader.cpp \
					CDummyTransformationSceneNode.cpp \
					CEmptySceneNode.cpp \
					CFileList.cpp \
					CFileSystem.cpp \
					CFPSCounter.cpp \
					leakHunter.cpp \
					CGeometryCreator.cpp \
					CGUIButton.cpp \
					CGUICheckBox.cpp \
					CGUIColorSelectDialog.cpp \
					CGUIComboBox.cpp \
					CGUIContextMenu.cpp \
					CGUIEditBox.cpp \
					CGUIEnvironment.cpp \
					CGUIFileOpenDialog.cpp \
					CGUIFont.cpp \
					CGUIImage.cpp \
					CGUIImageList.cpp \
					CGUIInOutFader.cpp \
					CGUIListBox.cpp \
					CGUIMenu.cpp \
					CGUIMeshViewer.cpp \
					CGUIMessageBox.cpp \
					CGUIModalScreen.cpp \
					CGUIScrollBar.cpp \
					CGUISkin.cpp \
					CGUISpinBox.cpp \
					CGUISpriteBank.cpp \
					CGUIStaticText.cpp \
					CGUITabControl.cpp \
					CGUITable.cpp \
					CGUIToolBar.cpp \
					CGUITreeView.cpp \
					CGUIWindow.cpp \
					CImage.cpp \
					CImageLoaderBMP.cpp \
					CImageLoaderDDS.cpp \
					CImageLoaderJPG.cpp \
					CImageLoaderPCX.cpp \
					CImageLoaderPNG.cpp \
					CImageLoaderPPM.cpp \
					CImageLoaderPSD.cpp \
					CImageLoaderRGB.cpp \
					CImageLoaderTGA.cpp \
					CImageLoaderWAL.cpp \
					CImageWriterBMP.cpp \
					CImageWriterJPG.cpp \
					CImageWriterPCX.cpp \
					CImageWriterPNG.cpp \
					CImageWriterPPM.cpp \
					CImageWriterPSD.cpp \
					CImageWriterTGA.cpp \
					CImageLoaderPVR.cpp \
					CIrrDeviceConsole.cpp \
					CIrrDeviceSDL.cpp \
					CIrrDeviceStub.cpp \
					CIrrMeshFileLoader.cpp \
					CIrrMeshWriter.cpp \
					CLightSceneNode.cpp \
					CLimitReadFile.cpp \
					CLMTSMeshFileLoader.cpp \
					CLogger.cpp \
					CLWOMeshFileLoader.cpp \
					CMD2MeshFileLoader.cpp \
					CMD3MeshFileLoader.cpp \
					CMemoryFile.cpp \
					CMeshCache.cpp \
					CMeshManipulator.cpp \
					CMeshSceneNode.cpp \
					CMeshTextureLoader.cpp \
					CMetaTriangleSelector.cpp \
					CMountPointReader.cpp \
					CMS3DMeshFileLoader.cpp \
					CMY3DMeshFileLoader.cpp \
					CNPKReader.cpp \
					CNullDriver.cpp \
					COBJMeshFileLoader.cpp \
					COBJMeshWriter.cpp \
					COCTLoader.cpp \
					COctreeSceneNode.cpp \
					COctreeTriangleSelector.cpp \
					CEGLManager.cpp \
					COGLESDriver.cpp \
					COGLESExtensionHandler.cpp \
					COGLESTexture.cpp \
					COgreMeshFileLoader.cpp \
					COSOperator.cpp \
					CPakReader.cpp \
					CParticleAnimatedMeshSceneNodeEmitter.cpp \
					CParticleAttractionAffector.cpp \
					CParticleBoxEmitter.cpp \
					CParticleCylinderEmitter.cpp \
					CParticleFadeOutAffector.cpp \
					CParticleGravityAffector.cpp \
					CParticleMeshEmitter.cpp \
					CParticlePointEmitter.cpp \
					CParticleRingEmitter.cpp \
					CParticleRotationAffector.cpp \
					CParticleScaleAffector.cpp \
					CParticleSphereEmitter.cpp \
					CParticleSystemSceneNode.cpp \
					CPLYMeshFileLoader.cpp \
					CPLYMeshWriter.cpp \
					CQ3LevelMesh.cpp \
					CQuake3ShaderSceneNode.cpp \
					CReadFile.cpp \
					CSceneCollisionManager.cpp \
					CSceneLoaderIrr.cpp \
					CSceneManager.cpp \
					CSceneNodeAnimatorCameraFPS.cpp \
					CSceneNodeAnimatorCameraMaya.cpp \
					CSceneNodeAnimatorCollisionResponse.cpp \
					CSceneNodeAnimatorDelete.cpp \
					CSceneNodeAnimatorFlyCircle.cpp \
					CSceneNodeAnimatorFlyStraight.cpp \
					CSceneNodeAnimatorFollowSpline.cpp \
					CSceneNodeAnimatorRotation.cpp \
					CSceneNodeAnimatorTexture.cpp \
					CShadowVolumeSceneNode.cpp \
					CSkinnedMesh.cpp \
					CSkyBoxSceneNode.cpp \
					CSkyDomeSceneNode.cpp \
					CSMFMeshFileLoader.cpp \
					CSoftwareDriver.cpp \
					CSoftwareDriver2.cpp \
					CSoftwareTexture.cpp \
					CSoftwareTexture2.cpp \
					CSphereSceneNode.cpp \
					CSTLMeshFileLoader.cpp \
					CSTLMeshWriter.cpp \
					CTarReader.cpp \
					CTerrainSceneNode.cpp \
					CTerrainTriangleSelector.cpp \
					CTextSceneNode.cpp \
					CTriangleBBSelector.cpp \
					CTriangleSelector.cpp \
					CVideoModeList.cpp \
					CVolumeLightSceneNode.cpp \
					CWADReader.cpp \
					CWaterSurfaceSceneNode.cpp \
					CWriteFile.cpp \
					CXMeshFileLoader.cpp \
					CXMLReader.cpp \
					CXMLWriter.cpp \
					CZBuffer.cpp \
					CZipReader.cpp \
					IBurningShader.cpp \
					Irrlicht.cpp \
					irrXML.cpp \
					os.cpp	 \
					bzip2/blocksort.c \
					bzip2/bzcompress.c \
					bzip2/bzlib.c \
					bzip2/crctable.c \
					bzip2/decompress.c \
					bzip2/huffman.c \
					bzip2/randtable.c \
					lzma/LzmaDec.c \
zlib/adler32.c   zlib/crc32.c    zlib/gzclose.c  zlib/gzread.c   zlib/infback.c  zlib/inflate.c   zlib/trees.c    zlib/zutil.c\
zlib/compress.c  zlib/deflate.c  zlib/gzlib.c    zlib/gzwrite.c  zlib/inffast.c  zlib/inftrees.c  zlib/uncompr.c

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)

all: $(IRRLICHT_LIB_PATH)
$(IRRLICHT_LIB_PATH) : $(TARGET_OUT)/$(IRRLICHT_LIB_NAME)
	cp $< $@

