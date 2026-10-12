#include "QuickGUIBrushPipeline.h"

#include "OgreGpuProgramManager.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreResourceGroupManager.h"
#include "OgreTextureManager.h"

namespace QuickGUI
{
	namespace
	{
		struct BlendFactors
		{
			Ogre::SceneBlendFactor sourceColour;
			Ogre::SceneBlendFactor destColour;
			Ogre::SceneBlendFactor sourceAlpha;
			Ogre::SceneBlendFactor destAlpha;
		};

		/// Indexed by BrushBlendMode.
		const BlendFactors BLEND_FACTORS[] =
		{
			{ Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ONE, Ogre::SBF_ZERO },
			{ Ogre::SBF_ZERO, Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE, Ogre::SBF_ONE },
			{ Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ZERO, Ogre::SBF_ONE },
			{ Ogre::SBF_ONE_MINUS_DEST_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE },
			{ Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE }
		};
	}

	BrushPipeline::BrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass) :
		mRenderSystem(renderSystem),
		mPass(pass),
		mSceneManager(NULL)
	{
	}

	void BrushPipeline::setSceneManager(Ogre::SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}

	void BrushPipeline::applyPassState()
	{
		if(mSceneManager != NULL)
		{
			mSceneManager->_setPass(mPass,false);
		}
	}

	FixedFunctionBrushPipeline::FixedFunctionBrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass, const Ogre::TextureUnitState::UVWAddressingMode& addressMode) :
		BrushPipeline(renderSystem,pass),
		mTextureAddressMode(addressMode)
	{
	}

	void FixedFunctionBrushPipeline::beginFrame()
	{
		// Legacy render systems rely on the fixed-function transform and state setup.
		mRenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
		mRenderSystem->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
		mRenderSystem->_setViewMatrix( Ogre::Matrix4::IDENTITY );

		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		mRenderSystem->setShadingType(Ogre::SO_GOURAUD);
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
		if(Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr())
		{
			Ogre::SamplerPtr sampler = tm->getDefaultSampler();
			sampler->setAddressingMode(mTextureAddressMode);
			mRenderSystem->_setSampler(0, *sampler);
		}
#else
		mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
#endif
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
		mRenderSystem->_disableTextureUnitsFrom(1);

		applyPassState();
	}

	void FixedFunctionBrushPipeline::targetChanged()
	{
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
	}

	void FixedFunctionBrushPipeline::setBlendMode(BrushBlendMode m)
	{
		const BlendFactors& f = BLEND_FACTORS[m];

		mRenderSystem->_setSeparateSceneBlending(f.sourceColour, f.destColour, f.sourceAlpha, f.destAlpha);
	}

	void FixedFunctionBrushPipeline::setFilterMode(BrushFilterMode m)
	{
		Ogre::TextureManager* tm = Ogre::TextureManager::getSingletonPtr();

		switch(m)
		{
		case BRUSHFILTER_NONE:
			tm->getDefaultSampler()->setFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
			break;
		case BRUSHFILTER_NEAREST:
			tm->getDefaultSampler()->setFiltering(Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_POINT);
			break;
		case BRUSHFILTER_LINEAR:
			tm->getDefaultSampler()->setFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
			break;
		}

		mRenderSystem->_setSampler(0, *tm->getDefaultSampler());
	}

	void FixedFunctionBrushPipeline::setTexture(Ogre::TexturePtr t)
	{
		mRenderSystem->_setTexture(0, true, t);
	}

	GlslBrushPipeline::GlslBrushPipeline(Ogre::RenderSystem* renderSystem, Ogre::Pass* pass, const GlslProgramSource& programs) :
		BrushPipeline(renderSystem,pass)
	{
		Ogre::HighLevelGpuProgramManager& programManager = Ogre::HighLevelGpuProgramManager::getSingleton();

		if(!programManager.resourceExists(programs.vertexName))
		{
			Ogre::HighLevelGpuProgramPtr vertexProgram = programManager.createProgram(
				programs.vertexName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"glsl",
				Ogre::GPT_VERTEX_PROGRAM);

			vertexProgram->setSource(programs.vertexSource);
			vertexProgram->setParameter("profiles", programs.profile);
			vertexProgram->setParameter("entry_point", programs.entryPoint);
			vertexProgram->load();
		}

		if(!programManager.resourceExists(programs.fragmentName))
		{
			Ogre::HighLevelGpuProgramPtr fragmentProgram = programManager.createProgram(
				programs.fragmentName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				"glsl",
				Ogre::GPT_FRAGMENT_PROGRAM);

			fragmentProgram->setSource(programs.fragmentSource);
			fragmentProgram->setParameter("profiles", programs.profile);
			fragmentProgram->setParameter("entry_point", programs.entryPoint);
			fragmentProgram->load();
		}

		mPass->setVertexProgram(programs.vertexName);
		mPass->setFragmentProgram(programs.fragmentName);

		Ogre::GpuProgramParametersSharedPtr fragmentParameters = mPass->getFragmentProgramParameters();
		if(!fragmentParameters.isNull())
		{
			fragmentParameters->setNamedConstant(programs.samplerName, 0);
		}
	}

	void GlslBrushPipeline::beginFrame()
	{
		applyPassState();
	}

	void GlslBrushPipeline::targetChanged()
	{
		applyPassState();
	}

	void GlslBrushPipeline::setBlendMode(BrushBlendMode m)
	{
		const BlendFactors& f = BLEND_FACTORS[m];

		mPass->setSeparateSceneBlending(f.sourceColour, f.destColour, f.sourceAlpha, f.destAlpha);

		applyPassState();
	}

	void GlslBrushPipeline::setFilterMode(BrushFilterMode m)
	{
		Ogre::TextureUnitState* tus = mPass->getTextureUnitState(0);

		if(tus != NULL)
		{
			switch(m)
			{
			case BRUSHFILTER_NONE:
				tus->setTextureFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
				break;
			case BRUSHFILTER_NEAREST:
				tus->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_POINT, Ogre::FO_POINT);
				break;
			case BRUSHFILTER_LINEAR:
				tus->setTextureFiltering(Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
				break;
			}
		}

		applyPassState();
	}

	void GlslBrushPipeline::setTexture(Ogre::TexturePtr t)
	{
		Ogre::TextureUnitState* tus = mPass->getTextureUnitState(0);

		if((tus != NULL) && !t.isNull())
		{
#if OGRE_VERSION >= ((1 << 16) | (13 << 8))
			tus->setTexture(t);
#else
			tus->setTextureName(t->getName(), t->getGroup());
#endif
		}

		applyPassState();
	}
}
