echo off
echo ---请确保拷贝前编译完Debug和Release两种模式
pause

xcopy /Y .\Win32\Debug\YBCtrl.dll ..\..\Win32\Debug\
xcopy /Y .\Win32\Debug\YBCtrl.lib ..\..\Win32\Debug\Lib\
xcopy /Y .\Win32\Release\YBCtrl.dll ..\..\Win32\Release\
xcopy /Y .\Win32\Release\YBCtrl.lib ..\..\Win32\Release\Lib\
xcopy /Y .\Win32\Release\YBCtrl.pdb ..\..\Win32\Release\
xcopy /Y .\Win32\StaticLib_Debug\YBCtrl_static.lib ..\..\Win32\Debug\lib\
xcopy /Y .\Win32\StaticLib_Release\YBCtrl_static.lib ..\..\Win32\Release\lib\
xcopy /Y .\Win32\StaticLib_MultiByte_Debug\YBCtrl_static_MultiByte.lib ..\..\Win32\Debug\lib\
xcopy /Y .\Win32\StaticLib_MultiByte_Release\YBCtrl_static_MultiByte.lib ..\..\Win32\Release\lib\
xcopy /Y .\x64\Debug\YBCtrl.dll ..\..\x64\Debug\
xcopy /Y .\x64\Debug\YBCtrl.lib ..\..\x64\Debug\Lib\
xcopy /Y .\x64\Release\YBCtrl.dll ..\..\x64\Release\
xcopy /Y .\x64\Release\YBCtrl.lib ..\..\x64\Release\Lib\
xcopy /Y .\x64\Release\YBCtrl.pdb ..\..\x64\Release\
xcopy /Y .\x64\StaticLib_Debug\YBCtrl_static.lib ..\..\x64\Debug\lib\
xcopy /Y .\x64\StaticLib_Release\YBCtrl_static.lib ..\..\x64\Release\lib\
xcopy /Y .\x64\StaticLib_MultiByte_Debug\YBCtrl_static_MultiByte.lib ..\..\x64\Debug\lib\
xcopy /Y .\x64\StaticLib_MultiByte_Release\YBCtrl_static_MultiByte.lib ..\..\x64\Release\lib\
xcopy /Y .\YBCtrl\inc\button\*.* ..\..\include\ybctrl\button\
xcopy /Y .\YBCtrl\inc\frame\*.* ..\..\include\ybctrl\frame\
xcopy /Y .\YBCtrl\inc\loader\*.* ..\..\include\ybctrl\loader\
xcopy /Y .\YBCtrl\inc\checkbox\*.* ..\..\include\ybctrl\checkbox\
xcopy /Y .\YBCtrl\inc\menu\*.* ..\..\include\ybctrl\menu\
xcopy /Y .\YBCtrl\inc\progressbar\*.* ..\..\include\ybctrl\progressbar\
xcopy /Y .\YBCtrl\inc\radiobtn\*.* ..\..\include\ybctrl\radiobtn\
xcopy /Y .\YBCtrl\inc\static\*.* ..\..\include\ybctrl\static\
xcopy /Y .\YBCtrl\inc\dialog\*.* ..\..\include\ybctrl\dialog\
xcopy /Y .\YBCtrl\inc\scrollbar\*.* ..\..\include\ybctrl\scrollbar\
xcopy /Y .\YBCtrl\inc\scrollwnd\*.* ..\..\include\ybctrl\scrollwnd\
xcopy /Y .\YBCtrl\inc\view\*.* ..\..\include\ybctrl\view\
xcopy /Y .\YBCtrl\inc\tooltip\*.* ..\..\include\ybctrl\tooltip\
xcopy /Y .\YBCtrl\inc\splitter\*.* ..\..\include\ybctrl\splitter\
xcopy /Y .\YBCtrl\inc\treectrl\*.* ..\..\include\ybctrl\treectrl\
xcopy /Y .\Win32\Debug\YBCtrl.dll ..\..\..\branches\easy1key4yongxiong\Debug\
xcopy /Y .\Win32\Debug\YBCtrl.lib ..\..\..\branches\easy1key4yongxiong\Debug\Lib\
xcopy /Y .\Win32\Release\YBCtrl.dll ..\..\..\branches\easy1key4yongxiong\Release\
xcopy /Y .\Win32\Release\YBCtrl.lib ..\..\..\branches\easy1key4yongxiong\Release\Lib\
xcopy /Y .\Win32\Release\YBCtrl.pdb ..\..\..\branches\easy1key4yongxiong\Release\
xcopy /Y .\Win32\StaticLib_Debug\YBCtrl_static.lib ..\..\..\branches\easy1key4yongxiong\Debug\lib\
xcopy /Y .\Win32\StaticLib_Release\YBCtrl_static.lib ..\..\..\branches\easy1key4yongxiong\Release\lib\
xcopy /Y .\Win32\StaticLib_MultiByte_Debug\YBCtrl_static_MultiByte.lib ..\..\..\branches\easy1key4yongxiong\Debug\lib\
xcopy /Y .\Win32\StaticLib_MultiByte_Release\YBCtrl_static_MultiByte.lib ..\..\..\branches\easy1key4yongxiong\Release\lib\
xcopy /Y .\YBCtrl\inc\button\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\button\
xcopy /Y .\YBCtrl\inc\frame\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\frame\
xcopy /Y .\YBCtrl\inc\loader\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\loader\
xcopy /Y .\YBCtrl\inc\checkbox\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\checkbox\
xcopy /Y .\YBCtrl\inc\menu\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\menu\
xcopy /Y .\YBCtrl\inc\progressbar\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\progressbar\
xcopy /Y .\YBCtrl\inc\radiobtn\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\radiobtn\
xcopy /Y .\YBCtrl\inc\static\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\static\
xcopy /Y .\YBCtrl\inc\dialog\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\dialog\
xcopy /Y .\YBCtrl\inc\scrollbar\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\scrollbar\
xcopy /Y .\YBCtrl\inc\scrollwnd\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\scrollwnd\
xcopy /Y .\YBCtrl\inc\view\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\view\
xcopy /Y .\YBCtrl\inc\tooltip\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\tooltip\
xcopy /Y .\YBCtrl\inc\splitter\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\splitter\
xcopy /Y .\YBCtrl\inc\treectrl\*.* ..\..\..\branches\easy1key4yongxiong\include\ybctrl\treectrl\
echo ---拷贝完成
pause



