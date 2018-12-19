#!/bin/bash

basepath="$(cd "$(dirname "$0")" && pwd)"
IDLxmlname="$1"
AutogeneratedBuildPath="$2"
"$basepath/bin/act.linux" $IDLxmlname
TempHeaderFolder=Lib3MF_component
rm -rf $AutogeneratedBuildPath/Bindings
rm -rf $AutogeneratedBuildPath/Source/Implementation
mkdir -p $AutogeneratedBuildPath/Bindings
mkdir -p $AutogeneratedBuildPath/Source/Implementation
cp -r $TempHeaderFolder/Bindings $AutogeneratedBuildPath/Bindings
cp -r $TempHeaderFolder/Implementations/Cpp/Interfaces $AutogeneratedBuildPath/Source/Implementation
rm -rf $TempHeaderFolder
