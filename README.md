##Description
This is a test project to experiment with physX3 update loop and the kinematic actor speed problem.

##Dependencies
* [3DGameStudio](http://www.3dgamestudio.com/)
* [NVidia's PhysX-SDK-3.3-beta-2](https://developer.nvidia.com/rdp/physx-downloads)
* Create a file named Variables.props
    <code>
    &lt;?xml version="1.0" encoding="utf-8"?&gt; 
    &lt;Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"&gt;
      &lt;ImportGroup Label="PropertySheets" /&gt;
      &lt;PropertyGroup Label="UserMacros"&gt;
        &lt;GstudioDir&gt;{PATH_TO_3DGS}&lt;/GstudioDir&gt;
        &lt;PhysX3_SDKs_Dir&gt;{PATH_TO_PHYSX_SDK}&lt;/PhysX3_SDKs_Dir&gt;
      &lt;/PropertyGroup&gt;
      &lt;PropertyGroup /&gt;
      &lt;ItemDefinitionGroup /&gt;
      &lt;ItemGroup&gt;
        &lt;BuildMacro Include="GstudioDir"&gt;
          &lt;Value&gt;$(GstudioDir)&lt;/Value&gt;
          &lt;EnvironmentVariable&gt;true&lt;/EnvironmentVariable&gt;
        &lt;/BuildMacro&gt;
        &lt;BuildMacro Include="PhysX3_SDKs_Dir"&gt;
          &lt;Value&gt;$(PhysX3_SDKs_Dir)&lt;/Value&gt;
          &lt;EnvironmentVariable&gt;true&lt;/EnvironmentVariable&gt;
        &lt;/BuildMacro&gt;
      &lt;/ItemGroup&gt;
    &lt;/Project&gt;
    </code>
