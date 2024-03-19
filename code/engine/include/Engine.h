#pragma  once

#include "EntityUtility.h"
#include "ECSSystem.h"
#include "FileSystem.h"
#include "Systems.h"
#include "ECS/Entity.h"
#include "Miscellaneous.h"

#include "ECS/Components/Name.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/MeshRenderer.h"
#include "ECS/Components/BoxCollider.h"
#include "ECS/Components/Text.h"

#include "ECS/Systems/TransformParentSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/TextRenderSystem.h"

#include "InputSystem/InputSystem.h"
#include "InputSystem/InputActionVec2.h"
#include "InputSystem/InputActionButton.h"
#include "InputSystem/GamepadInputID.h"