===
![VTK](https://img.shields.io/badge/VTK-8.1.2-red.svg)
![Qt](https://img.shields.io/badge/Qt-5.12.0-green.svg)

# How to build VTK
```
cmake ../VTK-8.1.2 -DBUILD_DOCUMENTATION:BOOL="1" -DDOCUMENTATION_HTML_HELP:BOOL="1" -DBUILD_TESTING:BOOL="1" -DBUILD_EXAMPLES:BOOL="1" -DModule_vtkGUISupportQtOpenGL:BOOL="1" -DModule_vtkDICOM:BOOL="1" -DModule_vtkGUISupportQtSQL:BOOL="1" -DModule_vtkViewsQt:BOOL="1" -DModule_vtkGUISupportQt:BOOL="1" -DModule_vtkRenderingQt:BOOL="1" -DModule_vtkRenderingExternal:BOOL="1" -DModule_vtkWebPython:BOOL="1" -DVTK_Group_Qt:BOOL="1" -DModule_vtkPython:BOOL="1" -DModule_vtkAddon:BOOL="1" -DVTK_WRAP_PYTHON:BOOL="1"
```

# How to build project
```
mkdir build
cd build
cmake .. -DVTK_DIR=/home/wayne/vtk/build-vtk-8.1.2
```

# About the project
- QQuickFramebufferObject + VTK
- model supported file type: image, mesh 3D file, volume rendering with DICOM.
- model manager
- mouse interaction
------

## 支持的模型类型有：

- 基本图元。
- bmp, jpeg, png, dicom 等格式图片
- STL,OBJ等3D面片模型
- 基于DICOM目录的体模型
