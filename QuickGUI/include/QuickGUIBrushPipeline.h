#ifndef QUICKGUIBRUSHPIPELINE_H
#define QUICKGUIBRUSHPIPELINE_H

#include "QuickGUIBrushEnums.h"
#include "QuickGUIExportDLL.h"

#include "OgreMatrix4.h"
#include "OgrePass.h"
#include "OgreRenderSystem.h"
#include "OgreSceneManager.h"
#include "OgreTexture.h"
#include "OgreTextureUnitState.h"

#include <string>

namespace QuickGUI
{
	/// How the Brush puts its state on the render system.
	class _QuickGUIExport BrushPipeline
	{
	public:
		BrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass);
		virtual ~BrushPipeline() {}

		/// Configures the render system before the first draw of a frame.
		virtual void beginFrame() = 0;
		/// Called after the render target has been set on the render system.
		virtual void targetChanged() = 0;
		virtual void setBlendMode(BrushBlendMode m) = 0;
		virtual void setFilterMode(BrushFilterMode m) = 0;
		virtual void setTexture(Ogre::TexturePtr t) = 0;

		/// The scene manager the pass is set through.
		void setSceneManager(Ogre::SceneManager* sceneManager);

	protected:
		/// Puts the state of the pass on the render system.
		void applyPassState();

		Ogre::RenderSystem* mRenderSystem;
		Ogre::Pass* mPass;
		Ogre::SceneManager* mSceneManager;
	};

	/// Sets state directly on the render system, for render systems with a fixed-function pipeline.
	class _QuickGUIExport FixedFunctionBrushPipeline :
		public BrushPipeline
	{
	public:
		FixedFunctionBrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass, const Ogre::TextureUnitState::UVWAddressingMode& addressMode);

		void beginFrame();
		void targetChanged();
		void setBlendMode(BrushBlendMode m);
		void setFilterMode(BrushFilterMode m);
		void setTexture(Ogre::TexturePtr t);

	protected:
		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode;
	};

	/// The programs of the GlslBrushPipeline, which read the vertex attributes vertex, colour and uv0.
	struct _QuickGUIExport GlslProgramSource
	{
		std::string vertexName;
		std::string vertexSource;
		std::string fragmentName;
		std::string fragmentSource;
		std::string profile;
		std::string entryPoint;
		/// Name of the sampler uniform in the fragment source, bound to texture unit 0.
		std::string samplerName;
	};

	/// Draws through GLSL programs set on the material pass, for render systems that have no fixed-function pipeline.
	class _QuickGUIExport GlslBrushPipeline :
		public BrushPipeline
	{
	public:
		GlslBrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass, const GlslProgramSource& programs);

		void beginFrame();
		void targetChanged();
		void setBlendMode(BrushBlendMode m);
		void setFilterMode(BrushFilterMode m);
		void setTexture(Ogre::TexturePtr t);
	};
}

#endif
