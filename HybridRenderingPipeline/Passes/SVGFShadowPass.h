// This class is to shading for the direct lighting

#pragma once
#include "../SharedUtils/RenderPass.h"
#include "../SharedUtils/FullscreenLaunch.h"

class SVGFShadowPass : public ::RenderPass, inherit_shared_from_this<::RenderPass, SVGFShadowPass>
{
public:
	using SharedPtr = std::shared_ptr<SVGFShadowPass>;

	static SharedPtr create(const std::string& bufferToAccumulate, const std::string& inputColorBuffer);
	virtual ~SVGFShadowPass() = default;

protected:
	SVGFShadowPass(const std::string& bufferToAccumulate, const std::string& inputColorBuffer);

	// Implementation of SimpleRenderPass interface
	bool initialize(RenderContext* pRenderContext, ResourceManager::SharedPtr pResManager) override;
	void execute(RenderContext* pRenderContext) override;
	void renderGui(Gui* pGui) override;
	void resize(uint32_t width, uint32_t height) override;

	// The RenderPass class defines various methods we can override to specify this pass' properties. 
	bool appliesPostprocess() override { return true; }
	void clearBuffers(RenderContext* pRenderContext);

	std::string                   mOutputTexName;
  std::string                   mInputTexName;

	// State for our shader
	GraphicsState::SharedPtr      mpGfxState;

	// We stash a copy of our current scene.  Why?  To detect if changes have occurred.
	Scene::SharedPtr              mpScene;

	// SVGF shaders
	FullscreenLaunch::SharedPtr   mpReprojection;
	FullscreenLaunch::SharedPtr   mpAtrous;
	FullscreenLaunch::SharedPtr   mpFilterMoments;

  // Intermediate framebuffers
  Fbo::SharedPtr mpPingPongFbo[2];
  Fbo::SharedPtr mpLinearZAndNormalFbo;
  Fbo::SharedPtr mpFilteredPastFbo;
  Fbo::SharedPtr mpCurReprojFbo;
  Fbo::SharedPtr mpPrevReprojFbo;
  Fbo::SharedPtr mpFilteredIlluminationFbo;

	bool    mBuffersNeedClear    = false;
  bool    mFilterEnabled       = true;
  int32_t mFilterIterations    = 4;
  int32_t mFeedbackTap         = 1;
  float   mVarainceEpsilon     = 1e-4f;
  float   mPhiColor            = 10.0f;
  float   mPhiNormal           = 128.0f;
  float   mAlpha               = 0.05f;
  float   mMomentsAlpha        = 0.2f;

private:
  void allocateFbos(glm::uvec2 dim);
  void computeReprojection(RenderContext* pRenderContext,
                            Texture::SharedPtr pMotionVectorTexture,
                            Texture::SharedPtr pPositionNormalFwidthTexture,
                            Texture::SharedPtr pCurLinearZTexture,
                            Texture::SharedPtr pPrevLinearZAndNormalTexture);
  void computeFilteredMoments(RenderContext* pRenderContext, Texture::SharedPtr pCurLinearZTexture);
  void computeAtrousDecomposition(RenderContext* pRenderContext, Texture::SharedPtr pCurLinearZTexture);
};
