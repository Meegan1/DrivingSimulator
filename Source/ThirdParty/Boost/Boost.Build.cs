using UnrealBuildTool;
using System;
using System.IO;
 
public class Boost : ModuleRules
{
    public Boost(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
        bUseRTTI = true;
        bEnableExceptions = true;
        PublicDefinitions.Add("WITH_BOOST_BINDING=1");
        PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS=1");
    }
}