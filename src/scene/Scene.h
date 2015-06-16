/*
 * Copyright Benjamin Wang 2014.
 *
 * This source code may not be distributed, or used for any commercial or non-commerical
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_SCENE_H_
#define _WNTR_SCENE_H_

#include "AssetLoader.h"
#include "Renderer.h"
#include "RenderEffect.h"
#include "RenderPass.h"

#include "animation/Animation.h"
#include "animation/Animator.h"
#include "animation/TransformAnimation3D.h"

#include "geometry/DeferredRenderer.h"
#include "geometry/GInstancePass.h"
#include "geometry/GPass.h"
#include "geometry/GSPass.h"
#include "geometry/LCompositionEffect.h"
#include "geometry/LOrthoPass.h"
#include "geometry/LPointPass.h"
#include "geometry/LSOrthoEffect.h"
#include "geometry/LSPointEffect.h"
#include "geometry/Material.h"
#include "geometry/Mesh.h"
#include "geometry/MeshInstance.h"
#include "geometry/MeshCollection.h"
#include "geometry/Perspective.h"
#include "geometry/ShadowMap.h"
#include "geometry/View.h"

#include "ui/Bitmap.h"
#include "ui/TextField.h"
#include "ui/UIBitmapPass.h"
#include "ui/UIEventManager.h"
#include "ui/UIRenderer.h"

#endif
