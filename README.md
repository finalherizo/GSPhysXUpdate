##Description
This is a test project to experiment with physX3 update loop and the kinematic actor speed problem.

##Dependencies
* [3DGameStudio](http://www.3dgamestudio.com/)
* [NVidia's PhysX-SDK-3.3-beta-2](https://developer.nvidia.com/rdp/physx-downloads)
* Create a file named Variables.props
<pre>
    <?xml version="1.0" encoding="utf-8"?> 
    <Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      <ImportGroup Label="PropertySheets" />
      <PropertyGroup Label="UserMacros">
        <GstudioDir>{PATH_TO_3DGS}</GstudioDir>
        <PhysX3_SDKs_Dir>{PATH_TO_PHYSX_SDK}</PhysX3_SDKs_Dir>
      </PropertyGroup>
      <PropertyGroup />
      <ItemDefinitionGroup />
      <ItemGroup>
        <BuildMacro Include="GstudioDir">
          <Value>$(GstudioDir)</Value>
          <EnvironmentVariable>true</EnvironmentVariable>
        </BuildMacro>
        <BuildMacro Include="PhysX3_SDKs_Dir">
          <Value>$(PhysX3_SDKs_Dir)</Value>
          <EnvironmentVariable>true</EnvironmentVariable>
        </BuildMacro>
      </ItemGroup>
    </Project>
</pre>
