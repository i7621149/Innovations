#Template Project

This is a template project, in a similar vein to ngl's BlankNGL. It contains the recommended starting point for any projects that use the shader library.

This template includes ShaderLibPro, ShaderVariables, ShaderSet, ShaderPro and Entity, which are all required to use the library and are considered the components of the library.

It also includes an NGLScene class and Background, which inherits from Entity. Both of these are recommended for use with my classes, since they are good examples of how to use the library, and the NGLScene will deal with the required ShaderVariables automatically, so that the user does not need to set those up themselves. The Background class is also recommended since it shows how to inherit from Entity, and means that there will always be something in the background rather than the clear color. 

The user could rewrite or get rid of either of these if they prefer to use the library in their own way.

The template also comes with a default shader that shows how it can be written, in this case it is the same as the "new" option in ShaderToy.

##shader text files

The shader text files take the following format:
```
#comments are made with #s

#start with SHADER and shader name. one MAIN is required.
SHADER MAIN

  #specify fragment shader glsl file. This will be combined with sampler2Ds if there are textures and the BaseFragment.glsl file
  FRAGMENT   shaders/DefaultQuadFragment.glsl

  #specify vertex shader glsl file. This will be combined with the BaseVertex.glsl
  VERTEX     shaders/GeoVertex.glsl

  #textures can be specified with TEXTURE followed by type (2D) and then the file
  TEXTURE 2D textures/example.png
  TEXTURE 2D textures/example2.png

#end the shader
END MAIN

```
