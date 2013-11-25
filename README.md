##Description
This is a test project to experiment with physX3 update loop and the kinematic actor speed problem.

##Dependencies
* [3DGameStudio](http://www.3dgamestudio.com/)
* [NVidia's PhysX-SDK-3.3-beta-2](https://developer.nvidia.com/rdp/physx-downloads)
* Create a file named Variables.props
    <code>
    &lt;?xml version="1.0" encoding="utf-8"?&gt; <br>
    &lt;Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"&gt; <br>
      &lt;ImportGroup Label="PropertySheets" /&gt; <br>
      &lt;PropertyGroup Label="UserMacros"&gt; <br>
        &lt;GstudioDir&gt;{PATH_TO_3DGS}&lt;/GstudioDir&gt; <br>
        &lt;PhysX3_SDKs_Dir&gt;{PATH_TO_PHYSX_SDK}&lt;/PhysX3_SDKs_Dir&gt; <br>
      &lt;/PropertyGroup&gt; <br>
      &lt;PropertyGroup /&gt; <br>
      &lt;ItemDefinitionGroup /&gt; <br>
      &lt;ItemGroup&gt; <br>
        &lt;BuildMacro Include="GstudioDir"&gt; <br>
          &lt;Value&gt;$(GstudioDir)&lt;/Value&gt; <br>
          &lt;EnvironmentVariable&gt;true&lt;/EnvironmentVariable&gt; <br>
        &lt;/BuildMacro&gt; <br>
        &lt;BuildMacro Include="PhysX3_SDKs_Dir"&gt; <br>
          &lt;Value&gt;$(PhysX3_SDKs_Dir)&lt;/Value&gt; <br>
          &lt;EnvironmentVariable&gt;true&lt;/EnvironmentVariable&gt; <br>
        &lt;/BuildMacro&gt; <br>
      &lt;/ItemGroup&gt; <br>
    &lt;/Project&gt; <br>
    </code>
