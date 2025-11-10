function CreateDirectory {
    param( [string]$newDirectory)
    New-Item -ItemType Directory -Path $newDirectory
}

#$curBuildCombo = $env:BUILD_COMBO
#$curConfig = $env:CONFIGURATION
$curCompiler = $env:CI_COMPILER

# only package on Release builds, but don't error out
#if ($curConfig -ne 'Release') {
#    Write-Host 'Non release build detected, exiting packaging script...'
#    exit 0
#}

$isGccBuild = $curCompiler -eq 'gcc'
$isClangBuild = $curCompiler -eq 'clang'
#$isMingwBuild = $curBuildCombo -eq 'windows-mingw'
#$isMsvcBuild = $curBuildCombo -eq 'windows-msvc'
$qtVersion = if ($env:QT_VERSION) { $env:QT_VERSION } else { "6.10.0" };

Write-Host "Running packaging script..."
Write-Host "Current setup build combo is: $curBuildCombo"

# create dir to store package files
CreateDirectory ./build/package

if ($isLinux) {
    # copy libuncso2 to the package dir
    Copy-Item ./build/libuncso2/libuncso2.so* -Destination ./build/package/

    # copy libcryptopp to the package dir
    #Copy-Item ./build/libuncso2/external/cryptopp/libcryptopp.so* -Destination ./build/package/

    # copy AppImage prebuilt files
    Copy-Item ./tf.harmony.UnCSO2.* -Destination ./build/package/
    
    # copy uncso2 itself to the package dir
    Copy-Item ./build/uncso2 -Destination ./build/package/

}
elseif ($isWindows) {
    #if ($isMingwBuild) {
    #    # copy crypto++ to the package dir
    #    Copy-Item ./build/libuncso2/external/cryptopp/libcryptopp.dll -Destination ./build/package/
    #}
    
    # copy libuncso2 to the package dir
    Copy-Item ./build/libuncso2/*uncso2.dll -Destination ./build/package/
    
    # copy uncso2 itself to the package dir
    Copy-Item ./build/uncso2.exe -Destination ./build/package/
}
else {
    Write-Error 'An unknown OS is running this script, implement me.'
    exit 1
}

# copy breeze icons
Copy-Item ./build/icons-breeze.rcc -Destination ./build/package/

# copy license
Copy-Item ./COPYING -Destination ./build/package/

# copy README
Copy-Item ./README.md -Destination ./build/package/

# get app version
$versionStr = Get-Content -Path ./version.txt -TotalCount 1
Write-Host "UnCSO2 version: $versionStr"

Push-Location ./build

if ($isLinux) {
    # retrieve deployment tool
    wget -c "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
    wget -c "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage"
    chmod a+x linuxdeploy-x86_64.AppImage
    chmod a+x linuxdeploy-plugin-qt-x86_64.AppImage

    $env:VERSION = $versionStr;
    ./linuxdeploy-x86_64.AppImage --appdir=./package/ --library=./package/libuncso2.so --executable=./package/uncso2 --desktop-file=./package/tf.harmony.UnCSO2.desktop --icon-file=./package/tf.harmony.UnCSO2.svg --plugin qt --output appimage

    if ($isGccBuild) {
        Move-Item UnCSO2*.AppImage -Destination "../UnCSO2-$versionStr-linux-x86_64-gcc.AppImage"
    }
    elseif ($isClangBuild) {
        Move-Item UnCSO2*.AppImage -Destination "../UnCSO2-$versionStr-linux-x86_64-clang.AppImage"
    }
}
elseif ($isWindows) {
    Push-Location ./package

    $windeployBin = ''

    if ($env:QT_ROOT_DIR) {
        $windeployBin = "$env:QT_ROOT_DIR\bin\windeployqt.exe"
    }
    else {        
        $windeployBin = "C:\Qt\$qtVersion\msvc2022_64\bin\windeployqt.exe"
    }

    #if ($curConfig -eq 'Release') {
    #    if ($isMingwBuild) {
    #        & $windeployBin ./uncso2.exe  
    #    }
    #    else {
            & $windeployBin --release ./uncso2.exe  
    #    }
    #}
    #else {       
    #    & $windeployBin ./uncso2.exe  
    #}

    Pop-Location

    #if ($isMingwBuild) {       
    #    7z a -t7z -m0=lzma2 -mx=9 -mfb=64 -md=64m -ms=on "../UnCSO2-$versionStr-windows-x86_64_mingw.7z" ./package/*
    #}
    #elseif ($isMsvcBuild) {       
        7z a -t7z -m0=lzma2 -mx=9 -mfb=64 -md=64m -ms=on "../UnCSO2-$versionStr-windows-x86_64_msvc.7z" ./package/*
    #}
}

Pop-Location
