# XVP 

![XVP Banner](xvpBanner.png)
> [!CAUTION]
The XVP SDK is no longer maintained or supported.

XVP was a passion project that I worked on for many years (2000-2007) in my spare time. Back then, Unity and Unreal weren't options. People built their own in-house game engines, typically in C++, and it required years of development to build one.

I'm publishing XVP 20 years later, primarily, to document it as a passion project, show C++ experience to potential employers, that sort of thing. It was a **huge** project.

Eventually Unity, Unreal, etc would democratize game development and make it accessible and affordable.


## What did XVP do?
It abstracted away the complexity of the underlying hardware and provided everything you need to build a game, from the ground up. 
- 3D Rendering
- 2D Rendering
- Bones/Skinning
- Audio
- Networking
- Input
- Physics
- UI
- Animation
- Scripting (Lua)
- Vector, Matrix, Quaternion math libraries
- 3D Max, Maya, Collada exporters

Eventually I'd end up building the X-VP Designer on top of the XVP SDK. It was a visual editor that allowed you to create games using the XVP SDK (think Unity). Many other tools followed, like the XVP Wad tool (Asset bundling), the XVP Managed wrapper (C# wrapper), the XVP ShaderGen compiler (Shader permutations), and the XVP LuaGen compiler (Lua scripting).

> [!NOTE]
You can probably get a good high level overview of XVP by reading the intro PDF in the docs folder. 
 
> ![XVP Introduction](docs/XVPIntro.pdf)


# Extensible Visualization Platform (X-VP) SDK

The Extensible Visualization Platform (X-VP) SDK is a comprehensive toolkit designed to facilitate the development of games and other visual applications.

## Companion Apps in the XVP Ecosystem
There are several large apps that are built on top of the XVP SDK.

- **XVP Designer**: A visual editor built in C#. Similar to what Unity is today. 
- **XVP Wad**: A stand alone cli for creating arbitrary, compressed bundles of assets with a manifest.
- **XVP Managed**: A managed C# wrapper around the XVP SDK. This was used to expose the C++ SDK to C# applications like the X-VP Designer (editor).
- **XVP ShaderGen** : A compiler for shader permutations for the SDK to create efficient shaders for the GPU.
- **XVP LuaGen** : A compiler for Lua scripts for the SDK. This was used to create the X-VP LuaGlue for game designers to create custom scripts for their games.
- **XVP Exporters** : A set of tools for exporting models and animations from 3D Studio Max, Maya, etc into the XVP scene format.
  
## Features

This XVP Introduction PDF in the docs folderis the best overview of what XVP was, it's design goals, and how it was used.

## Folder Structure

The `xvpsdk` directory is organized into several key folders. From math libraries to core graphics, networking and input functionality. 

- **`src`**: Contains the source code for the SDK. 
  - **`core`**: Core functionality of the SDK, including animation, graphics, and system utilities.
    - **Classes**:
      - **`X2DMovie`**: Manages 2D movie playback within the platform.
      - **`XAnimSprite`**: Handles sprite animations, providing control over frame sequences, playback speed, and looping.
      - **`XViewingFrustum`**: Represents the viewing frustum used in 3D rendering.
      - **`XActor`**: Represents an actor in the scene, handling its properties and behaviors.
      - **`XActionMap`**: Manages action mappings for input handling.
      - **`XActorContainer`**: Manages a collection of actors, providing methods to add, remove, and update actors within a scene.
      - **`XActorLuaGlue`**: Provides Lua scripting integration for actor objects.
      - **`XAnimController`**: Controls animation playback and timing.
      - **`XAnimTrack`**: Represents a track in an animation sequence.
      - **`XBaseSprite`**: Base class for sprite objects, providing basic rendering functionality.
      - **`XBitmap`**: Handles bitmap image data and operations.
      - **`XBone`**: Represents a bone in a skeletal animation system.
      - **`XBoundingBox`**: Defines an axis-aligned bounding box for collision detection.
      - **`XBsp`**: Manages Binary Space Partitioning (BSP) trees for efficient rendering.
      - **`XCamera`**: Represents a camera in the scene, managing view and projection matrices.
      - **`XCameraObject`**: Extends camera functionality with additional object-specific features.
      - **`XCharSet`**: Manages character sets for text rendering.
      - **`XChaseController`**: Implements a chase camera controller.
      - **`XColor`**: Represents color values and operations.
      - **`XConsole`**: Provides a console interface for debugging and command input.
      - **`XController`**: Base class for various controllers in the platform.
      - **`XCursor`**: Manages cursor properties and behavior.
      - **`XDebugUtilLib`**: Provides utilities for debugging and diagnostics.
      - **`XDecalMgr`**: Manages decals in the scene.
      - **`XDistortionPostEffect`**: Applies distortion post-processing effects.
      - **`XDistortionSC`**: Shader controller for distortion effects.
      - **`XDOFPostEffect`**: Implements depth of field post-processing effects.
      - **`XEdgePostEffect`**: Applies edge detection post-processing effects.
      - **`XGamePlayer`**: Manages game player properties and actions.
      - **`XGameServer`**: Handles game server operations and networking.
      - **`XGeometry`**: Manages geometric data and operations.
      - **`XGizmo`**: Represents a gizmo for manipulating objects in the scene.
      - **`XGizmoController`**: Controls gizmo interactions and transformations.
      - **`XGizmoMgr`**: Manages multiple gizmos within the scene.
      - **`XGizmoMove`**: Implements movement functionality for gizmos.
      - **`XGizmoRotate`**: Implements rotation functionality for gizmos.
      - **`XGizmoScale`**: Implements scaling functionality for gizmos.
      - **`XGrid`**: Represents a grid for spatial organization.
      - **`XHDRPostEffect`**: Applies high dynamic range post-processing effects.
      - **`XInputActionMap`**: Maps input actions to commands.
      - **`XIntersectLib`**: Provides intersection testing utilities.
      - **`XLight`**: Represents a light source in the scene.
      - **`XLightMgr`**: Manages multiple light sources.
      - **`XLookAtController`**: Implements a look-at camera controller.
      - **`XLua`**: Integrates Lua scripting into the platform.
      - **`XLuaScript`**: Manages Lua script execution.
      - **`XMatAnimController`**: Controls material animations.
      - **`XMatAnimTrack`**: Represents a track in a material animation sequence.
      - **`XMaterialColorSC`**: Shader controller for material color effects.
      - **`XMaterialList`**: Manages a list of materials.
      - **`XMathUtils`**: Provides mathematical utilities and functions.
      - **`XMatrix3x3`**: Represents a 3x3 matrix for transformations.
      - **`XMatrix4x4`**: Represents a 4x4 matrix for transformations.
      - **`XMemoryPool`**: Manages memory allocation pools.
      - **`XModel`**: Represents a 3D model in the scene.
      - **`XModelBlinnPhongSC`**: Shader controller for Blinn-Phong shading.
      - **`XModelLoader`**: Loads 3D models from files.
      - **`XModelLODController`**: Controls level of detail for models.
      - **`XModelNoLightSC`**: Shader controller for models without lighting.
      - **`XModelObject`**: Represents an object within a model.
      - **`XModelObjectTemplate`**: Template for creating model objects.
      - **`XModelTemplate`**: Template for creating models.
      - **`XMotionBlurPostEffect`**: Applies motion blur post-processing effects.
      - **`XNetAddress`**: Manages network address information.
      - **`XPacketPool`**: Manages packet memory pools.
      - **`XParentChildCombo`**: Represents a parent-child relationship.
      - **`XParticle`**: Represents a particle in a particle system.
      - **`XOctree`**: Manages an octree for spatial partitioning.
      - **`XParticleEmitter`**: Emits particles in a particle system.
      - **`XParticleSystem`**: Manages a system of particles.
      - **`XParticleSystemBlinnPhongSC`**: Shader controller for particle systems with Blinn-Phong shading.
      - **`XParticleSystemNoLightSC`**: Shader controller for particle systems without lighting.
      - **`XParticleSystemTemplate`**: Template for creating particle systems.
      - **`XPhysicsController`**: Controls physics interactions.
      - **`XPhysXController`**: Integrates PhysX physics engine.
      - **`XPlane`**: Represents a geometric plane.
      - **`XPolygon`**: Represents a polygon in 3D space.
      - **`XPose`**: Manages pose transformations.
      - **`XPostEffect`**: Base class for post-processing effects.
      - **`XProfiler`**: Provides profiling utilities.
      - **`XProfilerIterator`**: Iterates over profiling data.
      - **`XProfilerNode`**: Represents a node in the profiling hierarchy.
      - **`XPropertyBag`**: Manages a collection of properties.
      - **`XPRZController`**: Implements a pan, rotate, and zoom controller.
      - **`XQuaternion`**: Represents a quaternion for rotations.
      - **`XRandomNumberLib`**: Provides random number generation utilities.
      - **`XRay3D`**: Represents a 3D ray for intersection testing.
      - **`XRect`**: Represents a rectangle.
      - **`XResource`**: Manages a resource with reference counting.
      - **`XResourceList`**: Manages a list of resources.
      - **`XRsa`**: Implements RSA encryption and decryption.
      - **`XScene`**: Represents a scene in the platform.
      - **`XSceneGraph`**: Manages the scene graph hierarchy.
      - **`XSceneNode`**: Represents a node in the scene graph.
      - **`XScreenFader`**: Implements screen fade effects.
      - **`XShaderController`**: Controls shader parameters and states.
      - **`XShaderMgr`**: Manages shader resources.
      - **`XShadowCasterSSMSC`**: Shader controller for shadow casting.
      - **`XSkinGeometry`**: Manages skinned geometry data.
      - **`XSkinMesh`**: Represents a skinned mesh.
      - **`XSkinMeshTemplate`**: Template for creating skinned meshes.
      - **`XSkinObject`**: Represents a skinned object.
      - **`XSkinObjectTemplate`**: Template for creating skinned objects.
      - **`XSkyBox`**: Represents a skybox for environmental rendering.
      - **`XSmoothFilter`**: Applies a smooth filter to data.
      - **`XSprite`**: Represents a 2D sprite.
      - **`XStage`**: Manages a stage in the platform.
      - **`XState`**: Represents a state in a state machine.
      - **`XString`**: Manages string data and operations.
      - **`XTCPPacketizer`**: Manages TCP packetization.
      - **`XTexSeqSprite`**: Manages texture sequence sprites.
      - **`XTextureSequence`**: Manages a sequence of textures.
      - **`XTimer`**: Provides timing utilities.
      - **`XUFOController`**: Implements a UFO-style controller.
      - **`XUI2DMovie`**: Manages 2D movie elements in the UI.
      - **`XUIAnimSprite`**: Manages animated sprites in the UI.
      - **`XUIButton`**: Represents a button in the UI.
      - **`XUIComboBox`**: Represents a combo box in the UI.
      - **`XUIElement`**: Base class for UI elements.
      - **`XUIFrame`**: Represents a frame in the UI.
      - **`XUIHorizontalScrollbar`**: Manages horizontal scrollbars in the UI.
      - **`XUIHorizontalSlider`**: Manages horizontal sliders in the UI.
      - **`XUIListBox`**: Represents a list box in the UI.
      - **`XUIListView`**: Represents a list view in the UI.
      - **`XUIProgressBar`**: Represents a progress bar in the UI.
      - **`XUISpinner`**: Represents a spinner in the UI.
      - **`XUISprite`**: Represents a sprite in the UI.
      - **`XUITextbox`**: Represents a textbox in the UI.
      - **`XUIToggle`**: Represents a toggle button in the UI.
      - **`XUIVerticalScrollbar`**: Manages vertical scrollbars in the UI.
      - **`XUIVerticalSlider`**: Manages vertical sliders in the UI.
      - **`XUserIntefaceLuaGlue`**: Provides Lua scripting integration for UI elements.
      - **`XVector2D`**: Represents a 2D vector.
      - **`XUserInterface`**: Manages the user interface.
      - **`XVector3D`**: Represents a 3D vector.
      - **`XVector4D`**: Represents a 4D vector.
      - **`XVertexColorSC`**: Shader controller for vertex color effects.
      - **`XViewBindController`**: Binds view transformations to objects.
      - **`XVP`**: Main class for the Extensible Visualization Platform.
      - **`XVPLuaGlue`**: Provides Lua scripting integration for the platform.
      - **`XWad`**: Manages WAD file operations.
      - **`XWhiteSC`**: Shader controller for white shading effects.
    - **`xvpsdk.cpp`**: Main entry point for the SDK, initializing core components.

- **`inc`**: Header files necessary for building applications with the SDK. These files define the interfaces and structures used throughout the SDK.
  - **`core`**: Core headers for graphics, audio, input, and more.

- **`docs`**: Documentation for the SDK, including release notes and tutorials.
  - **`ReleaseNotes-v.01.html`**: Details the features and changes in version 0.1 of the SDK.
  - **`TutorialsExplained.html`**: Provides explanations and guidance on using the SDK's tutorials.

- **`exporters`**: Contains tools for exporting content to be used with the SDK.
  - **`XVPExport`**: Project files for building the exporter tools.

- **`artwork`**: Contains design assets used in the SDK.
  - **`xvp_final.ai`**: Adobe Illustrator file for the final design of the X-VP logo or related artwork.

- **`system/shaders/hlsl`**: Contains HLSL shader files used for rendering effects.
  - **`FinalLum.postfx`**: Shader for final luminance post-processing.
  - **`Copy.postfx`**: Shader for copying textures or render targets.
  - **`AdaptLum.postfx`**: Shader for adaptive luminance adjustments.

- **`.gitignore`**: Specifies files and directories that should be ignored by Git version control, such as build outputs, temporary files, and other non-essential items.

- **`XVP.vcxproj`**: Visual Studio project file for building the SDK.
- **`XVP.sln`**: Visual Studio solution file for managing the SDK's projects.

